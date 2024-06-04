#pragma once
#include "GameProcess.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <vector>
#include <memory>
#include <functional>
#include "../03. PhysXEngine/Common.h"

// Engine 정적 라이브러리 프로젝트 내의 모든 클래스들은 namespace Engine을 썼으므로 클래스 선언 할 때에는 
// namespace Engine을 포함해야지 쓸 수 있습니다.
namespace Engine
{
	class Object;
}

namespace physics
{
	class FQPhysics;
}

// 게임 프로세스 클래스를 상속 받아야 사용할 수 있습니다.
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