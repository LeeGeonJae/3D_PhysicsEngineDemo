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
		virtual void Create(string _path);

	public:
		inline const ResourceType& GetResourceType();
		inline const string& GetPath();
		inline const string& GetName();
		inline void SetName(string _name);

	protected:
		ResourceType m_ResourceType;
		string m_Path;
		string m_Name;
	};



	const ResourceType& ResourceBase::GetResourceType()
	{
		return m_ResourceType;
	}
	const string& ResourceBase::GetPath()
	{
		return m_Path;
	}
	const string& ResourceBase::GetName()
	{
		return m_Name;
	}
	void ResourceBase::SetName(string _name)
	{
		m_Name = _name;
	}
}
