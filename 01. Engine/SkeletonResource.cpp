#include "pch.h"
#include "SkeletonResource.h"

#include "SkeletalMeshResource.h"
#include "Helper.h"

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

		for (int i = 0; i < m_BoneVec.size(); i++)
		{
			BoneMapping.insert(make_pair(m_BoneVec[i]->m_NodeName, i));
		}

		for (UINT i = 0; i < meshBoneCount; i++)
		{
			aiBone* aibone = _aiMesh->mBones[i];
			UINT boneIndex = 0;

			if (BoneMapping.find(aibone->mName.C_Str()) == BoneMapping.end())
			{
				shared_ptr<Bone> bone = make_shared<Bone>();

				boneIndex = i;
				boneIndexCounter++;
				bone->m_NodeName = aibone->mName.C_Str();
				Engine::aiMatrixToMatrix(aibone->mOffsetMatrix, bone->m_OffsetMatrix);

				m_BoneMap.insert(make_pair(bone->m_NodeName, bone));
				m_BoneVec.push_back(bone);
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
