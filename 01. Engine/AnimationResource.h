#pragma once
#include "ResourceBase.h"

namespace Engine
{
	// 키프레임 구조체
	struct KeyFrameData
	{
		float m_Time;
		Vector3 m_Scale;
		Quaternion m_Rotation;
		Vector3 m_Transtation;
	};


	// 애니메이션 노드 구조체
	struct AnimationNode
	{
		string m_Name;
		AnimationResource* m_Owner;
		vector<KeyFrameData> m_KeyFrame;
	};


	// 애니메이션 리소스 구조체
	class AnimationResource : public ResourceBase
	{
	public:
		AnimationResource();
		virtual ~AnimationResource();

	public:
		void Create(string _path) override;

	private:
		void processAnimation(aiAnimation* srcAnimation);
		AnimationNode ParseAnimationNode(aiNodeAnim* srcNode);

	private:
		unsigned int m_FrameCount;
		float m_FrameRate;
		float m_Duration;
		unordered_map<string, AnimationNode> m_Nodes;
	};
}
