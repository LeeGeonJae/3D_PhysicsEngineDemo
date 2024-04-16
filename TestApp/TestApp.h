#pragma once
#include "GameProcess.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

#pragma comment(lib, "../Lib/physX/deb/LowLevel_static_64.lib")
#pragma comment(lib, "../Lib/physX/deb/LowLevelAABB_static_64.lib")
#pragma comment(lib, "../Lib/physX/deb/LowLevelDynamics_static_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PhysX_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PhysXCommon_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PhysXCooking_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PhysXFoundation_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PhysXTask_static_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PhysXVehicle_static_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "../Lib/physX/deb/PVDRuntime_64.lib")
#pragma comment(lib, "../Lib/physX/deb/SceneQuery_static_64.lib")
#pragma comment(lib, "../Lib/physX/deb/SimulationController_static_64.lib")

// Engine ���� ���̺귯�� ������Ʈ ���� ��� Ŭ�������� namespace Engine�� �����Ƿ� Ŭ���� ���� �� ������ 
// namespace Engine�� �����ؾ��� �� �� �ֽ��ϴ�.
namespace Engine
{
	class Object;
}

namespace PhysicsEngine
{
	class PhysX;
}

// ���� ���μ��� Ŭ������ ��� �޾ƾ� ����� �� �ֽ��ϴ�.
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