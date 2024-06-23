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
		/// 물리 엔진을 세팅하여 물리 공간을 만듭니다.
		/// </summary>
		virtual bool Initialize(PhysicsEngineInfo& info);

		/// <summary>
		/// 물리 엔진의 업데이트를 통해 리지드 바디를 추가하고 시뮬레이션을 실행합니다.
		/// 시뮬레이션이 끝마쳤을 시에 충돌 이벤트가 콜백 함수를 통해 실행됩니다.
		/// </summary>
		virtual bool Update(float deltaTime);

		/// <summary>
		/// 디버그 데이터 등 물리 엔진 데이터 클리어용 업데이트
		/// </summary>
		virtual bool FinalUpdate();

		/// <summary>
		/// 콜백 함수 등록
		/// </summary>
		/// <param name="func"> 콜리전 데이터 구조체 </param>
		/// <param name="eventType"> 콜리전 이벤트 타입 </param>
		virtual void SetCallBackFunction(std::function<void(physics::CollisionData, physics::ECollisionEventType)> func);

		/// <summary>
		/// 물리 엔진의 수정할 정보를 전달 받습니다. ( 중력, 충돌 매트릭스 )
		/// </summary>
		/// <param name="info"> 물리 엔진 정보 </param>
		virtual void SetPhysicsInfo(PhysicsEngineInfo& info);

		/// <summary>
		/// 레이캐스트 : 원점, 방향, 거리값의 선을 쏴서 물리 공간의 오브젝트들을 충돌 검사
		/// </summary>
		virtual RayCastData RayCast(const DirectX::SimpleMath::Vector3& origin, const DirectX::SimpleMath::Vector3& direction, const float& distance);

#pragma region RigidBodyManager
		/// <summary>
		/// 물리 공간에 추가할 스태틱 바디 및 다이나믹 바디 생성합니다.
		/// </summary>
		template <typename ColliderInfo>
		bool CreateStaticBody(const ColliderInfo& info, const EColliderType& colliderType, const std::string& name);
		template <typename ColliderInfo>
		bool CreateDynamicBody(const ColliderInfo& info, const EColliderType& colliderType, const std::string& name);

		/// <summary>
		/// 아이디를 받으면 해당 아이디의 리지드 바디를 반환
		/// </summary>
		virtual RigidBodyGetSetData GetRigidBodyData(const unsigned int& id);

		/// <summary>
		/// 아이디를 받으면 해당 아이디의 리지드 바디에게 지정한 트랜스폼으로 이동 ( 순간이동 )
		/// </summary>
		virtual bool SetRigidBodyData(const unsigned int& id, const RigidBodyGetSetData& rigidBodyData);

		/// <summary>
		/// 아이디 값을 받은 리지드 바디를 삭제합니다.
		/// </summary>
		virtual bool RemoveRigidBody(const unsigned int& id);

		/// <summary>
		/// 물리 공간에 있는 모든 리지드 바디들을 삭제합니다.
		/// </summary>
		virtual bool RemoveAllRigidBody();

		/// <summary>
		/// 폴리곤의 디버그 데이터 
		/// </summary>
		const std::unordered_map<unsigned int, PolygonMesh>& GetDebugPolygon();
#pragma endregion

#pragma region CharacterControllerManager
		/// <summary>
		/// 캐릭터 컨트롤러 생성 함수
		/// </summary>
		/// <param name="controllerInfo"> 캐릭터 컨트롤러 생성 데이터 </param>
		/// <param name="movementInfo"> 캐릭터 무브먼트 생성 데이터 </param>
		virtual bool CreateCCT(const CharacterControllerInfo& controllerInfo, const CharacterMovementInfo& movementInfo);

		/// <summary>
		/// 캐릭터 컨트롤러 삭제 함수
		/// </summary>
		/// <param name="id"> 삭제할 캐릭터 컨트롤러의 아이디 </param>
		virtual bool RemoveController(const unsigned int& id);

		/// <summary>
		/// 특정 캐릭터 컨트롤러의 입력 값 추가 함수
		/// </summary>
		/// <param name="id"> 캐릭터 컨트롤러 아이디 </param>
		/// <param name="input"> 입력한 이동 방향 (ex. {1.f, 0.f, 0.f}) </param>
		virtual bool AddInputMove(const unsigned int& id, const DirectX::SimpleMath::Vector3& input);

		/// <summary>
		/// 캐릭터 컨트롤러와 캐릭터 무브먼트의 데이터를 Get Set합니다.
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
		// 씬
		physx::PxScene* mScene;
		physx::PxCudaContextManager* mCudaContextManager;
		std::shared_ptr<Physics> mPhysics;
		std::shared_ptr<PhysicsRigidBodyManager> mRigidBodyManager;
		std::shared_ptr<PhysicsCharactorControllerManager> mCCTManager;
		std::shared_ptr<PhysicsCharacterPhysicsManager> mCharacterPhysicsManager;
		std::shared_ptr<PhysicsClothManager> mPhysicsClothManager;
		std::shared_ptr<PhysicsResourceManager> mResourceManager;

		// 충돌 이벤트 클래스
		std::shared_ptr<PhysicsSimulationEventCallback> mMyEventCallback;

		// 충돌 매트릭스
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

