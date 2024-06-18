#pragma once
#include "ResourceBase.h"

namespace GraphicsEngine
{
	// ���� : Ŭ���� ����
	class AnimationResource;

	// ���� : Ű������ ����ü
	struct KeyFrameData
	{
		float m_Time;
		Vector3 m_Scale;
		Quaternion m_Rotation;
		Vector3 m_Transtation;
	};


	// ���� : �ִϸ��̼� ��� ����ü
	struct AnimationNode
	{
		string m_Name;
		AnimationResource* m_Owner;
		vector<KeyFrameData> m_KeyFrame;
	};


	// -------------------------------------------------------------------------
	// ���� : �ִϸ��̼� ���ҽ� Ŭ����
	class AnimationResource : public ResourceBase
	{
	public:
		AnimationResource();
		virtual ~AnimationResource();

	public:
		void Create(aiAnimation* _srcAnimation);

	private:
		shared_ptr<AnimationNode> ParseAnimationNode(aiNodeAnim* srcNode);

	public:
		inline unsigned int GetFrameCount();
		inline float GetFrameRate();
		inline float GetDuration();
		inline shared_ptr<AnimationNode> FindAnimationNode(const string& _key);

	private:
		unsigned int m_FrameCount;
		float m_FrameRate;
		float m_Duration;
		unordered_map<string, shared_ptr<AnimationNode>> m_pAnimationNodeMap;
	};

	// ���� : AnimationResource�� Get & Set �Լ�
	unsigned int AnimationResource::GetFrameCount()
	{
		return m_FrameCount;
	}
	float AnimationResource::GetFrameRate()
	{
		return m_FrameRate;
	}
	float AnimationResource::GetDuration()
	{
		return m_Duration;
	}
	shared_ptr<AnimationNode> AnimationResource::FindAnimationNode(const string& _key)
	{
		if (m_pAnimationNodeMap.find(_key) != m_pAnimationNodeMap.end())
			return m_pAnimationNodeMap.find(_key)->second;

		return nullptr;
	}
	// -------------------------------------------------------------------------
}