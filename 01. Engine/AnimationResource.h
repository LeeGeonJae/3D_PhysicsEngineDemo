#pragma once
#include "ResourceBase.h"

namespace Engine
{
	struct KeyFrameData
	{
		float m_Time;
		Vector3 m_Scale;
		Quaternion m_Rotation;
		Vector3 m_Transtation;
	};


	struct AnimationNode
	{
		string m_Name;
		weak_ptr<AnimationResource> m_Owner;
		vector<KeyFrameData> m_KeyFrame;
	};


	class AnimationResource : public ResourceBase
	{
	public:
		AnimationResource();
		virtual ~AnimationResource();

	public:
		void Create(string _path) override;

	private:
		string m_Name;
		unsigned int m_FrameCount;
		float m_FrameRate;
		float m_Duration;
		vector<AnimationNode> m_Nodes;
	};
}
