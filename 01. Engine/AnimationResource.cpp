#include "pch.h"
#include "AnimationResource.h"

namespace Engine
{
	AnimationResource::AnimationResource()
		:ResourceBase(ResourceType::Animation)
	{
	}

	AnimationResource::~AnimationResource()
	{
	}

	void AnimationResource::Create(aiAnimation* _srcAnimation)
	{
		m_Name = _srcAnimation->mName.C_Str();
		m_FrameRate = static_cast<float>(_srcAnimation->mTicksPerSecond);
		m_FrameCount = static_cast<unsigned int>(_srcAnimation->mDuration + 1);

		for (int i = 0; i < _srcAnimation->mNumChannels; i++)
		{
			aiNodeAnim* node = _srcAnimation->mChannels[i];
			m_Nodes.insert(make_pair(node->mNodeName.C_Str(), ParseAnimationNode(node)));
		}
	}

	shared_ptr<AnimationNode> AnimationResource::ParseAnimationNode(aiNodeAnim* srcNode)
	{
		shared_ptr<AnimationNode> animationNode = make_shared<AnimationNode>();

		animationNode->m_Owner = this;
		animationNode->m_Name = srcNode->mNodeName.C_Str();

		int KeyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumScalingKeys), srcNode->mNumRotationKeys);
		animationNode->m_KeyFrame.reserve(KeyCount);

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
				animationNode->m_KeyFrame.push_back(frameData);
		}

		return animationNode;
	}
}
