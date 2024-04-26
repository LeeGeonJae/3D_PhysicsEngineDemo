#pragma once
#include "GameProcess.h"

#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>

#pragma comment(lib, "LowLevel_static_64.lib")
#pragma comment(lib, "LowLevelAABB_static_64.lib")
#pragma comment(lib, "LowLevelDynamics_static_64.lib")
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysXCooking_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXTask_static_64.lib")
#pragma comment(lib, "PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysXVehicle2_static_64.lib")
//#pragma comment(lib, "PVDRuntime_64.lib")
#pragma comment(lib, "SceneQuery_static_64.lib")
#pragma comment(lib, "SimulationController_static_64.lib")

// Engine 정적 라이브러리 프로젝트 내의 모든 클래스들은 namespace Engine을 썼으므로 클래스 선언 할 때에는 
// namespace Engine을 포함해야지 쓸 수 있습니다.
namespace Engine
{
	class Object;
}

namespace PhysicsEngine
{
	class PhysX;
}

// 게임 프로세스 클래스를 상속 받아야 사용할 수 있습니다.
class TestApp : public Engine::GameProcess
{
public:
	void Init() override;
	void Update(float _deltaTime) override;
	void LateUpdate(float _deltaTime) override;
	void FixedUpdate(float _deltaTime) override;
	void Render() override;

public:
	vector<shared_ptr<Engine::Object>> m_ObjectVec;
	shared_ptr<PhysicsEngine::PhysX> m_PhysX;
};