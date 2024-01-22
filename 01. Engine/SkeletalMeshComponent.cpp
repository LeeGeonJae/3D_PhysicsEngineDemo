#include "pch.h"
#include "SkeletalMeshComponent.h"
#include "Helper.h"

#include "ResourceManager.h"
#include "RenderManager.h"

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
		// 건재 : 정보 구조체에 따라 멤버 함수 세팅
		__super::Setting(_info.m_RenderComponentInfo);
		m_FilePath = _info.m_FilePath;
	}

	void SkeletalMeshComponent::Init()
	{
		__super::Init();

		// 건재 : 리소스매니저에서 리소스들 찾기
		m_pSkeletalMeshes = RESOURCE->Find<SkeletalMeshSceneResource>(m_FilePath);
		m_pShader = RESOURCE->Find<Shader>("SkeletalMeshShader");
		shared_ptr<NodeDataResource> nodeDataResource = RESOURCE->Find<NodeDataResource>(m_FilePath);
		shared_ptr<AnimationResource> animationResource = RESOURCE->Find<AnimationResource>(m_FilePath);
		shared_ptr<SkeletonResource> skeletonResource = RESOURCE->Find<SkeletonResource>(m_FilePath);

		// 건재 : 노드 갯수만큼 생성하고 노드 데이타 세팅
		m_pNodeVec.reserve(nodeDataResource->GetNodeDataVec().size());
		for (auto nodeData : nodeDataResource->GetNodeDataVec())
		{
			shared_ptr<Node> node = make_shared<Node>();
			node->SetNodeData(nodeData);
			node->SetLocalTransform(nodeData.m_LocalMatrix);

			m_pNodeVec.push_back(node);
		}
		m_pNodeVec.resize(m_pNodeVec.size());

		// 건재 : 노드 세팅해주는 함수 실행
		NodeSetting(animationResource, skeletonResource, m_pNodeVec, m_pRootNode);

		// 건재 : 렌더 매니저에 보내줄 메시 인스턴스 생성
		for (int i = 0; i < m_pSkeletalMeshes->GetSkeletalMeshVec().size(); i++)
		{
			shared_ptr<SkeletalMeshInstance> meshInstance = make_shared<SkeletalMeshInstance>();
			meshInstance->Create(&m_pSkeletalMeshes->GetSkeletalMeshVec()[i], m_pNodeVec, m_pSkeletalMeshes->GetSkeletalMeshVec()[i].GetMaterial().get());
			RENDER->SetSkeletalMeshInstance(meshInstance);
		}
	}

	void SkeletalMeshComponent::Update(float _deltaTime)
	{
		__super::Update(_deltaTime);

		m_CurrentTime += _deltaTime;
		m_pRootNode->Update(GetWorldTransform(), m_CurrentTime * 30.f);
	}

	void SkeletalMeshComponent::Render()
	{
		__super::Render();
	}

	void SkeletalMeshComponent::NodeSetting(shared_ptr<AnimationResource> _animationResource, shared_ptr<SkeletonResource> _skeletonResource, vector<shared_ptr<Node>>& _nodeVec, shared_ptr<Node>& _rootNode)
	{
		// 노드에 애니메이션노드, 본, 부모 세팅
		for (auto currentNode : _nodeVec)
		{
			// 애니메이션 및 본 노드에 세팅
			shared_ptr<AnimationNode> animationNode = _animationResource->FindAnimationNode(currentNode->GetNodaData().m_Name);
			shared_ptr<Bone> bone = _skeletonResource->FindBone(currentNode->GetNodaData().m_Name);

			if (animationNode != nullptr)
				currentNode->SetAnimationNode(animationNode);
			if (bone != nullptr)
				currentNode->SetBone(bone);

			// 노드 계층구조 세팅
			for (auto parentNode : _nodeVec)
			{
				if (currentNode->GetNodaData().m_ParentName.empty())
				{
					_rootNode = currentNode;
				}

				if (parentNode->GetNodaData().m_Name == currentNode->GetNodaData().m_ParentName)
				{
					currentNode->SetParent(parentNode);
					break;
				}
			}
		}
	}
}

