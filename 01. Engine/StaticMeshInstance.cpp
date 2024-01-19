#include "pch.h"
#include "StaticMeshInstance.h"

#include "Node.h"
#include "StaticMeshResource.h"

namespace Engine
{
	StaticMeshInstance::StaticMeshInstance()
	{
	}

	StaticMeshInstance::~StaticMeshInstance()
	{
	}

	void StaticMeshInstance::Create(StaticMesh* _pMeshResource, Material* _pMaterial, vector<shared_ptr<Node>>& _pNodeVec)
	{
		m_pMeshResource = _pMeshResource;
		m_pMaterial = _pMaterial;

		for (auto node : _pNodeVec)
		{
			for (string name : node->GetNodaData().m_MeshName)
			{
				if (name == _pMeshResource->GetName())
				{
					m_pNode = node.get();
				}
			}
		}
	}
}
