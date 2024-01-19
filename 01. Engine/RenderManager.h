#pragma once

#include <dxgi1_4.h>
#include <DirectXtk/BufferHelpers.h>
#include <DirectXCollision.h>

namespace GraphicsEngine
{
	class Graphics;
	class PipeLine;
}

namespace Engine
{
	// 클래스 선언
	class SkeletalMeshInstance;
	class StaticMeshInstance;


	// ConstantBuffer 구조체
	struct CB_ModelTransform
	{
		Matrix m_World;
	};

	struct CB_Camera
	{
		Matrix m_View;
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
		int m_bIsTexture = 0;
		Vector3 m_dummy;
	};

	struct CB_MatrixPalette
	{
		Matrix Array[128];
	};

	struct CB_bIsTexture
	{
		int bIsValidTextureMap[7];
		int dummy;
	};


	// 렌더 매니저 클래스
	class RenderManager
	{
		SINGLETON(RenderManager)

	public:
		void Initalize(HWND _hwnd);
		void Update();
		void Render();

	private:
		void renderSkeletalMeshInstance();
		void renderStaticMeshInstance();
		void constantBufferUpdate(shared_ptr<SkeletalMeshInstance> _meshInstance);

	private:
		void createConstantBuffer();

	public:
		inline shared_ptr<GraphicsEngine::Graphics> GetGraphics();
		inline shared_ptr<GraphicsEngine::PipeLine> GetPipeLine();
		inline void SetSkeletalMeshInstance(shared_ptr<SkeletalMeshInstance> _meshInstance);
		inline void SetStaticMeshInstance(shared_ptr<StaticMeshInstance> _meshInstance);

	private:
		shared_ptr<GraphicsEngine::Graphics> m_Graphics;
		shared_ptr<GraphicsEngine::PipeLine> m_PipeLine;

		vector<shared_ptr<SkeletalMeshInstance>> m_pSkeletalMeshInstanceVec;
		vector<shared_ptr<StaticMeshInstance>> m_pStaticMeshInstanceVec;

		CB_Camera m_CBCameraData;
		CB_DirectionLight m_CBDirectionLightData;
		

		ComPtr<ID3D11Buffer> m_pCBMaterial = nullptr;				// 상수 버퍼: 변환행렬
		ComPtr<ID3D11Buffer> m_pCBModelTransform = nullptr;		// 상수 버퍼: 변환행렬
		ComPtr<ID3D11Buffer> m_pCBCamera = nullptr;				// 상수 버퍼: 변환행렬
		ComPtr<ID3D11Buffer> m_pCBDirectionLight = nullptr;		// 상수 버퍼: 방향광
		ComPtr<ID3D11Buffer> m_pCBBoneTransformPallete = nullptr;	// 상수 버퍼: 방향광
		ComPtr<ID3D11Buffer> m_pCBbIsTexture = nullptr;	// 상수 버퍼: 방향광
		//ConstantBuffer<CB_MatrixPalette> m_cbMatrixPallete; // DirectXTK의 상수버퍼 클래스 활용
	};

	shared_ptr<GraphicsEngine::Graphics> RenderManager::GetGraphics()
	{
		return m_Graphics;
	}
	shared_ptr<GraphicsEngine::PipeLine> RenderManager::GetPipeLine()
	{
		return m_PipeLine;
	}

	void RenderManager::SetSkeletalMeshInstance(shared_ptr<SkeletalMeshInstance> _meshInstance)
	{
		m_pSkeletalMeshInstanceVec.push_back(_meshInstance);
	}
	void RenderManager::SetStaticMeshInstance(shared_ptr<StaticMeshInstance> _meshInstance)
	{
		m_pStaticMeshInstanceVec.push_back(_meshInstance);
	}
}

#define RENDER Engine::RenderManager::GetInstance()
#define DEVICE Engine::RenderManager::GetInstance()->GetGraphics()->GetDevice()
#define DEVICE_CONTEXT Engine::RenderManager::GetInstance()->GetGraphics()->GetDeviceContext()