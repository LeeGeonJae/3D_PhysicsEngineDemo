#include "pch.h"
#include "StaticMeshComponent.h"

#include "ResourceManager.h"
#include "RenderManager.h"

#include "StaticMeshResource.h"
#include "Shader.h"
#include "StaticMeshInstance.h"

namespace Engine
{
	StaticMeshComponent::StaticMeshComponent()
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

		for (int i = 0; i < m_pStaticMesheScene->GetStaticMeshVec().size(); i++)
		{
			shared_ptr<StaticMeshInstance> meshInstance = make_shared<StaticMeshInstance>();
			meshInstance->Create(&m_pStaticMesheScene->GetStaticMeshVec()[i], m_pStaticMesheScene->GetStaticMeshVec()[i].GetMaterial().get(), &GetWorldTransform());
			m_pStaticMeshInstanceVec.push_back(meshInstance);
		}
	}

	void StaticMeshComponent::Update()
	{
		__super::Update();


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

