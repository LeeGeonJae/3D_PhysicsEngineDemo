#pragma once

namespace Engine
{
	class Bone;
	class AnimationResource;


	// 노드 데이터 구조체
	struct NodeData
	{
		string m_Name;
		string m_Parent;
	};


	// 노드 구조체
	class Node
	{
	public:
		Node();
		~Node();

	public:
		void Update();

	private:
		NodeData m_NodeData;

		shared_ptr<Node> m_Parent;
		vector<shared_ptr<Node>> m_Children;

		shared_ptr<Bone> m_Bone;
		shared_ptr<AnimationResource> m_AnimationResource;
	};
}