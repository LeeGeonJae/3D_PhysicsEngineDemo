#pragma once

namespace Engine
{
	// 건재 : 클래스 선언
	class Object;
	class Pawn;

	// -------------------------------------------------------------------------
	// 건재 : 오브젝트 매니저 클래스 ( 오브젝트들을 생성 및 삭제 관리해주는 매니저 )
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

	// 건재 : 오브젝트를 생성해주는 템플릿 함수
	template <typename T>
	inline shared_ptr<T> ObjectManager::CreateObject()
	{
		// 건재 : 해당 클래스가 오브젝트를 상속받지 않는 클래스라면 오류
		bool bIsBase = std::is_base_of<Object, T>::value;
		assert(bIsBase == true);

		// 건재 : 오브젝트에 오브젝트 아이디를 부여하고 오브젝트 맵에 insert
		shared_ptr<T> object = make_shared<T>(m_ObjectId);
		m_ObjectMap.insert(make_pair(m_ObjectId++, object));
		
		return object;
	}

	// 건재 : 오브젝트를 찾아주는 함수
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
