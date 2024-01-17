#pragma once
#include "RenderComponent.h"

namespace Engine
{
	// 클래스 선언
	class AnimationResource;
	class NodeData;
	class Node;
	class SkeletalMeshSceneResource;
	class SkeletalMesh;
	class SkeletonResource;
	class Material;
	class Shader;

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

		shared_ptr<Node> m_pRootNode;
		vector<shared_ptr<Node>> m_pNodes;
		shared_ptr<SkeletalMeshSceneResource> m_pSkeletalMeshes;
		shared_ptr<Shader> m_pShader;
	};
}


