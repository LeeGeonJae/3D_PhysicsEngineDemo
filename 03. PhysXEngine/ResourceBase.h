#pragma once
#include <string>

namespace physics
{
	// ���� : ���ҽ� Ÿ�� ������
	enum class EResourceType
	{
		MATERIAL,
		CONVEX_MESH,
		TRIANGLE_MESH,
		HEIGHT_FIELD_MESH,

		END
	};

	// ���� : ���ҽ� Ÿ���� ����
	enum
	{
		RESOURCE_TYPE_COUNT = static_cast<unsigned int>(EResourceType::END)
	};

	class ResourceBase
	{
	public:
		ResourceBase(const EResourceType& type);
		virtual ~ResourceBase();

		inline const EResourceType& GetResourceType();

	private:
		EResourceType mType;
	};

	const EResourceType& ResourceBase::GetResourceType()
	{
		return mType;
	}
}