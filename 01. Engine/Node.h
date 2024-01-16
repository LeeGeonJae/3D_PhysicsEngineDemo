#pragma once
#include "ResourceBase.h"

namespace Engine
{
	class SkeletonResource;
	class AnimationResource;


	// 노드 데이터 구조체
	struct NodeData
	{
		string m_Name;
		string m_ParentName;
	};

	class NodeDataResource : public ResourceBase
	{
	public:
		NodeDataResource();
		virtual ~NodeDataResource();

	public:
		inline const vector<NodeData>& GetNodeDataVec();
		inline void SetNodeDataVec(vector<NodeData> _nodeDataVec);
		inline void AddNodeData(NodeData _nodeData);

	private:
		vector<NodeData> m_NodeData;
	};

	const vector<NodeData>& NodeDataResource::GetNodeDataVec()
	{
		return m_NodeData;
	}
	void NodeDataResource::SetNodeDataVec(vector<NodeData> _nodeDataVec)
	{
		m_NodeData = _nodeDataVec;
	}
	void NodeDataResource::AddNodeData(NodeData _nodeData)
	{
		m_NodeData.push_back(_nodeData);
	}


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

		shared_ptr<SkeletonResource> m_Bone;
		shared_ptr<AnimationResource> m_AnimationResource;
	};
}