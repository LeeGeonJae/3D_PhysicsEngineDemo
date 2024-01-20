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

		// ���ҽ��Ŵ������� ���ҽ��� ã��
		m_pSkeletalMeshes = RESOURCE->Find<SkeletalMeshSceneResource>(m_FilePath);
		m_pShader = RESOURCE->Find<Shader>("SkeletalMeshShader");
		shared_ptr<NodeDataResource> nodeDataResource = RESOURCE->Find<NodeDataResource>(m_FilePath);
		shared_ptr<AnimationResource> animationResource = RESOURCE->Find<AnimationResource>(m_FilePath);
		shared_ptr<SkeletonResource> skeletonResource = RESOURCE->Find<SkeletonResource>(m_FilePath);

		// ��� ������ŭ �����ϰ� ��� ����Ÿ ����
		m_pNodeVec.reserve(nodeDataResource->GetNodeDataVec().size());
		for (auto nodeData : nodeDataResource->GetNodeDataVec())
		{
			shared_ptr<Node> node = make_shared<Node>();
			node->SetNodeData(nodeData);
			node->SetLocalTransform(nodeData.m_LocalMatrix);

			m_pNodeVec.push_back(node);
		}
		m_pNodeVec.resize(m_pNodeVec.size());

		// ��� �������ִ� ���� �Լ� ����
		Engine::NodeSetting(animationResource, skeletonResource, m_pNodeVec, m_pRootNode);

		// ���� �Ŵ����� ������ �޽� �ν��Ͻ� ����
		for (int i = 0; i < m_pSkeletalMeshes->GetSkeletalMeshVec().size(); i++)
		{
			shared_ptr<SkeletalMeshInstance> meshInstance = make_shared<SkeletalMeshInstance>();
			meshInstance->Create(&m_pSkeletalMeshes->GetSkeletalMeshVec()[i], m_pNodeVec, m_pSkeletalMeshes->GetSkeletalMeshVec()[i].GetMaterial().get());
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

