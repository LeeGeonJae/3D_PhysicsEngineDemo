#pragma once

#include <dxgi1_4.h>
#include <DirectXtk/BufferHelpers.h>
#include <DirectXCollision.h>

namespace GraphicsEngine
{
	// ���� : Ŭ���� ����
	class Graphics;
	class PipeLine;
	class SkeletalMeshInstance;
	class StaticMeshInstance;
	class Texture;
	class Material;
	class ImGuiTool;

	// ���� : ConstantBuffer ����ü
	struct CB_ModelTransform
	{
		// ���� : ����ƽ �޽� Ʈ������
		Matrix m_World;
	};

	struct CB_Camera
	{
		Matrix m_View;
		// �� �ٲ��� �ʴ� �����ʹ� ���� ������۸� ����� ������ �Ѵ�!
		Matrix m_Projection;
		Vector3 m_CameraPosition;
		int dummy;
	};

	struct CB_DirectionLight
	{
		Vector3 m_Direction = { 0.0f, -1.0f, 1.0f };
		Vector3 m_DircetionColor = { 1.f, 1.f, 1.f };
		Vector2 m_dummy;
	};

	struct CB_Material
	{
		Vector3 m_baseColor = Vector3(1.f, 1.f, 1.f);
		Vector3 m_emissiveColor = Vector3(1.f, 1.f, 1.f);
		float m_OpacityValue = 1.f;
		float m_EmissivePower = 1.f;
	};

	struct CB_UseTextureMap
	{
		int UseDiffuseMap;
		int UseNormalMap;
		int UseSpecularMap;
		int UseOpacityMap;
		int UseEmissiveMap;
		int UseMatalnessMap;
		int UseRoughnessMap;
		int UseCubeMap;
	};

	struct CB_MatrixPalette
	{
		Matrix Array[128];
	};

	// ���� �Ŵ��� Ŭ����
	class RenderManager
	{
		SINGLETON(RenderManager)

	public:
		void Initalize(HWND _hwnd, float _height, float _width, Color _color);
		void Update();
		void Render();

	private:
		void SortSkeletalMeshInstance();
		void renderStaticMeshInstance();

	private:
		void createConstantBuffer();

	public:
		inline shared_ptr<Graphics> GetGraphics();
		inline shared_ptr<PipeLine> GetPipeLine();
		inline shared_ptr<ImGuiTool> GetImGuiTool();
		inline void SetCamera(CB_Camera&& _camera);
		void SetSkeletalMeshInstance(shared_ptr<SkeletalMeshInstance> _meshInstance);

		void SetStaticMeshInstance(shared_ptr<StaticMeshInstance> _meshInstance);

	private:
		shared_ptr<Graphics> m_pGraphics;
		shared_ptr<PipeLine> m_pPipeLine;
		shared_ptr<ImGuiTool> m_pImGuiTool;

		std::unordered_map<Material* ,vector<shared_ptr<SkeletalMeshInstance>>> m_pSkeletalMeshInstanceVec;
		std::unordered_map<Material*, vector<shared_ptr<StaticMeshInstance>>> m_pStaticMeshInstanceVec;

		CB_Camera m_CBCameraData;
		CB_DirectionLight m_CBDirectionLightData;
		CB_UseTextureMap m_CBUseTextureMap;

		ComPtr<ID3D11Buffer> m_pCBMaterial = nullptr;				// ��� ����: ��ȯ���
		ComPtr<ID3D11Buffer> m_pCBModelTransform = nullptr;			// ��� ����: ��ȯ���
		ComPtr<ID3D11Buffer> m_pCBCamera = nullptr;					// ��� ����: ��ȯ���
		ComPtr<ID3D11Buffer> m_pCBDirectionLight = nullptr;			// ��� ����: ���Ɽ
		ComPtr<ID3D11Buffer> m_pCBBoneTransformPallete = nullptr;	// ��� ����: ���Ɽ
		ComPtr<ID3D11Buffer> m_pCBbIsTexture = nullptr;				// ��� ����: ���Ɽ
		//ConstantBuffer<CB_MatrixPalette> m_cbMatrixPallete; // DirectXTK�� ������� Ŭ���� Ȱ��
	};

	// ���� : RenderManager�� Get & Set �Լ�
	shared_ptr<GraphicsEngine::Graphics> RenderManager::GetGraphics()
	{
		return m_pGraphics;
	}
	shared_ptr<GraphicsEngine::PipeLine> RenderManager::GetPipeLine()
	{
		return m_pPipeLine;
	}
	shared_ptr<GraphicsEngine::ImGuiTool> RenderManager::GetImGuiTool()
	{
		return m_pImGuiTool;
	}
	void RenderManager::SetCamera(CB_Camera&& _camera)
	{
		m_CBCameraData = _camera;
	}
}

#define RENDER GraphicsEngine::RenderManager::GetInstance()
#define IMGUI GraphicsEngine::RenderManager::GetInstance()->GetImGuiTool()
#define DEVICE GraphicsEngine::RenderManager::GetInstance()->GetGraphics()->GetDevice()
#define DEVICE_CONTEXT GraphicsEngine::RenderManager::GetInstance()->GetGraphics()->GetDeviceContext()