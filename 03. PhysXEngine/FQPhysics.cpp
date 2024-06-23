#include "pch.h"
#include "FQPhysics.h"

#include "Physics.h"
#include "PhysicsRigidBodyManager.h"
#include "PhysicsCharactorControllerManager.h"
#include "PhysicsCharacterPhysicsManager.h"
#include "PhysicsResourceManager.h"
#include "PhysicsClothManager.h"
#include "PhysicsSimulationEventCallback.h"
#include "EngineDataConverter.h"

namespace physics
{ 
	/// <summary>
	/// 충돌 콜백 함수
	/// <summary>
	physx::PxFilterFlags CustomSimulationFilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		/// <summary>
		/// 쌍에 대해 CCD를 활성화하고 초기 및 CCD 연락처에 대한 연락처 보고서를 요청합니다.
		/// 또한 접점별 정보를 제공하고 행위자에게 정보를 제공합니다
		/// 접촉할 때 포즈를 취합니다.
		/// <summary>

		// 필터 셰이더 로직 ( 트리거 )
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			if ((((1 << filterData0.word0) & filterData1.word1) > 0) && (((1 << filterData1.word0) & filterData0.word1) > 0))
			{
				pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT
					| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
					| physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
				return physx::PxFilterFlag::eDEFAULT;
			}
		}

		// 필터 데이터 충돌 체크 ( 시뮬레이션 )
		if ((((1 << filterData0.word0) & filterData1.word1) > 0) && (((1 << filterData1.word0) & filterData0.word1) > 0))
		{
			pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT
				| physx::PxPairFlag::eDETECT_CCD_CONTACT
				| physx::PxPairFlag::eNOTIFY_TOUCH_CCD
				| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
				| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
				| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS
				| physx::PxPairFlag::eCONTACT_EVENT_POSE
				| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			return physx::PxFilterFlag::eDEFAULT;
		}
		else
		{
			pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT
				| physx::PxPairFlag::eDETECT_CCD_CONTACT
				| physx::PxPairFlag::eNOTIFY_TOUCH_CCD
				| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
				| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
				| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS
				| physx::PxPairFlag::eCONTACT_EVENT_POSE
				| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			return physx::PxFilterFlag::eDEFAULT;

			//pairFlags &= ~physx::PxPairFlag::eCONTACT_DEFAULT; // 충돌 행동 비허용
			//return physx::PxFilterFlag::eSUPPRESS;
		}
	}

	FQPhysics::FQPhysics()
		: mPhysics(std::make_shared<Physics>())
		, mRigidBodyManager(std::make_shared<PhysicsRigidBodyManager>())
		, mCCTManager(std::make_shared<PhysicsCharactorControllerManager>())
		, mCharacterPhysicsManager(std::make_shared<PhysicsCharacterPhysicsManager>())
		, mMyEventCallback(std::make_shared<PhysicsSimulationEventCallback>())
		, mResourceManager(std::make_shared<PhysicsResourceManager>())
		, mPhysicsClothManager(std::make_shared<PhysicsClothManager>())
		, mScene(nullptr)
		, mCollisionMatrix{}
	{
	}

	FQPhysics::~FQPhysics()
	{
		mCCTManager = nullptr;
		mRigidBodyManager = nullptr;
		PX_RELEASE(mScene);
	}

	bool FQPhysics::Initialize(PhysicsEngineInfo& info)
	{
		mPhysics->Initialize();

		physx::PxPhysics* physics = mPhysics->GetPhysics();

		// PhysX 시뮬레이션을 위한 Scene을 설정합니다.
		physx::PxSceneDesc sceneDesc(physics->getTolerancesScale()); // Scene을 생성할 때 물리적인 허용 오차 스케일을 설정합니다.

		if (PxGetSuggestedCudaDeviceOrdinal(mPhysics->GetFoundation()->getErrorCallback()) >= 0)
		{
			// initialize CUDA
			physx::PxCudaContextManagerDesc cudaContextManagerDesc;
			mCudaContextManager = PxCreateCudaContextManager(*mPhysics->GetFoundation(), cudaContextManagerDesc, PxGetProfilerCallback());
			if (mCudaContextManager && !mCudaContextManager->contextIsValid())
			{
				mCudaContextManager->release();
				mCudaContextManager = NULL;
			}
		}
		if (mCudaContextManager == NULL)
		{
			PxGetFoundation().error(physx::PxErrorCode::eINVALID_OPERATION, PX_FL, "Failed to initialize CUDA!\n");
			return false;
		}

		// 중력을 설정합니다.
		sceneDesc.gravity.x = info.gravity.x;
		sceneDesc.gravity.y = info.gravity.y;
		sceneDesc.gravity.z = -info.gravity.z;
		memcpy(mCollisionMatrix, info.collisionMatrix, sizeof(int) * 16);

		// 충돌 쌍 플래그입니다.
		physx::PxPairFlags pairFlags = physx::PxPairFlags();
		
		// Scene 설명자에 CPU 디스패처와 필터 셰이더를 설정합니다.
		sceneDesc.cpuDispatcher = mPhysics->GetDispatcher();
		sceneDesc.filterShader = CustomSimulationFilterShader;
		sceneDesc.simulationEventCallback = mMyEventCallback.get();
		sceneDesc.cudaContextManager = mCudaContextManager;
		sceneDesc.staticStructure = physx::PxPruningStructureType::eDYNAMIC_AABB_TREE;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
		sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
		sceneDesc.solverType = physx::PxSolverType::eTGS;

		// PhysX Phsics에서 PhysX의 Scene을 생성합니다.
		mScene = physics->createScene(sceneDesc);
		assert(mScene);

		// 매니저 초기화
		if (!mResourceManager->Initialize(mPhysics->GetPhysics())) return false;
		if (!mRigidBodyManager->Initialize(mPhysics->GetPhysics(), mResourceManager)) return false;
		if (!mCCTManager->initialize(mPhysics->GetPhysics(), mScene)) return false;
		if (!mCharacterPhysicsManager->initialize(mPhysics->GetPhysics(), mScene)) return false;
		if (!mPhysicsClothManager->Initialize(mCudaContextManager, mPhysics->GetPhysics(), mScene)) return false;

		// PVD 클라이언트에 PhysX Scene 연결 ( Debug )
#ifdef _DEBUG
		mPhysics->SettingPVDClient(mScene);
#endif

		return true;
	}

	bool FQPhysics::Update(float deltaTime)
	{
		if (!mRigidBodyManager->Update(mScene, mCollisionMatrix))
			return false;
		if (!mCCTManager->Update(deltaTime))
			return false;

		if (!mScene->simulate(deltaTime))
			return false;
		if (!mScene->fetchResults(true))
			return false;
		if (!mPhysicsClothManager->Update())
			return false;

		return true;
	}

	bool FQPhysics::FinalUpdate()
	{
		if (!mRigidBodyManager->FinalUpdate())
			return false;
		if (!mCCTManager->FinalUpdate())
			return false;

		return true;
	}

	void FQPhysics::SetCallBackFunction(std::function<void(physics::CollisionData, ECollisionEventType)> func)
	{
		mMyEventCallback->SetCallbackFunction(func);
	}

	void FQPhysics::SetPhysicsInfo(PhysicsEngineInfo& info)
	{
		mScene->setGravity(physx::PxVec3(info.gravity.x, info.gravity.y, -info.gravity.z));
		memcpy(mCollisionMatrix, info.collisionMatrix, sizeof(int) * 16);
		mRigidBodyManager->UpdateCollisionMatrix(mCollisionMatrix);
		mCCTManager->UpdateCollisionMatrix(mCollisionMatrix);
	}

	RayCastData FQPhysics::RayCast(const DirectX::SimpleMath::Vector3& origin, const DirectX::SimpleMath::Vector3& direction, const float& distance)
	{
		RayCastData raycastData;
		physx::PxRaycastBuffer hitBuffer;

		physx::PxVec3 pxOrigin;
		physx::PxVec3 pxDirection;
		CopyDxVec3ToPxVec3(origin, pxOrigin);
		CopyDxVec3ToPxVec3(direction, pxDirection);

		bool isBlock = mScene->raycast(pxOrigin, pxDirection, distance, hitBuffer);
		if (isBlock)
		{
			unsigned int hitSize = hitBuffer.getNbAnyHits();
			raycastData.hitSize = hitSize;

			for (unsigned int hitNumber = 0; hitNumber < hitSize; hitNumber++)
			{
				const physx::PxRaycastHit& hit = hitBuffer.getAnyHit(hitNumber);
				physx::PxShape* shape = hit.shape;

				DirectX::SimpleMath::Vector3 position;
				CopyPxVec3ToDxVec3(hit.position, position);
				unsigned int id = static_cast<CollisionData*>(shape->userData)->myId;
				unsigned int layerNumber = static_cast<CollisionData*>(shape->userData)->myLayerNumber;

				raycastData.contectPoints.push_back(position);
				raycastData.id.push_back(id);
				raycastData.layerNumber.push_back(layerNumber);
			}
		}

		return raycastData;
	}

