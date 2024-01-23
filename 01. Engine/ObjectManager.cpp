#include "pch.h"
#include "ObjectManager.h"

#include "Object.h"

namespace Engine
{
	void ObjectManager::Update()
	{
		// 건재 : Update할 때 bIsDead 죽을 녀석인지 1프레임 이후에 처리
		while (!m_DeleteExpectedObjectQueue.empty())
		{
			m_ObjectMap.erase(m_DeleteExpectedObjectQueue.front());
			m_DeleteExpectedObjectQueue.pop();
		}
		// 건재 : Update할 때 오브젝트들을 순회하면서 bIsDead 죽을 놈인지 확인 한 후에 queue에 입력
		for (const auto& object : m_ObjectMap)
		{
			if (object.second->GetIsDead())
			{
				m_DeleteExpectedObjectQueue.push(object.first);
			}
		}
	}
}