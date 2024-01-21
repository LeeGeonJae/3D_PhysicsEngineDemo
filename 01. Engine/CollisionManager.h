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

	private:
		unsigned int m_CollisionId = 0;
		unordered_map<unsigned int, shared_ptr<CollisionComponent>> m_CollisionMap;
	};

	// ���� : �ݸ��� ����
	// ���� : �ݸ��� ���� �ÿ� �ݸ��� ���̵� �����ϰ� �ݸ��� ������Ʈ �������ֱ� ���� �Լ�
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
		auto object = m_CollisionMap.find(_id);
		assert(object != m_CollisionMap.end());

		return object->second;
	}
	// -------------------------------------------------------------------------
}
