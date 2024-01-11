#pragma once

namespace Engine
{
	enum class ResourceType
	{
		None = 1,
		StaticMesh,
		SkeletalMesh,
		Shader,
		Texture,
		Material,
		Animation,
		Bone,

		End
	};

	constexpr int RESOURCE_TYPE_COUNT = static_cast<int>(ResourceType::End);

	class ResourceBase
	{
	public:
		ResourceBase();
		virtual ~ResourceBase();

	public:
		inline const ResourceType& GetResourceType();
		inline const wstring& GetPath();
		inline const wstring& GetName();

	private:
		ResourceType m_ResourceType;
		wstring m_Path;
		wstring m_Name;
	};

	const ResourceType& ResourceBase::GetResourceType()
	{
		return m_ResourceType;
	}
	const wstring& ResourceBase::GetPath()
	{
		return m_Path;
	}
	const wstring& ResourceBase::GetName()
	{
		return m_Name;
	}
}
