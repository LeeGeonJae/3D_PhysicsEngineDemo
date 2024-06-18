#include "pch.h"
#include "SkeletalMeshInstance.h"

#include "RenderManager.h"

#include "Node.h"
#include "SkeletalMeshResource.h"
#include "SkeletonResource.h"
#include "Material.h"


namespace GraphicsEngine
{
	SkeletalMeshInstance::SkeletalMeshInstance()
	{
	}

	SkeletalMeshInstance::~SkeletalMeshInstance()
	{
	}

	void SkeletalMeshInstance::Create(SkeletalMesh* _pMeshResource, vector<shared_ptr<Node>>& _pNodeVec, Material* _pMaterial)
	{
		m_pMaterial = _pMaterial;
		m_pMeshResource = _pMeshResource;
		m_pNodeVec = _pNodeVec;
	}

	void SkeletalMeshInstance::UpdateMatrixPallete(CB_MatrixPalette* pMatrixPallete)
	{
		// 건재 : 본 팔레트 상수 버퍼의 데이터를 세팅해주는 함수입니다.
		int Count = 0;

		for (int i = 0; i < m_pNodeVec.size(); i++)
		{
			if (m_pNodeVec[i]->GetBone() != nullptr)
			{
				const Matrix& nodeWorldTransform = m_pNodeVec[i]->GetWorldTransform().Transpose();
				const Matrix& boneOffsetMatrix = m_pNodeVec[i]->GetBone()->m_OffsetMatrix.Transpose();
				pMatrixPallete->Array[Count] = (nodeWorldTransform * boneOffsetMatrix);
				Count++;
			}
		}

		int a = 0;
	}
}

