#include "pch.h"
#include "ResourceBase.h"

namespace GraphicsEngine
{
	ResourceBase::ResourceBase(ResourceType _type)
		: m_ResourceType(_type)
	{
	}

	ResourceBase::~ResourceBase()
	{
	}
}
