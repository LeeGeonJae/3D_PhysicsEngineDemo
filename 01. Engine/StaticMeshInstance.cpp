#include "pch.h"
#include "StaticMeshInstance.h"

namespace Engine
{
	StaticMeshInstance::StaticMeshInstance()
	{
	}

	StaticMeshInstance::~StaticMeshInstance()
	{
	}

	void StaticMeshInstance::Create(StaticMesh* _pMeshResource, Material* _pMaterial, Matrix* _pTransform)
	{
		m_pMeshResource = _pMeshResource;
		m_pMaterial = _pMaterial;
		m_pTransform = _pTransform;
	}
}
