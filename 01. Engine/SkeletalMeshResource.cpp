#include "pch.h"
#include "SkeletalMeshResource.h"

namespace Engine
{
	SkeletalMeshResource::SkeletalMeshResource()
	{
		
	}

	SkeletalMeshResource::~SkeletalMeshResource()
	{
	}

	void SkeletalMeshResource::Create(aiMesh* mesh, SkeletonResource* skeleton)
	{
	}



	SkeletalMeshSceneResource::SkeletalMeshSceneResource()
	{
		m_ResourceType = ResourceType::SkeletalMesh;
	}

	SkeletalMeshSceneResource::~SkeletalMeshSceneResource()
	{
	}

	void SkeletalMeshSceneResource::Create(string _path)
	{
	}
}
