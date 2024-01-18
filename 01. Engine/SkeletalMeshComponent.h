#pragma once
#include "RenderComponent.h"

namespace Engine
{
	// 클래스 선언
	class Node;
	class SkeletalMeshSceneResource;
	class Shader;
	class SkeletalMeshInstance;

	// 스켈레탈메시 컴포넌트 정보 구조체
	struct SkeletalMeshComponentInfo
	{
		RenderComponentInfo m_RenderComponentInfo;
		string m_FilePath;
	};

	// 스켈레탈 메시 컴포넌트 클래스
	class SkeletalMeshComponent : public RenderComponent
	{
	public:
		SkeletalMeshComponent();
		virtual ~SkeletalMeshComponent();

	public:
		void Setting(SkeletalMeshComponentInfo _info);

	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;

	private:
		string m_FilePath;
		float m_CurrentTime = 0.f;

		shared_ptr<Node> m_pRootNode;
		vector<shared_ptr<Node>> m_pNodeVec;
		shared_ptr<SkeletalMeshSceneResource> m_pSkeletalMeshes;
		shared_ptr<Shader> m_pShader;

		vector<shared_ptr<SkeletalMeshInstance>> m_pSkeletalMeshInstanceVec;
	};
}