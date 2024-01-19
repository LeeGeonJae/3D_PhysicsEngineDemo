#pragma once

namespace Engine
{
	class AnimationResource;
	class SkeletonResource;
	class Node;

	void NodeSetting(shared_ptr<AnimationResource> _animationResource, shared_ptr<SkeletonResource> _skeletonResource, vector<shared_ptr<Node>>& _nodeVec, shared_ptr<Node>& _rootNode);
	void NodeSetting(shared_ptr<AnimationResource> _animationResource, vector<shared_ptr<Node>>& _nodeVec, shared_ptr<Node>& _rootNode);
	void aiMatrixToMatrix(const aiMatrix4x4& _aiMatrix, Matrix& _matrix);
}
