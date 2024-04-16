#include "pch.h"
#include "RenderManager.h"
#include "ResourceManager.h"

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

namespace GraphicsEngine
{
	void RenderManager::Initalize(HWND _hwnd, float _height, float _width, Color _color)
	{
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
		m_pImGuiTool->Init(_hwnd, m_pGraphics->GetDevice(), m_pGraphics->GetDeviceContext());

		m_pImGuiTool->SetFloat3("DirectionColor", (float*) & (m_CBDirectionLightData.m_DircetionColor), -1.f, 1.f);
		m_pImGuiTool->SetFloat3("Light Direction", (float*) & (m_CBDirectionLightData.m_Direction), -1.f, 1.f);

		// ī�޶� ����
		//Vector3 eye = Vector3(0.f, 100.f, -400.f);
		//Vector3 target = Vector3(eye.x, eye.y, 0.f);
		//Vector3 up = Vector3(0.f, 1.f, 0.f);
		//m_CBCameraData.m_CameraPosition = eye;
		//m_CBCameraData.m_View = XMMatrixLookAtLH(eye, target, up);
		//m_CBCameraData.m_Projection = XMMatrixPerspectiveFovLH((45.f * 3.14f / 180.f), m_pGraphics->GetWidth() / m_pGraphics->GetHieght(), 0.1f, 50000.f);
		//m_CBCameraData.m_View = m_CBCameraData.m_View.Transpose();
		//m_CBCameraData.m_Projection = m_CBCameraData.m_Projection.Transpose();

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

		m_pImGuiTool->Render();

		m_pGraphics->RenderEnd();
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

			// �� �Ʒ��� ���� �ʿ�.
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

	void RenderManager::SetSkeletalMeshInstance(shared_ptr<SkeletalMeshInstance> _meshInstance)
	{
		// ��ü���� ������ �ϴٰ� �����Ѵ�.
		m_pSkeletalMeshInstanceVec[_meshInstance->GetMaterial()].push_back(_meshInstance);
	}

	void RenderManager::SetStaticMeshInstance(shared_ptr<StaticMeshInstance> _meshInstance)
	{
		// ��ü���� ������ �ϴٰ� �����Ѵ�.
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
}