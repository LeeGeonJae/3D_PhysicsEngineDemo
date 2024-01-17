#include "pch.h"
#include "SkeletonResource.h"

#include "SkeletalMeshResource.h"

namespace Engine
{
	SkeletonResource::SkeletonResource()
		:ResourceBase(ResourceType::Bone)
	{
	}

	SkeletonResource::~SkeletonResource()
	{
	}

	void SkeletonResource::Create(aiMesh* _aiMesh, vector<BoneWeightVertex>* _vertices)
	{
		UINT meshBoneCount = _aiMesh->mNumBones;
		UINT boneIndexCounter = 0;
		map<string, int> BoneMapping;

		for (UINT i = 0; i < meshBoneCount; i++)
		{
			aiBone* aibone = _aiMesh->mBones[i];
			UINT boneIndex = 0;

			if (BoneMapping.find(aibone->mName.C_Str()) == BoneMapping.end())
			{
				shared_ptr<Bone> bone = make_shared<Bone>();

				boneIndex = boneIndexCounter;
				boneIndexCounter++;
				bone->m_NodeName = aibone->mName.C_Str();
				
				bone->m_OffsetMatrix = DirectX::SimpleMath::Matrix(
					aibone->mOffsetMatrix.a1, aibone->mOffsetMatrix.b1, aibone->mOffsetMatrix.c1, aibone->mOffsetMatrix.d1,
					aibone->mOffsetMatrix.a2, aibone->mOffsetMatrix.b2, aibone->mOffsetMatrix.c2, aibone->mOffsetMatrix.d2,
					aibone->mOffsetMatrix.a3, aibone->mOffsetMatrix.b3, aibone->mOffsetMatrix.c3, aibone->mOffsetMatrix.d3,
					aibone->mOffsetMatrix.a4, aibone->mOffsetMatrix.b4, aibone->mOffsetMatrix.c4, aibone->mOffsetMatrix.d4
				);

				m_BoneMap.insert(make_pair(bone->m_NodeName, bone));
			}
			else
			{
				boneIndex = BoneMapping[aibone->mName.C_Str()];
			}

			for (UINT j = 0; j < aibone->mNumWeights; j++)
			{
				UINT vertexID = aibone->mWeights[j].mVertexId;
				float weight = aibone->mWeights[j].mWeight;

				(*_vertices)[vertexID].AddBoneData(boneIndex, weight);
			}
		}
	}
}
