#include "pch.h"
#include "StaticMeshResource.h"

namespace Engine
{
	StaticMeshResource::StaticMeshResource()
	{
	}

	StaticMeshResource::~StaticMeshResource()
	{
	}

	void StaticMeshResource::Create(aiMesh* _mesh)
	{
	}



	StaticMeshSceneResource::StaticMeshSceneResource()
	{
		m_ResourceType = ResourceType::StaticMesh;
	}

	StaticMeshSceneResource::~StaticMeshSceneResource()
	{
	}

	void StaticMeshSceneResource::Create(string _path)
	{
	}

}
