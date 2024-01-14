#include "pch.h"
#include "Material.h"

namespace Engine
{
	MaterialResource::MaterialResource()
	{
		m_ResourceType = ResourceType::Material;
	}

	MaterialResource::~MaterialResource()
	{
	}

	void MaterialResource::Create(const aiMaterial* _path)
	{
	}
}
