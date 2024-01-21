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
		// 건재 : 리소스 매니저에서 aiAnimation를 받아 우리가 사용할 정보의 형태에 맞게 데이터 파싱합니다.
		m_Name = _srcAnimation->mName.C_Str();
		m_FrameRate = static_cast<float>(_srcAnimation->mTicksPerSecond);
		m_FrameCount = static_cast<unsigned int>(_srcAnimation->mDuration + 1);

		for (int i = 0; i < _srcAnimation->mNumChannels; i++)
		{
			aiNodeAnim* node = _srcAnimation->mChannels[i];
			m_pAnimationNodeMap.insert(make_pair(node->mNodeName.C_Str(), ParseAnimationNode(node)));
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

			// 건재 : Position
			{
				aiVectorKey key = srcNode->mPositionKeys[i];
				frameData.m_Time = (float)key.mTime;

				frameData.m_Transtation.x = key.mValue.x;
				frameData.m_Transtation.y = key.mValue.y;
				frameData.m_Transtation.z = key.mValue.z;

				found = true;
			}
			// 건재 : Rotation
			{
				aiQuatKey key = srcNode->mRotationKeys[i];
				frameData.m_Time = (float)key.mTime;

				frameData.m_Rotation.x = key.mValue.x;
				frameData.m_Rotation.y = key.mValue.y;
				frameData.m_Rotation.z = key.mValue.z;
				frameData.m_Rotation.w = key.mValue.w;

				found = true;
			}
			// 건재 : Scale
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

		// 건재 : Keyframe 늘려주기
		if (animationNode->m_KeyFrame.size() < animationNode->m_Owner->GetFrameCount())
		{
			unsigned int count = animationNode->m_Owner->GetFrameCount() - animationNode->m_KeyFrame.size();
			KeyFrameData keyFrame = animationNode->m_KeyFrame.back();

			for (unsigned int i = 0; i < count; i++)
			{
				animationNode->m_KeyFrame.push_back(keyFrame);
			}
		}

		return animationNode;
	}
}
