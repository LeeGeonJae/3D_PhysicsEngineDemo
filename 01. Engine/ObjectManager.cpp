#include "pch.h"
#include "ObjectManager.h"

#include "Object.h"

namespace Engine
{
	void ObjectManager::Update()
	{
		// ���� : Update�� �� bIsDead ���� �༮���� 1������ ���Ŀ� ó��
		while (!m_DeleteExpectedObjectQueue.empty())
		{
			m_ObjectMap.erase(m_DeleteExpectedObjectQueue.front());
			m_DeleteExpectedObjectQueue.pop();
		}
		// ���� : Update�� �� ������Ʈ���� ��ȸ�ϸ鼭 bIsDead ���� ������ Ȯ�� �� �Ŀ� queue�� �Է�
		for (const auto& object : m_ObjectMap)
		{
			if (object.second->GetIsDead())
			{
				m_DeleteExpectedObjectQueue.push(object.first);
			}
		}
	}
}