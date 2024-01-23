#pragma once

namespace Engine
{
	// ���� : Ŭ���� ����
	class Object;
	class Pawn;

	// -------------------------------------------------------------------------
	// ���� : ������Ʈ �Ŵ��� Ŭ���� ( ������Ʈ���� ���� �� ���� �������ִ� �Ŵ��� )
	class ObjectManager
	{
		SINGLETON(ObjectManager)
	public:
		void Update();

	public:
		template <typename T>
		shared_ptr<T> CreateObject();
		
	public:
		inline shared_ptr<Object> FindObject(unsigned int _id);

	private:
		unsigned int m_ObjectId = 0;
		unordered_map<unsigned int, shared_ptr<Object>> m_ObjectMap;
		queue<unsigned int> m_DeleteExpectedObjectQueue;
	};

	// ���� : ������Ʈ�� �������ִ� ���ø� �Լ�
	template <typename T>
	inline shared_ptr<T> ObjectManager::CreateObject()
	{
		// ���� : �ش� Ŭ������ ������Ʈ�� ��ӹ��� �ʴ� Ŭ������� ����
		bool bIsBase = std::is_base_of<Object, T>::value;
		assert(bIsBase == true);

		// ���� : ������Ʈ�� ������Ʈ ���̵� �ο��ϰ� ������Ʈ �ʿ� insert
		shared_ptr<T> object = make_shared<T>(m_ObjectId);
		m_ObjectMap.insert(make_pair(m_ObjectId++, object));
		
		return object;
	}

	// ���� : ������Ʈ�� ã���ִ� �Լ�
	shared_ptr<Object> ObjectManager::FindObject(unsigned int _id)
	{
		auto object = m_ObjectMap.find(_id);
		if (object != m_ObjectMap.end())
		{
			return nullptr;
		}

		return object->second;
	}
	// -------------------------------------------------------------------------
}
