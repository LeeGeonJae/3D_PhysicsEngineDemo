#pragma once
#include "GameProcess.h"

#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>
#include <vector>
#include <memory>
#include <functional>
#include "../03. PhysXEngine/Common.h"

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

// Engine ���� ���̺귯�� ������Ʈ ���� ��� Ŭ�������� namespace Engine�� �����Ƿ� Ŭ���� ���� �� ������ 
// namespace Engine�� �����ؾ��� �� �� �ֽ��ϴ�.
namespace Engine
{
	class Object;
}

namespace physics
{
	class FQPhysics;
}

// ���� ���μ��� Ŭ������ ��� �޾ƾ� ����� �� �ֽ��ϴ�.
class DemoApp : public Engine::GameProcess
{
public:
	void Init() override;
	void Update(float _deltaTime) override;
	void LateUpdate(float _deltaTime) override;
	void FixedUpdate(float _deltaTime) override;
	void Render() override;

public:
	std::vector<std::shared_ptr<Engine::Object>> m_ObjectVec;
	std::vector<unsigned int> m_RigidBodyIDVec;
	std::function<void(physics::CollisionData, physics::ECollisionEventType)> m_Funtion;
	std::shared_ptr<physics::FQPhysics> m_Physics;
};