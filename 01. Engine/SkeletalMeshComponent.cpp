#include "pch.h"
#include "SkeletalMeshComponent.h"
#include "Helper.h"

#include "ResourceManager.h"
#include "RenderManager.h"
#include "TimeManager.h"

#include "AnimationResource.h"
#include "SkeletalMeshResource.h"
#include "SkeletonResource.h"
#include "Node.h"
#include "Shader.h"

#include "SkeletalMeshInstance.h"

namespace Engine
{
	SkeletalMeshComponent::SkeletalMeshComponent()
	{
	}

	SkeletalMeshComponent::~SkeletalMeshComponent()
	{
	}

	void SkeletalMeshComponent::Setting(SkeletalMeshComponentInfo _info)
	{
		__super::Setting(_info.m_RenderComponentInfo);

		m_FilePath = _info.m_FilePath;
	}

	void SkeletalMeshComponent::Init()
	{
		__super::Init();

		// 리소스매니저에서 리소스들 찾기
		m_pSkeletalMeshes = RESOURCE->Find<SkeletalMeshSceneResource>(m_FilePath);
		m_pShader = RESOURCE->Find<Shader>("SkeletalMeshShader");
		shared_ptr<NodeDataResource> nodeDataResource = RESOURCE->Find<NodeDataResource>(m_FilePath);
		shared_ptr<AnimationResource> animationResource = RESOURCE->Find<AnimationResource>(m_FilePath);
		shared_ptr<SkeletonResource> skeletonResource = RESOURCE->Find<SkeletonResource>(m_FilePath);

		// 노드 갯수만큼 생성하고 노드 데이타 세팅
		m_pNodeVec.reserve(nodeDataResource->GetNodeDataVec().size());
		for (auto nodeData : nodeDataResource->GetNodeDataVec())
		{
			m_pNodeVec.emplace_back(make_shared<Node>(nodeData));
		}
		m_pNodeVec.resize(m_pNodeVec.size());

		// 노드 세팅해주는 전역 함수 실행
		Engine::NodeSetting(animationResource, skeletonResource, m_pNodeVec, m_pRootNode);

		// 렌더 매니저에 보내줄 메시 인스턴스 생성
		for (int i = 0; i < m_pSkeletalMeshes->GetSkeletalMeshVec().size(); i++)
		{
			m_pSkeletalMeshInstanceVec.emplace_back(make_shared<SkeletalMeshInstance>( & m_pSkeletalMeshes->GetSkeletalMeshVec()[i], m_pNodeVec, m_pSkeletalMeshes->GetSkeletalMeshVec()[i].GetMaterial().get(), &GetWorldTransform()));
		}

		for (auto meshInstance : m_pSkeletalMeshInstanceVec)
		{
			RENDER->SetSkeletalMeshInstance(meshInstance);
		}
	}

	void SkeletalMeshComponent::Update()
	{
		__super::Update();

		m_CurrentTime += TimeManager::GetInstance()->GetfDT();
		m_pRootNode->Update(GetWorldTransform(), m_CurrentTime * 30.f);
	}

	void SkeletalMeshComponent::Render()
	{
		__super::Render();
	}
}

