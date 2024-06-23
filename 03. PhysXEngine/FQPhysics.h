#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING

#include "Common.h"
#include "PhysicsRigidBodyManager.h"
#include "PhysicsCharacterPhysicsManager.h"

#include <d3d11.h>

namespace physics
{
	class Physics;
	class PhysicsSimulationEventCallback;
	class PhysicsCharactorControllerManager;
	class PhysicsResourceManager;
	class PhysicsClothManager;

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

		/// <summary>
		/// ����ĳ��Ʈ : ����, ����, �Ÿ����� ���� ���� ���� ������ ������Ʈ���� �浹 �˻�
		/// </summary>
		virtual RayCastData RayCast(const DirectX::SimpleMath::Vector3& origin, const DirectX::SimpleMath::Vector3& direction, const float& distance);

#pragma region RigidBodyManager
		/// <summary>
		/// ���� ������ �߰��� ����ƽ �ٵ� �� ���̳��� �ٵ� �����մϴ�.
		/// </summary>
		template <typename ColliderInfo>
		bool CreateStaticBody(const ColliderInfo& info, const EColliderType& colliderType, const std::string& name);
		template <typename ColliderInfo>
		bool CreateDynamicBody(const ColliderInfo& info, const EColliderType& colliderType, const std::string& name);

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

		bool SimulationCharacter(unsigned int id);

		template <typename ...Params>
		bool AddArticulationLink(unsigned int id, const CharacterLinkInfo& info, Params...);
#pragma endregion

#pragma region ClothManager

		virtual bool CreateCloth(const PhysicsClothInfo& info);

		bool RegisterD3D11Buffer(unsigned int id, ID3D11Buffer* clothBuffer);

		PhysicsClothGetData GetPhysicsCloth(unsigned int id);
		void SetPhysicsCloth(unsigned int id, const PhysicsClothSetData& clothData);

#pragma endregion


	private:
		// ��
		physx::PxScene* mScene;
		physx::PxCudaContextManager* mCudaContextManager;
		std::shared_ptr<Physics> mPhysics;
		std::shared_ptr<PhysicsRigidBodyManager> mRigidBodyManager;
		std::shared_ptr<PhysicsCharactorControllerManager> mCCTManager;
		std::shared_ptr<PhysicsCharacterPhysicsManager> mCharacterPhysicsManager;
		std::shared_ptr<PhysicsClothManager> mPhysicsClothManager;
		std::shared_ptr<PhysicsResourceManager> mResourceManager;

		// �浹 �̺�Ʈ Ŭ����
		std::shared_ptr<PhysicsSimulationEventCallback> mMyEventCallback;

		// �浹 ��Ʈ����
		int mCollisionMatrix[16];
	};

	template<typename ColliderInfo>
	inline bool FQPhysics::CreateStaticBody(const ColliderInfo& info, const EColliderType& colliderType, const std::string& name)
	{
		return mRigidBodyManager->CreateStaticBody(info, colliderType, name, mCollisionMatrix);
	}
	template<typename ColliderInfo>
	inline bool FQPhysics::CreateDynamicBody(const ColliderInfo& info, const EColliderType& colliderType, const std::string& name)
	{
		return mRigidBodyManager->CreateDynamicBody(info, colliderType, name, mCollisionMatrix);
	}

	template<typename ...Params>
	inline bool FQPhysics::AddArticulationLink(unsigned int id, const CharacterLinkInfo& info, Params... params)
	{
		return mCharacterPhysicsManager->AddArticulationLink(id, info, mCollisionMatrix, params...);
	}
}

