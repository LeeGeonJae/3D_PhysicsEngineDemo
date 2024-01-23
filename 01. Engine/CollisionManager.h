#pragma once


namespace Engine
{
	// 건재 : 클래스 선언
	class CollisionComponent;

	// -------------------------------------------------------------------------
	// 건재 : 콜리전 매니저 ( 싱글톤 )
	class CollisionManager
	{
	private:
		CollisionManager() {}
	public:
		static CollisionManager* GetInstance()
		{
			static CollisionManager instance;
			return &instance;
		}

	public:
		void Init();
		void FixedUpdate();

	public:
		template <typename T>
		shared_ptr<T> CreateCollision();
		inline shared_ptr<CollisionComponent> FindCollision(unsigned int _id);
		inline void DeleteCollision(unsigned int _id);

	private:
		unsigned int m_CollisionId = 0;
		unordered_map<unsigned int, weak_ptr<CollisionComponent>> m_CollisionMap;
	};

	// 건재 : 콜리전 생성
	// 건재 : 콜리전 생성 시에 콜리전 아이디 배정하고 콜리전 컴포넌트 관리해주기 위한 템플릿 함수
	template <typename T>
	inline shared_ptr<T> CollisionManager::CreateCollision()
	{
		// 건재 : 해당 클래스가 오브젝트를 상속받지 않는 클래스라면 오류
		bool bIsBase = std::is_base_of<CollisionComponent, T>::value;
		assert(bIsBase == true);

		shared_ptr<T> collision = make_shared<T>(m_CollisionId);
		m_CollisionMap.insert(make_pair(m_CollisionId++, collision));

		return collision;
	}

	// 건재 : 아이디 값을 받으면 해당 콜리전은 찾아주는 함수
	shared_ptr<CollisionComponent> CollisionManager::FindCollision(unsigned int _id)
	{
		auto collision = m_CollisionMap.find(_id);
		assert(collision != m_CollisionMap.end());

		return collision->second.lock();
	}
	inline void CollisionManager::DeleteCollision(unsigned int _id)
	{
		m_CollisionMap.erase(_id);
	}
	// -------------------------------------------------------------------------
}
