#pragma once
#include "ResourceBase.h"

namespace Engine
{
	class SkeletonResource;
	class AnimationResource;
	class AnimationNode;
	class Bone;


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
	class Node : public enable_shared_from_this<Node>
	{
	public:
		Node();
		~Node();

	public:
		void Update(Matrix _modelTransform, float _currentTime);

	private:
		void interpolateAnimationData(float currentTime, Vector3& outPosition, Vector3& outScaling, Quaternion& outRotation);

	public:
		inline const NodeData& GetNodaData();
		inline void SetNodeData(NodeData _nodeData);
		inline shared_ptr<Node> GetParent();
		inline void SetParent(shared_ptr<Node> _node);
		inline const vector<shared_ptr<Node>>& GetNodes();
		inline void SetChildNode(shared_ptr<Node> _node);
		inline shared_ptr<Bone> GetBone();
		inline void SetBone(shared_ptr<Bone> _bone);
		inline shared_ptr<AnimationNode> GetAnimationNode();
		inline void SetAnimationNode(shared_ptr<AnimationNode> _animationNode);
		inline const Matrix& GetLocalTransform();
		inline void SetLocalTransform(Matrix _transform);
		inline const Matrix& GetWorldTransform();

	private:
		NodeData m_NodeData;

		shared_ptr<Node> m_Parent;
		vector<shared_ptr<Node>> m_Children;

		shared_ptr<Bone> m_Bone;
		shared_ptr<AnimationNode> m_AnimationNode;

		Matrix m_Local;
		Matrix m_World;
	};


	const NodeData& Node::GetNodaData()
	{
		return m_NodeData;
	}
	void Node::SetNodeData(NodeData _nodeData)
	{
		m_NodeData = _nodeData;
	}
	shared_ptr<Node> Node::GetParent()
	{
		return m_Parent;
	}
	void Node::SetParent(shared_ptr<Node> _node)
	{
		m_Parent = _node;
		m_Parent->SetChildNode(shared_from_this());
	}
	const vector<shared_ptr<Node>>& Node::GetNodes()
	{
		return m_Children;
	}
	void Node::SetChildNode(shared_ptr<Node> _node)
	{
		m_Children.push_back(_node);
	}
	shared_ptr<Bone> Node::GetBone()
	{
		return m_Bone;
	}
	void Node::SetBone(shared_ptr<Bone> _bone)
	{
		m_Bone = _bone;
	}
	shared_ptr<AnimationNode> Node::GetAnimationNode()
	{
		return m_AnimationNode;
	}
	void Node::SetAnimationNode(shared_ptr<AnimationNode> _animationNode)
	{
		m_AnimationNode = _animationNode;
	}
	const Matrix& Node::GetLocalTransform()
	{
		return m_Local;
	}
	void Node::SetLocalTransform(Matrix _transform)
	{
		m_Local = _transform;
	}
	const Matrix& Node::GetWorldTransform()
	{
		return m_World;
	}

}