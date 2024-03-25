#pragma once
#include "RenderComponent.h"

namespace GraphicsEngine
{
	class Node;
	class SkeletalMeshSceneResource;
	class Shader;
	class SkeletalMeshInstance;
	class AnimationResource;
	class SkeletonResource;
}

namespace Engine
{
	// 건재 : 스켈레탈메시 컴포넌트 정보 구조체
	struct SkeletalMeshComponentInfo
	{
		RenderComponentInfo m_RenderComponentInfo;
		string m_FilePath;
	};

	// -------------------------------------------------------------------------
	// 건재 : 스켈레탈 메시 컴포넌트 클래스
	class SkeletalMeshComponent : public RenderComponent
	{
	public:
		SkeletalMeshComponent();
		virtual ~SkeletalMeshComponent();

	public:
		void Setting(SkeletalMeshComponentInfo _info);

	public:
		virtual void Init() override;
		virtual void Update(float _deltaTime) override;
		virtual void Render() override;

	private:
		void NodeSetting(shared_ptr<GraphicsEngine::AnimationResource> _animationResource, shared_ptr<GraphicsEngine::SkeletonResource> _skeletonResource, vector<shared_ptr<GraphicsEngine::Node>>& _nodeVec, shared_ptr<GraphicsEngine::Node>& _rootNode);

	private:
		string m_FilePath;
		float m_CurrentTime = 0.f;

		shared_ptr<GraphicsEngine::Node> m_pRootNode;
		vector<shared_ptr<GraphicsEngine::Node>> m_pNodeVec;
		shared_ptr<GraphicsEngine::SkeletalMeshSceneResource> m_pSkeletalMeshes;
		shared_ptr<GraphicsEngine::Shader> m_pShader;

		vector<shared_ptr<GraphicsEngine::SkeletalMeshInstance>> m_pSkeletalMeshInstanceVec;
	};
	// -------------------------------------------------------------------------
}