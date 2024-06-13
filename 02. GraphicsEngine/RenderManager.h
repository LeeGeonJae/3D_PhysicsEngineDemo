#pragma once

#include <dxgi1_4.h>
#include <DirectXtk/BufferHelpers.h>
#include <DirectXCollision.h>

namespace GraphicsEngine
{
	// 건재 : 클래스 선언
	class Graphics;
	class PipeLine;
	class SkeletalMeshInstance;
	class StaticMeshInstance;
	class Texture;
	class Material;
	class ImGuiTool;

	// 건재 : ConstantBuffer 구조체
	struct CB_ModelTransform
	{
		// 건재 : 스태틱 메시 트랜스폼
		Matrix m_World;
	};

	struct CB_Camera
	{
		Matrix m_View;
		// 잘 바뀌지 않는 데이터는 따로 상수버퍼를 만들어 빼도록 한다!
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

	struct DebugLineData
	{
		Vector3 Pos0;
		Vector3 Pos1;
		DirectX::SimpleMath::Color Color;
	};

	struct PhysicsVertex
	{
		Vector3 position;
		Vector2 uv;
		Vector3 normal;
	};

	// 렌더 매니저 클래스
	class RenderManager
	{
		SINGLETON(RenderManager)

	public:
		void Initalize(HWND _hwnd, float _height, float _width, DirectX::SimpleMath::Color _color);
		void Update();
		void Render();
		void RenderDebug();

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

		//?   Util     ///
		void GetVideoMemoryInfo(std::string& out);							// 비디오 메모리 데이터 구하기
		void GetSystemMemoryInfo(std::string& out);							// 시스템 메모리 데이터 구하기

	public:
		inline void AddDebugBox(shared_ptr<DirectX::BoundingOrientedBox> _boundingBox);
		inline void AddDebugSphere(shared_ptr<DirectX::BoundingSphere> _boundingSphere);
		inline void AddDebugLine(shared_ptr<DebugLineData> _line);
		inline void AddDebugPoint(shared_ptr<Vector3> _point);
		inline void AddDebugPolygon(vector<Vector3> _triangle);

		void SetPhysicsBuffer(vector<PhysicsVertex>& vertex, vector<unsigned int>& index);
		void RenderPhysics();

	private:
		shared_ptr<Graphics> m_pGraphics;
		shared_ptr<PipeLine> m_pPipeLine;
		shared_ptr<ImGuiTool> m_pImGuiTool;

		ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI팩토리
		ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// 비디오카드 정보에 접근 가능한 인터페이스

		std::unordered_map<Material* ,vector<shared_ptr<SkeletalMeshInstance>>> m_pSkeletalMeshInstanceVec;
		std::unordered_map<Material*, vector<shared_ptr<StaticMeshInstance>>> m_pStaticMeshInstanceVec;

		CB_Camera m_CBCameraData;
		CB_DirectionLight m_CBDirectionLightData;
		CB_UseTextureMap m_CBUseTextureMap;

		ComPtr<ID3D11Buffer> m_pCBMaterial = nullptr;				// 상수 버퍼: 변환행렬
		ComPtr<ID3D11Buffer> m_pCBModelTransform = nullptr;			// 상수 버퍼: 변환행렬
		ComPtr<ID3D11Buffer> m_pCBCamera = nullptr;					// 상수 버퍼: 변환행렬
		ComPtr<ID3D11Buffer> m_pCBDirectionLight = nullptr;			// 상수 버퍼: 방향광
		ComPtr<ID3D11Buffer> m_pCBBoneTransformPallete = nullptr;	// 상수 버퍼: 방향광
		ComPtr<ID3D11Buffer> m_pCBbIsTexture = nullptr;				// 상수 버퍼: 방향광
		//ConstantBuffer<CB_MatrixPalette> m_cbMatrixPallete; // DirectXTK의 상수버퍼 클래스 활용

	private:
		vector<shared_ptr<DirectX::BoundingOrientedBox>> m_DebugBoxes;
		vector<shared_ptr<DirectX::BoundingSphere>> m_DebugSpheres;
		vector<shared_ptr<DebugLineData>> m_DebugLines;
		vector<vector<Vector3>> m_DebugPolygon;
		vector<shared_ptr<Vector3>> m_DebugPoints;

		vector<PhysicsVertex> m_PhysicsVertex;
		vector<unsigned int> m_PhysicsIndex;
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		ComPtr<ID3D11Buffer> m_IndexBuffer;
	};

	// 건재 : RenderManager의 Get & Set 함수
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
	void RenderManager::AddDebugBox(shared_ptr<DirectX::BoundingOrientedBox> _boundingBox)
	{
		m_DebugBoxes.push_back(_boundingBox);
	}
	void RenderManager::AddDebugSphere(shared_ptr<DirectX::BoundingSphere> _boundingSphere)
	{
		m_DebugSpheres.push_back(_boundingSphere);
	}
	void RenderManager::AddDebugLine(shared_ptr<DebugLineData> _line)
	{
		m_DebugLines.push_back(_line);
	}
	void RenderManager::AddDebugPoint(shared_ptr<Vector3> _point)
	{
		m_DebugPoints.push_back(_point);
	}
	inline void RenderManager::AddDebugPolygon(vector<Vector3> _triangle)
	{
		m_DebugPolygon.push_back(_triangle);
	}
}

#define RENDER GraphicsEngine::RenderManager::GetInstance()
#define IMGUI GraphicsEngine::RenderManager::GetInstance()->GetImGuiTool()
#define DEVICE GraphicsEngine::RenderManager::GetInstance()->GetGraphics()->GetDevice()
#define DEVICE_CONTEXT GraphicsEngine::RenderManager::GetInstance()->GetGraphics()->GetDeviceContext()