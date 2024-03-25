#pragma once

namespace GraphicsEngine
{
	// 건재 : 클래스 선언
	class StaticMesh;
	class Material;
	class Node;

	// 건재 : 스태틱 메시 인스턴스
	// 건재 : 스태틱 메시 인스턴스 클래스로 렌더 매니저에 보낼 데이터를 가지고 있습니다.
	// 건재 : 스태틱 메시 컴포넌트에서 스태틱 메시 인스턴스 클래스를 생성하면 해당 스태틱 메시 컴포넌트의 렌더 정보들을 인스턴스에 저장해서
	// 건재 : 렌더 매니저에 보내주면 렌더 매니저에서 정보를 처리하여 렌더링 합니다.
	class StaticMeshInstance
	{
	public:
		StaticMeshInstance();
		~StaticMeshInstance();

	public:
		void Create(StaticMesh* _pMeshResource, Material* _pMaterial, vector<shared_ptr<Node>>& _pNodeVec);

	public:
		inline StaticMesh* GetStaticMesh();
		inline Material* GetMaterial();
		inline Node* GetNode();

	private:
		StaticMesh* m_pMeshResource;
		Material* m_pMaterial;
		Node* m_pNode;
	};

	StaticMesh* StaticMeshInstance::GetStaticMesh()
	{
		return m_pMeshResource;
	}
	Material* StaticMeshInstance::GetMaterial()
	{
		return m_pMaterial;
	}
	Node* StaticMeshInstance::GetNode()
	{
		return m_pNode;
	}
}