#pragma region RigidBodyManager
	bool FQPhysics::RemoveRigidBody(const unsigned int& id)
	{
		return mRigidBodyManager->RemoveRigidBody(id, mScene);
	}

	bool FQPhysics::RemoveAllRigidBody()
	{
		if (!mRigidBodyManager->RemoveAllRigidBody(mScene))
			return false;

		return true;
	}

	RigidBodyGetSetData FQPhysics::GetRigidBodyData(const unsigned int& id)
	{
		RigidBodyGetSetData rigidBodyData;
		mRigidBodyManager->GetRigidBodyData(id, rigidBodyData);

		return rigidBodyData;
	}

	bool FQPhysics::SetRigidBodyData(const unsigned int& id, const RigidBodyGetSetData& rigidBodyData)
	{
		return mRigidBodyManager->SetRigidBodyData(id, rigidBodyData);
	}

	const std::unordered_map<unsigned int, PolygonMesh>& FQPhysics::GetDebugPolygon()
	{
		return mRigidBodyManager->GetDebugPolygon();
	}

#pragma endregion

#pragma region CCTManager

	bool FQPhysics::CreateCCT(const CharacterControllerInfo& controllerInfo, const CharacterMovementInfo& movementInfo)
	{
		return mCCTManager->CreateCCT(controllerInfo, movementInfo, mCollisionMatrix);
	}

	bool FQPhysics::RemoveController(const unsigned int& id)
	{
		return mCCTManager->RemoveController(id);
	}

	bool FQPhysics::AddInputMove(const unsigned int& id, const DirectX::SimpleMath::Vector3& input)
	{
		return mCCTManager->AddInputMove(id, input);
	}

	CharacterControllerGetSetData FQPhysics::GetCharacterControllerData(const unsigned int& id)
	{
		CharacterControllerGetSetData data;
		mCCTManager->GetCharacterControllerData(id, data);

		return data;
	}

	CharacterMovementGetSetData FQPhysics::GetCharacterMovementData(const unsigned int& id)
	{
		CharacterMovementGetSetData data;
		mCCTManager->GetCharacterMovementData(id, data);

		return data;
	}

	void FQPhysics::SetCharacterControllerData(const unsigned int& id, const CharacterControllerGetSetData& controllerData)
	{
		mCCTManager->SetCharacterControllerData(id, controllerData);
	}

	void FQPhysics::SetCharacterMovementData(const unsigned int& id, const CharacterMovementGetSetData& movementData)
	{
		mCCTManager->SetCharacterMovementData(id, movementData);
	}
#pragma endregion

#pragma region CharacterPhysicsManager
	bool FQPhysics::CreateCharacterphysics(const CharacterPhysicsInfo& info)
	{
		return mCharacterPhysicsManager->CreateCharacterphysics(info);
	}
	bool FQPhysics::SimulationCharacter(unsigned int id)
	{
		return mCharacterPhysicsManager->SimulationCharacter(id);
	}
#pragma endregion

	bool FQPhysics::CreateCloth(const PhysicsClothInfo& info)
	{
		return mPhysicsClothManager->CreateCloth(info);
	}
	bool FQPhysics::RegisterD3D11Buffer(unsigned int id, ID3D11Buffer* clothBuffer)
	{
		return mPhysicsClothManager->RegisterD3D11Buffer(id, clothBuffer);
	}
	PhysicsClothGetData FQPhysics::GetPhysicsCloth(unsigned int id)
	{
		PhysicsClothGetData clothData;
		mPhysicsClothManager->GetClothData(id, clothData);

		return clothData;
	}
	void FQPhysics::SetPhysicsCloth(unsigned int id, const PhysicsClothSetData& clothData)
	{
		mPhysicsClothManager->SetClothData(id, clothData);
	}
}
