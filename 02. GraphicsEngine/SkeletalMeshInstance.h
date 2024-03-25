#pragma once

namespace GraphicsEngine
{
	// 건재 : 클래스 선언
	class SkeletalMesh;
	class Material;
	class Node;
	struct CB_MatrixPalette;

	// -------------------------------------------------------------------------
	// 건재 : 스켈레탈 메시 인스턴스 클래스
	// 건재 : 스켈레탈 메시 인스턴스 클래스로 렌더 매니저에 보낼 데이터를 가지고 있습니다.
	// 건재 : 스켈레탈 메시 컴포넌트에서 스켈레탈 메시 인스턴스 클래스를 생성하면 해당 스켈레탈 메시 컴포넌트의 렌더 정보들을 인스턴스에 저장해서
	// 건재 : 렌더 매니저에 보내주면 렌더 매니저에서 정보를 처리하여 렌더링 합니다.
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
	// -------------------------------------------------------------------------
}