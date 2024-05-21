#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING

#include "Common.h"

namespace physics
{
	class Physics;
	class PhysicsRigidBodyManager;
	class PhysicsSimulationEventCallback;
	class PhysicsCharactorControllerManager;
	class PhysicsCharacterPhysicsManager;

	using PolygonMesh = std::shared_ptr<std::vector<std::vector<DirectX::SimpleMath::Vector3>>>;

	class FQPhysics
	{
	public:
		FQPhysics();
		virtual ~FQPhysics();

		/// <summary>
		/// ���� ������ �����Ͽ� ���� ������ ����ϴ�.
		/// </summary>
		virtual bool Initialize(PhysicsEngineInfo& info);

		/// <summary>
		/// ���� ������ ������Ʈ�� ���� ������ �ٵ� �߰��ϰ� �ùķ��̼��� �����մϴ�.
		/// �ùķ��̼��� �������� �ÿ� �浹 �̺�Ʈ�� �ݹ� �Լ��� ���� ����˴ϴ�.
		/// </summary>
		virtual bool Update(float deltaTime);

		/// <summary>
		/// ����� ������ �� ���� ���� ������ Ŭ����� ������Ʈ
		/// </summary>
		virtual bool FinalUpdate();

		/// <summary>
		/// �ݹ� �Լ� ���
		/// </summary>
		/// <param name="func"> �ݸ��� ������ ����ü </param>
		/// <param name="eventType"> �ݸ��� �̺�Ʈ Ÿ�� </param>
		virtual void SetCallBackFunction(std::function<void(physics::CollisionData, physics::ECollisionEventType)> func);

		/// <summary>
		/// ���� ������ ������ ������ ���� �޽��ϴ�. ( �߷�, �浹 ��Ʈ���� )
		/// </summary>
		/// <param name="info"> ���� ���� ���� </param>
		virtual void SetPhysicsInfo(PhysicsEngineInfo& info);

#pragma region RigidBodyManager
		/// <summary>
		/// ���� ������ �߰��� ����ƽ �ٵ� �� ���̳��� �ٵ� �����մϴ�.
		/// </summary>
		virtual bool CreateStaticBody(const BoxColliderInfo& info, const EColliderType& colliderType);
		virtual bool CreateStaticBody(const SphereColliderInfo& info, const EColliderType& colliderType);
		virtual bool CreateStaticBody(const CapsuleColliderInfo& info, const EColliderType& colliderType);
		virtual bool CreateStaticBody(const ConvexMeshColliderInfo& info, const EColliderType& colliderType);
		virtual bool CreateDynamicBody(const BoxColliderInfo& info, const EColliderType& colliderType);
		virtual bool CreateDynamicBody(const SphereColliderInfo& info, const EColliderType& colliderType);
		virtual bool CreateDynamicBody(const CapsuleColliderInfo& info, const EColliderType& colliderType);
		virtual bool CreateDynamicBody(const ConvexMeshColliderInfo& info, const EColliderType& colliderType);

		/// <summary>
		/// ���̵� ������ �ش� ���̵��� ������ �ٵ� ��ȯ
		/// </summary>
		virtual RigidBodyGetSetData GetRigidBodyData(const unsigned int& id);

		/// <summary>
		/// ���̵� ������ �ش� ���̵��� ������ �ٵ𿡰� ������ Ʈ���������� �̵� ( �����̵� )
		/// </summary>
		virtual bool SetRigidBodyData(const unsigned int& id, const RigidBodyGetSetData& rigidBodyData);

		/// <summary>
		/// ���̵� ���� ���� ������ �ٵ� �����մϴ�.
		/// </summary>
		virtual bool RemoveRigidBody(const unsigned int& id);

		/// <summary>
		/// ���� ������ �ִ� ��� ������ �ٵ���� �����մϴ�.
		/// </summary>
		virtual bool RemoveAllRigidBody();

		/// <summary>
		/// �������� ����� ������ 
		/// </summary>
		const std::unordered_map<unsigned int, PolygonMesh>& GetDebugPolygon();
#pragma endregion

#pragma region CharacterControllerManager

		/// <summary>
		/// ĳ���� ��Ʈ�ѷ� ���� �Լ�
		/// </summary>
		/// <param name="controllerInfo"> ĳ���� ��Ʈ�ѷ� ���� ������ </param>
		/// <param name="movementInfo"> ĳ���� �����Ʈ ���� ������ </param>
		virtual bool CreateCCT(const CharacterControllerInfo& controllerInfo, const CharacterMovementInfo& movementInfo);

		/// <summary>
		/// ĳ���� ��Ʈ�ѷ� ���� �Լ�
		/// </summary>
		/// <param name="id"> ������ ĳ���� ��Ʈ�ѷ��� ���̵� </param>
		virtual bool RemoveController(const unsigned int& id);

		/// <summary>
		/// Ư�� ĳ���� ��Ʈ�ѷ��� �Է� �� �߰� �Լ�
		/// </summary>
		/// <param name="id"> ĳ���� ��Ʈ�ѷ� ���̵� </param>
		/// <param name="input"> �Է��� �̵� ���� (ex. {1.f, 0.f, 0.f}) </param>
		virtual bool AddInputMove(const unsigned int& id, const DirectX::SimpleMath::Vector3& input);

		/// <summary>
		/// ĳ���� ��Ʈ�ѷ��� ĳ���� �����Ʈ�� �����͸� Get Set�մϴ�.
		/// </summary>
		virtual CharacterControllerGetSetData GetCharacterControllerData(const unsigned int& id);
		virtual CharacterMovementGetSetData GetCharacterMovementData(const unsigned int& id);
		virtual void SetCharacterControllerData(const unsigned int& id, const CharacterControllerGetSetData& controllerData);
		virtual void SetCharacterMovementData(const unsigned int& id, const CharacterMovementGetSetData& movementData);

#pragma endregion

#pragma region CharacterPhysicsManager
		bool CreateCharacterphysics(const CharacterPhysicsInfo& info);

		bool AddArticulationLink(unsigned int id, const CharacterLinkInfo& info, const DirectX::SimpleMath::Vector3& extent);
		bool AddArticulationLink(unsigned int id, const CharacterLinkInfo& info, const float& radius);
		bool AddArticulationLink(unsigned int id, const CharacterLinkInfo& info, const float& halfHeight, const float& radius);

		bool SimulationCharacter(unsigned int id);
#pragma endregion


	private:
		// ��
		physx::PxScene* mScene;
		std::shared_ptr<Physics> mPhysics;
		std::shared_ptr<PhysicsRigidBodyManager> mRigidBodyManager;
		std::shared_ptr<PhysicsCharactorControllerManager> mCCTManager;
		std::shared_ptr<PhysicsCharacterPhysicsManager> mCharacterPhysicsManager;

		// �浹 �̺�Ʈ Ŭ����
		std::shared_ptr<PhysicsSimulationEventCallback> mMyEventCallback;

		// �浹 ��Ʈ����
		int mCollisionMatrix[16];
	};
}

