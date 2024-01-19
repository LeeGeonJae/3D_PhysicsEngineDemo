#include "pch.h"
#include "StaticMeshComponent.h"

#include "ResourceManager.h"
#include "RenderManager.h"
#include "TimeManager.h"

#include "StaticMeshResource.h"
#include "AnimationResource.h"
#include "Shader.h"
#include "Node.h"
#include "StaticMeshInstance.h"

#include "Helper.h"

namespace Engine
{
	StaticMeshComponent::StaticMeshComponent()
		: m_FilePath()
		, m_CurrentTime(0.f)
	{
	}
	StaticMeshComponent::~StaticMeshComponent()
	{
	}
	void StaticMeshComponent::Setting(StaticMeshComponentInfo _info)
	{
		__super::Setting(_info.m_RenderComponentInfo);

		m_FilePath = _info.m_FilePath;
	}

	void StaticMeshComponent::Init()
	{
		__super::Init();

		// 리소스매니저에서 리소스들 찾기
		m_pStaticMesheScene = RESOURCE->Find<StaticMeshSceneResource>(m_FilePath);
		m_pShader = RESOURCE->Find<Shader>("StaticMeshShader");
		shared_ptr<NodeDataResource> nodeDataResource = RESOURCE->Find<NodeDataResource>(m_FilePath);
		shared_ptr<AnimationResource> animationResource = RESOURCE->Find<AnimationResource>(m_FilePath);

		// 노드 갯수만큼 생성하고 노드 데이타 세팅
		m_pNodeVec.reserve(nodeDataResource->GetNodeDataVec().size());
		for (auto nodeData : nodeDataResource->GetNodeDataVec())
		{
			shared_ptr<Node> node = make_shared<Node>();
			node->SetNodeData(nodeData);
			node->SetLocalTransform(nodeData.m_LocalMatrix);

			m_pNodeVec.push_back(node);
		}
		NodeSetting(animationResource, m_pNodeVec, m_pRootNode);

		for (int i = 0; i < m_pStaticMesheScene->GetStaticMeshVec().size(); i++)
		{
			shared_ptr<StaticMeshInstance> meshInstance = make_shared<StaticMeshInstance>();
			meshInstance->Create(&m_pStaticMesheScene->GetStaticMeshVec()[i], m_pStaticMesheScene->GetStaticMeshVec()[i].GetMaterial().get(), m_pNodeVec);
			m_pStaticMeshInstanceVec.push_back(meshInstance);
		}
	}

	void StaticMeshComponent::Update()
	{
		__super::Update();

		m_CurrentTime += TimeManager::GetInstance()->GetfDT();
		m_pRootNode->Update(GetWorldTransform(), m_CurrentTime);
	}

	void StaticMeshComponent::Render()
	{
		__super::Render();

		for (auto meshInstance : m_pStaticMeshInstanceVec)
		{
			RENDER->SetStaticMeshInstance(meshInstance);
		}
	}
}

