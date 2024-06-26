#pragma once

#include "PhysicsCookingMeshTool.h"
#include "StaticRigidBody.h"
#include "DynamicRigidBody.h"
#include "RigidBody.h"

namespace physics
{
	class PhysicsResourceManager;

	using PolygonMesh = std::shared_ptr<std::vector<std::vector<DirectX::SimpleMath::Vector3>>>;

	class PhysicsRigidBodyManager
	{
	public:
		PhysicsRigidBodyManager();
		~PhysicsRigidBodyManager();

		/// <summary>
		/// ������ �ٵ� ���� �� �����ϴ� �Ŵ����� �����մϴ�.
		/// </summary>
		bool Initialize(physx::PxPhysics* physics, std::shared_ptr<PhysicsResourceManager> resourceManager);

		/// <summary>
		/// ������ ������ �ٵ���� �� ���� ���� ������ �����մϴ�.
		/// </summary>
		bool Update(physx::PxScene* scene, int* collisionMatrix);

		/// <summary>
		/// ����� ������ �� ���� ���� ������ Ŭ����� ������Ʈ
		/// </summary>
		bool FinalUpdate();

		/// <summary>
		/// ���� ������ �߰��� ����ƽ �ٵ� �� ���̳��� �ٵ� �����մϴ�.
		/// </summary>
		/// <param name="info"> ������ �ٵ��� ���� ���� �Դϴ�. </param>
		/// <param name="colliderType"> �ݶ��̴� Ÿ��(Ʈ����, �ݸ���) �Դϴ�. </param>
		bool CreateStaticBody(const BoxColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateStaticBody(const SphereColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateStaticBody(const CapsuleColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateStaticBody(const ConvexMeshColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateStaticBody(const TriangleMeshColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateStaticBody(const HeightFieldColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateDynamicBody(const BoxColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateDynamicBody(const SphereColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateDynamicBody(const CapsuleColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateDynamicBody(const ConvexMeshColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateDynamicBody(const TriangleMeshColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);
		bool CreateDynamicBody(const HeightFieldColliderInfo& info, const EColliderType& colliderType, const std::string& name, int* collisionMatrix);

		bool SettingStaticBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, int* collisionMatrix);
		bool SettingDynamicBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, int* collisionMatrix);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="info"></param>
		/// <param name="colliderType"></param>
		/// <param name="collisionMatrix"></param>
		/// <returns></returns>
		void GetRigidBodyData(unsigned int id, RigidBodyGetSetData& rigidBodyData);

		/// <summary>
		/// ���̵� ������ �ش� ���̵��� ������ �ٵ𿡰� ������ Ʈ���������� �̵� ( �����̵� )
		/// </summary>
		bool SetRigidBodyData(const unsigned int& id, const RigidBodyGetSetData& rigidBodyData);

		/// <summary>
		/// ���̵� ���� ���� ������ �ٵ� �����մϴ�.
		/// </summary>
		bool RemoveRigidBody(const unsigned int& id, physx::PxScene* scene);

		/// <summary>
		/// ���� ������ �ִ� ��� ������ �ٵ���� �����մϴ�.
		/// </summary>
		bool RemoveAllRigidBody(physx::PxScene* scene);

		/// <summary>
		/// �浹 ��Ʈ������ ������ �ȴٸ� ��� ������ �ٵ���� �浹 ��Ʈ������ ������Ʈ�մϴ�.
		/// </summary>
		/// <param name="collisionMatrix"></param>
		void UpdateCollisionMatrix(int* collisionMatrix);

		/// <summary>
		/// ���� ������ �ٵ���� ����� �����͸� �����մϴ�.
		/// </summary>
		void ExtractDebugData();

		/// <summary>
		/// ���� ���� �����͸� Ŭ���� �մϴ�.
		/// </summary>
		void UserDataClear();

		inline std::unordered_map<unsigned int, PolygonMesh>& GetDebugPolygon();
		inline std::unordered_map<unsigned int, std::shared_ptr<RigidBody>>& GetRigidBodyContainer();

	private:
		physx::PxPhysics* mPhysics;

		std::weak_ptr<PhysicsResourceManager> mResourceManager;

		std::unordered_map<unsigned int, std::shared_ptr<RigidBody>> mRigidBodyContainer;
		std::unordered_map<unsigned int, std::shared_ptr<CollisionData>> mCollisionDataContainer;
		std::vector<std::shared_ptr<RigidBody>> mUpcomingActors;

		std::unordered_map<unsigned int, PolygonMesh> mDebugPolygon;
	};

	std::unordered_map<unsigned int, PolygonMesh>& PhysicsRigidBodyManager::GetDebugPolygon()
	{
		return mDebugPolygon;
	}

	std::unordered_map<unsigned int, std::shared_ptr<RigidBody>>& PhysicsRigidBodyManager::GetRigidBodyContainer()
	{
		return mRigidBodyContainer;
	}
}
