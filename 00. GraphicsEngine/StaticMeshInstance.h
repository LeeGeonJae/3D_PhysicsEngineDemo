#pragma once

namespace GraphicsEngine
{
	// ���� : Ŭ���� ����
	class StaticMesh;
	class Material;
	class Node;

	// ���� : ����ƽ �޽� �ν��Ͻ�
	// ���� : ����ƽ �޽� �ν��Ͻ� Ŭ������ ���� �Ŵ����� ���� �����͸� ������ �ֽ��ϴ�.
	// ���� : ����ƽ �޽� ������Ʈ���� ����ƽ �޽� �ν��Ͻ� Ŭ������ �����ϸ� �ش� ����ƽ �޽� ������Ʈ�� ���� �������� �ν��Ͻ��� �����ؼ�
	// ���� : ���� �Ŵ����� �����ָ� ���� �Ŵ������� ������ ó���Ͽ� ������ �մϴ�.
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

