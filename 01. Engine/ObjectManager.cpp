#include "pch.h"
#include "ObjectManager.h"

#include "Object.h"

namespace Engine
{
	void ObjectManager::Update()
	{
		while (!m_DeleteExpectedObjectQueue.empty())
		{
			m_ObjectMap.erase(m_DeleteExpectedObjectQueue.front());
			m_DeleteExpectedObjectQueue.pop();
		}

		for (const auto& object : m_ObjectMap)
		{
			if (object.second->GetIsDead())
			{
				m_DeleteExpectedObjectQueue.push(object.first);
			}
		}
	}
}