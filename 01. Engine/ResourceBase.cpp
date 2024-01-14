#include "pch.h"
#include "ResourceBase.h"

namespace Engine
{
	ResourceBase::ResourceBase()
		: m_ResourceType(ResourceType::None)
	{
	}

	ResourceBase::~ResourceBase()
	{
	}

	void ResourceBase::Create(string _path)
	{
	}
}
