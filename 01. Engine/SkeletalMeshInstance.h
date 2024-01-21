#pragma once

namespace Engine
{
	// ���� : Ŭ���� ����
	class SkeletalMesh;
	class Material;
	class Node;
	struct CB_MatrixPalette;

	// ���� : ���̷�Ż �޽� �ν��Ͻ� Ŭ����
	// ���� : ���̷�Ż �޽� �ν��Ͻ� Ŭ������ ���� �Ŵ����� ���� �����͸� ������ �ֽ��ϴ�.
	// ���� : ���̷�Ż �޽� ������Ʈ���� ���̷�Ż �޽� �ν��Ͻ� Ŭ������ �����ϸ� �ش� ���̷�Ż �޽� ������Ʈ�� ���� �������� �ν��Ͻ��� �����ؼ�
	// ���� : ���� �Ŵ����� �����ָ� ���� �Ŵ������� ������ ó���Ͽ� ������ �մϴ�.
	class SkeletalMeshInstance
	{
	public:
		SkeletalMeshInstance();
		SkeletalMeshInstance(SkeletalMesh* _pMeshResource, vector<shared_ptr<Node>>& _pNodeVec, Material* _pMaterial, Matrix* _pTransform) : m_pMaterial(_pMaterial), m_pMeshResource(_pMeshResource), m_pNodeVec(_pNodeVec) {};
		
		~SkeletalMeshInstance();			 

	public:
		void Create(SkeletalMesh* _pMeshResource, vector<shared_ptr<Node>>& _pNodeVec, Material* _pMaterial);
		void UpdateMatrixPallete(CB_MatrixPalette* pMatrixPallete);

	public:
		inline SkeletalMesh* GetSkeletalMesh();
		inline Material* GetMaterial();
		inline vector<shared_ptr<Node>>& GetNodeVec();

	private:
		SkeletalMesh* m_pMeshResource;
		Material* m_pMaterial;
		vector<shared_ptr<Node>> m_pNodeVec;
	};

	SkeletalMesh* SkeletalMeshInstance::GetSkeletalMesh()
	{
		return m_pMeshResource;
	}
	Material* SkeletalMeshInstance::GetMaterial()
	{
		return m_pMaterial;
	}
	vector<shared_ptr<Node>>& SkeletalMeshInstance::GetNodeVec()
	{
		return m_pNodeVec;
	}
}