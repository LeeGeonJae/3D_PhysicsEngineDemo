#include "pch.h"
#include "AnimationResource.h"

namespace Engine
{
	AnimationResource::AnimationResource()
	{
		m_ResourceType = ResourceType::Animation;
	}

	AnimationResource::~AnimationResource()
	{
	}

	void AnimationResource::Create(string _path)
	{
		Assimp::Importer importer;

		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ 노드 생성안함
		unsigned int importFlags = aiProcess_ConvertToLeftHanded;	// 왼손 좌표계로 변환
		const aiScene* pScene = importer.ReadFile(_path, importFlags);

		if (pScene->HasAnimations())
		{
			processAnimation(*pScene->mAnimations);
		}
	} 

	void AnimationResource::processAnimation(aiAnimation* srcAnimation)
	{
		m_Name = srcAnimation->mName.C_Str();
		m_FrameRate = static_cast<float>(srcAnimation->mTicksPerSecond);
		m_FrameCount = static_cast<unsigned int>(srcAnimation->mDuration + 1);

		for (int i = 0; i < srcAnimation->mNumChannels; i++)
		{
			aiNodeAnim* node = srcAnimation->mChannels[i];
			m_Nodes.push_back(ParseAnimationNode(node));
		}
	}

	AnimationNode AnimationResource::ParseAnimationNode(aiNodeAnim* srcNode)
	{
		AnimationNode animationNode;

		animationNode.m_Owner = this;
		animationNode.m_Name = srcNode->mNodeName.C_Str();

		int KeyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumScalingKeys), srcNode->mNumRotationKeys);
		animationNode.m_KeyFrame.reserve(KeyCount);

		for (int i = 0; i < KeyCount; i++)
		{
			KeyFrameData frameData;

			bool found = false;

			// Position
			{
				aiVectorKey key = srcNode->mPositionKeys[i];
				frameData.m_Time = (float)key.mTime;

				frameData.m_Transtation.x = key.mValue.x;
				frameData.m_Transtation.y = key.mValue.y;
				frameData.m_Transtation.z = key.mValue.z;

				found = true;
			}
			// Rotation
			{
				aiQuatKey key = srcNode->mRotationKeys[i];
				frameData.m_Time = (float)key.mTime;

				frameData.m_Rotation.x = key.mValue.x;
				frameData.m_Rotation.y = key.mValue.y;
				frameData.m_Rotation.z = key.mValue.z;
				frameData.m_Rotation.w = key.mValue.w;

				found = true;
			}
			// Scale
			{
				aiVectorKey key = srcNode->mScalingKeys[i];
				frameData.m_Time = (float)key.mTime;

				frameData.m_Scale.x = key.mValue.x;
				frameData.m_Scale.y = key.mValue.y;
				frameData.m_Scale.z = key.mValue.z;

				found = true;
			}

			if (found == true)
				animationNode.m_KeyFrame.push_back(frameData);
		}

		return animationNode;
	}
}
