#pragma once

namespace Engine
{
	class Object;
	class Pawn;

	class ObjectManager
	{
		friend class GameManager;
	private:
		ObjectManager() {}
	public:
		static ObjectManager* GetInstance()
		{
			static ObjectManager instance;
			return &instance;
		}

	public:
		void Update();

	public:
		template <typename T>
		shared_ptr<T> CreateObject();
		
	public:
		inline shared_ptr<Object> FindObject(unsigned int _id);
		inline void DeleteObject(unsigned int _id);

	private:
		unsigned int m_ObjectId = 0;
		unordered_map<unsigned int, shared_ptr<Object>> m_ObjectMap;
		queue<shared_ptr<Object>> m_DeleteObjectVec;
		queue<unsigned int> m_DeleteExpectedObjectQueue;
	};

	template <typename T>
	inline shared_ptr<T> ObjectManager::CreateObject()
	{
		// 해당 클래스가 오브젝트를 상속받지 않는 클래스라면 오류
		bool bIsBase = std::is_base_of<Object, T>::value;
		assert(bIsBase == true);

		shared_ptr<T> object = make_shared<T>(m_ObjectId);
		m_ObjectMap.insert(make_pair(m_ObjectId++, object));

		return object;
	}

	shared_ptr<Object> ObjectManager::FindObject(unsigned int _id)
	{
		auto object = m_ObjectMap.find(_id);
		if (object != m_ObjectMap.end())
		{
			return nullptr;
		}

		return object->second;
	}

	void ObjectManager::DeleteObject(unsigned int _id)
	{
		shared_ptr<Object> object = FindObject(_id);

		m_DeleteObjectVec.push(object);
		m_ObjectMap.erase(_id);
	}
}
