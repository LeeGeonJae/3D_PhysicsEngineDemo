#pragma once

namespace Engine
{
	class Bone;
	class AnimationResource;



	struct NodeData
	{
		string m_Name;
		string m_Parent;
	};



	class Node
	{
	public:
		Node();
		~Node();

	public:


	private:
		NodeData m_NodeData;

		shared_ptr<Node> m_Parent;
		vector<shared_ptr<Node>> m_Children;

		shared_ptr<Bone> m_Bone;
		shared_ptr<AnimationResource> m_AnimationResource;
	};
}