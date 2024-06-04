#pragma once
#include "GameProcess.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <vector>
#include <memory>
#include <functional>
#include "../03. PhysXEngine/Common.h"

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