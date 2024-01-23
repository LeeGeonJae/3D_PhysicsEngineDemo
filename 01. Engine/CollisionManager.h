#pragma once


namespace Engine
{
	// ���� : Ŭ���� ����
	class CollisionComponent;

	// -------------------------------------------------------------------------
	// ���� : �ݸ��� �Ŵ��� ( �̱��� )
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

	// ���� : �ݸ��� ����
	// ���� : �ݸ��� ���� �ÿ� �ݸ��� ���̵� �����ϰ� �ݸ��� ������Ʈ �������ֱ� ���� ���ø� �Լ�
	template <typename T>
	inline shared_ptr<T> CollisionManager::CreateCollision()
	{
		// ���� : �ش� Ŭ������ ������Ʈ�� ��ӹ��� �ʴ� Ŭ������� ����
		bool bIsBase = std::is_base_of<CollisionComponent, T>::value;
		assert(bIsBase == true);

		shared_ptr<T> collision = make_shared<T>(m_CollisionId);
		m_CollisionMap.insert(make_pair(m_CollisionId++, collision));

		return collision;
	}

	// ���� : ���̵� ���� ������ �ش� �ݸ����� ã���ִ� �Լ�
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
