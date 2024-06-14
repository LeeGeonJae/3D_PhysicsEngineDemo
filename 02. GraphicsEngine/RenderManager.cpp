#include "pch.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "DebugDraw.h"

#include "Graphics.h"
#include "PipeLine.h"
#include "ImGuiTool.h"

#include "SkeletalMeshInstance.h"
#include "SkeletalMeshResource.h"
#include "StaticMeshInstance.h";
#include "StaticMeshResource.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Node.h"
#include <Psapi.h>


namespace GraphicsEngine
{
	void RenderManager::Initalize(HWND _hwnd, float _height, float _width, DirectX::SimpleMath::Color _color)
	{
		HRESULT hr = 0;

		// Create DXGI factory
		hr = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)m_pDXGIFactory.GetAddressOf());
		assert(SUCCEEDED(hr));
		hr = m_pDXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(m_pDXGIAdapter.GetAddressOf()));
		assert(SUCCEEDED(hr));

		m_pGraphics = make_shared<Graphics>();
		m_pPipeLine = make_shared<PipeLine>();
		m_pImGuiTool = make_shared<ImGuiTool>();
		
		GraphicsEngine::GraphicsInfo graphicsinfo;
		graphicsinfo.m_Height = _height;
		graphicsinfo.m_Width = _width;
		graphicsinfo.m_hwnd = _hwnd;
		graphicsinfo.m_ClearColor = _color;
		m_pGraphics->Initalize(graphicsinfo);
		m_pPipeLine->Initalize(m_pGraphics->GetDevice(), m_pGraphics->GetDeviceContext(), static_cast<UINT>(TextureType::END));
		//m_pImGuiTool->Init(_hwnd, m_pGraphics->GetDevice(), m_pGraphics->GetDeviceContext());

		//m_pImGuiTool->SetFloat3("DirectionColor", (float*) & (m_CBDirectionLightData.m_DircetionColor), -1.f, 1.f);
		//m_pImGuiTool->SetFloat3("Light Direction", (float*) & (m_CBDirectionLightData.m_Direction), -1.f, 1.f);

		DebugDraw::Initialize(DEVICE, m_pGraphics->GetDeviceContext());

		createConstantBuffer();
	}

	void RenderManager::Update()
	{
		DEVICE_CONTEXT->UpdateSubresource(m_pCBCamera.Get(), 0, nullptr, &m_CBCameraData, 0, 0);
		DEVICE_CONTEXT->UpdateSubresource(m_pCBDirectionLight.Get(), 0, nullptr, &m_CBDirectionLightData, 0, 0);

		DEVICE_CONTEXT->VSSetConstantBuffers(1, 1, m_pCBCamera.GetAddressOf());
		DEVICE_CONTEXT->VSSetConstantBuffers(2, 1, m_pCBDirectionLight.GetAddressOf());
		DEVICE_CONTEXT->PSSetConstantBuffers(1, 1, m_pCBCamera.GetAddressOf());
		DEVICE_CONTEXT->PSSetConstantBuffers(2, 1, m_pCBDirectionLight.GetAddressOf());
	}

	void RenderManager::Render()
	{
		m_pGraphics->RenderBegin();

		m_pPipeLine->StateSetDefault();
		SortSkeletalMeshInstance();
		renderStaticMeshInstance();
		RenderPhysics();

		RenderDebug();
		//m_pImGuiTool->Render();

		m_pGraphics->RenderEnd();
	}

	void RenderManager::RenderDebug()
	{
		DebugDraw::g_BatchEffect->Apply(m_pGraphics->GetDeviceContext().Get());
		DebugDraw::g_BatchEffect->SetView(m_CBCameraData.m_View.Transpose());
		DebugDraw::g_BatchEffect->SetProjection(m_CBCameraData.m_Projection.Transpose());

		m_pGraphics->GetDeviceContext()->IASetInputLayout(DebugDraw::g_pBatchInputLayout.Get());

		DebugDraw::g_Batch->Begin();

		{
			for (auto boundingBox : m_DebugBoxes)
			{
				DebugDraw::Draw(DebugDraw::g_Batch.get(), *boundingBox.get(), DirectX::Colors::Yellow);
			}
			for (auto boindingSphere : m_DebugSpheres)
			{
				DebugDraw::Draw(DebugDraw::g_Batch.get(), *boindingSphere.get(), DirectX::Colors::Yellow);
			}
			for (auto Line : m_DebugLines)
			{
				DebugDraw::DrawRay(DebugDraw::g_Batch.get(), Line->Pos0, Line->Pos1 - Line->Pos0, false, Line->Color);
			}
			for (auto polygon : m_DebugPolygon)
			{
				for (int i = 0; i < polygon.size(); i++)
				{
					if (i + 1 >= polygon.size())
					{
						DebugDraw::DrawRay(DebugDraw::g_Batch.get(), polygon[i], polygon[0] - polygon[i], false,  DirectX::Colors::Yellow);
						break;
					}

					DebugDraw::DrawRay(DebugDraw::g_Batch.get(), polygon[i], polygon[i + 1] - polygon[i], false, DirectX::Colors::Yellow);
				}
			}
			for (auto point : m_DebugPoints)
			{
				DebugDraw::DrawRay(DebugDraw::g_Batch.get(), *point.get(), *point.get(), true, DirectX::Colors::Red);
			}
		}

		DebugDraw::g_Batch->End();

		m_DebugBoxes.clear();
		m_DebugSpheres.clear();
		m_DebugLines.clear();
		m_DebugPolygon.clear();
	}

	void RenderManager::SortSkeletalMeshInstance()
	{
		shared_ptr<Shader> shader = RESOURCE->Find<Shader>("SkeletalMeshShader");

		DEVICE_CONTEXT->IASetInputLayout(shader->GetInputLayout().Get());
		DEVICE_CONTEXT->VSSetShader(shader->GetVertexShader().Get(), nullptr, 0);
		DEVICE_CONTEXT->PSSetShader(shader->GetPixelShader().Get(), nullptr, 0);

		for (auto& material : m_pSkeletalMeshInstanceVec)
		{
			UINT stride = sizeof(BoneWeightVertex);
			UINT offset = 0;

			CB_Material CBMaterialData;
			CB_MatrixPalette CBMatrixPaletteData;
			CB_ModelTransform CBModelTransformData;

			CBMaterialData.m_baseColor = material.first->GetBaseColor();
			CBMaterialData.m_emissiveColor = material.first->GetEmissiveColor();

			
			for (int i = 0; i < static_cast<int>(TextureType::END); i++)
			{
				ComPtr<ID3D11ShaderResourceView> texture = nullptr;
				if (material.first->GetTexture(static_cast<TextureType>(i)) != nullptr)
				{
					texture = material.first->GetTexture(static_cast<TextureType>(i))->GetTexture();
				}
				m_pPipeLine->SetTexture(i, texture);
			}

			DEVICE_CONTEXT->UpdateSubresource(m_pCBMaterial.Get(), 0, nullptr, &CBMaterialData, 0, 0);
			DEVICE_CONTEXT->PSSetConstantBuffers(11, 1, m_pCBMaterial.GetAddressOf());

			for (auto& meshInstance : material.second)
			{
				meshInstance->UpdateMatrixPallete(&CBMatrixPaletteData);
				DEVICE_CONTEXT->UpdateSubresource(m_pCBBoneTransformPallete.Get(), 0, nullptr, &CBMatrixPaletteData, 0, 0);
				DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, m_pCBModelTransform.GetAddressOf());
				DEVICE_CONTEXT->VSSetConstantBuffers(12, 1, m_pCBBoneTransformPallete.GetAddressOf());

				DEVICE_CONTEXT->IASetVertexBuffers(0, 1, meshInstance->GetSkeletalMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				DEVICE_CONTEXT->IASetIndexBuffer(meshInstance->GetSkeletalMesh()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				DEVICE_CONTEXT->DrawIndexed(static_cast<UINT>(meshInstance->GetSkeletalMesh()->GetIndices().size()), 0, 0);
			}
		}
	}

	void RenderManager::renderStaticMeshInstance()
	{
		shared_ptr<Shader> shader = RESOURCE->Find<Shader>("StaticMeshShader");


		DEVICE_CONTEXT->IASetInputLayout(shader->GetInputLayout().Get());
		DEVICE_CONTEXT->VSSetShader(shader->GetVertexShader().Get(), nullptr, 0);
		DEVICE_CONTEXT->PSSetShader(shader->GetPixelShader().Get(), nullptr, 0);

		for (auto& material : m_pStaticMeshInstanceVec)
		{
			UINT stride = sizeof(Vertex);
			UINT offset = 0;

			CB_Material CBMaterialData;
			CB_MatrixPalette CBMatrixPaletteData;
			CB_ModelTransform CBModelTransformData;
			CBMaterialData.m_baseColor = material.first->GetBaseColor();
			CBMaterialData.m_emissiveColor = material.first->GetEmissiveColor();
			
			for (int i = 0; i < static_cast<int>(TextureType::END); i++)
			{
				ComPtr<ID3D11ShaderResourceView> texture = nullptr;
				if (material.first->GetTexture(static_cast<TextureType>(i)) != nullptr)
				{
					texture = material.first->GetTexture(static_cast<TextureType>(i))->GetTexture();
				}
				m_pPipeLine->SetTexture(i, texture);
			}

			// 이 아래로 수정 필요.
			DEVICE_CONTEXT->UpdateSubresource(m_pCBMaterial.Get(), 0, nullptr, &CBMaterialData, 0, 0);

			for (auto& meshInstance : material.second)
			{
				CBModelTransformData.m_World = meshInstance->GetNode()->GetWorldTransform().Transpose();
				DEVICE_CONTEXT->UpdateSubresource(m_pCBModelTransform.Get(), 0, nullptr, &CBModelTransformData, 0, 0);

				DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, m_pCBModelTransform.GetAddressOf());
				DEVICE_CONTEXT->VSSetConstantBuffers(4, 1, m_pCBBoneTransformPallete.GetAddressOf());
				DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, m_pCBModelTransform.GetAddressOf());
				DEVICE_CONTEXT->PSSetConstantBuffers(3, 1, m_pCBMaterial.GetAddressOf());
				DEVICE_CONTEXT->PSSetConstantBuffers(5, 1, m_pCBbIsTexture.GetAddressOf());

				DEVICE_CONTEXT->IASetVertexBuffers(0, 1, meshInstance->GetStaticMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				DEVICE_CONTEXT->IASetIndexBuffer(meshInstance->GetStaticMesh()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				DEVICE_CONTEXT->DrawIndexed(static_cast<UINT>(meshInstance->GetStaticMesh()->GetIndices().size()), 0, 0);
			}
		}

		m_pStaticMeshInstanceVec.clear();
	}

	void RenderManager::SetPhysicsBuffer(vector<PhysicsVertex>& vertex, vector<unsigned int>& index)
	{
		m_IndexBuffer.Reset();
		m_VertexBuffer.Reset();

		m_PhysicsVertex = vertex;
		m_PhysicsIndex = index;

		// 건재 : 버텍스 및 인덱스 버퍼 생성
		HRESULT hr;

		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = static_cast<UINT>(sizeof(PhysicsVertex) * m_PhysicsVertex.size());
		vertexDesc.CPUAccessFlags = 0;
		vertexDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &m_PhysicsVertex[0];

		hr = DEVICE->CreateBuffer(&vertexDesc, &initData, m_VertexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC indexDesc;
		indexDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexDesc.ByteWidth = static_cast<UINT>(sizeof(UINT) * m_PhysicsIndex.size());
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = 0;
		indexDesc.MiscFlags = 0;

		initData.pSysMem = &m_PhysicsIndex[0];

		hr = DEVICE->CreateBuffer(&indexDesc, &initData, m_IndexBuffer.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
	void RenderManager::RenderPhysics()
	{
		shared_ptr<Shader> shader = RESOURCE->Find<Shader>("PhysicsShader");

		DEVICE_CONTEXT->IASetInputLayout(shader->GetInputLayout().Get());
		DEVICE_CONTEXT->VSSetShader(shader->GetVertexShader().Get(), nullptr, 0);
		DEVICE_CONTEXT->PSSetShader(shader->GetPixelShader().Get(), nullptr, 0);

		UINT stride = sizeof(PhysicsVertex);
		UINT offset = 0;

		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		DEVICE_CONTEXT->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		DEVICE_CONTEXT->DrawIndexed(static_cast<UINT>(m_PhysicsIndex.size()), 0, 0);


	}

	void RenderManager::SetSkeletalMeshInstance(shared_ptr<SkeletalMeshInstance> _meshInstance)
	{
		// 물체들은 불투명 하다고 가정한다.
		m_pSkeletalMeshInstanceVec[_meshInstance->GetMaterial()].push_back(_meshInstance);
	}

	void RenderManager::SetStaticMeshInstance(shared_ptr<StaticMeshInstance> _meshInstance)
	{
		// 물체들은 불투명 하다고 가정한다.
		m_pStaticMeshInstanceVec[_meshInstance->GetMaterial()].push_back(_meshInstance);
	}

	void RenderManager::createConstantBuffer()
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CB_ModelTransform);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = DEVICE->CreateBuffer(&bd, nullptr, m_pCBModelTransform.GetAddressOf());
		assert(SUCCEEDED(hr));

		bd.ByteWidth = sizeof(CB_Camera);
		hr = DEVICE->CreateBuffer(&bd, nullptr, m_pCBCamera.GetAddressOf());
		assert(SUCCEEDED(hr));

		bd.ByteWidth = sizeof(CB_DirectionLight);
		hr = DEVICE->CreateBuffer(&bd, nullptr, m_pCBDirectionLight.GetAddressOf());
		assert(SUCCEEDED(hr));

		bd.ByteWidth = sizeof(CB_Material);
		hr = DEVICE->CreateBuffer(&bd, nullptr, m_pCBMaterial.GetAddressOf());
		assert(SUCCEEDED(hr));

		bd.ByteWidth = sizeof(CB_MatrixPalette);
		hr = DEVICE->CreateBuffer(&bd, nullptr, m_pCBBoneTransformPallete.GetAddressOf());
		assert(SUCCEEDED(hr));

		bd.ByteWidth = sizeof(CB_UseTextureMap);
		hr = DEVICE->CreateBuffer(&bd, nullptr, m_pCBbIsTexture.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	/// 비디오 메모리 읽기 ///
	void RenderManager::GetVideoMemoryInfo(std::string& out)
	{
		DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
		m_pDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);

		out = std::to_string(videoMemoryInfo.CurrentUsage / 1024 / 1024) + " MB" + " / " + std::to_string(videoMemoryInfo.Budget / 1024 / 1024) + " MB";
	}


	/// 시스템 메모리 읽기 ///
	void RenderManager::GetSystemMemoryInfo(std::string& out)
	{
		HANDLE hProcess = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS_EX pmc;
		pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
		GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		out = std::to_string((pmc.PagefileUsage) / 1024 / 1024) + " MB";
	}
}