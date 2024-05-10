#pragma once

#include "EngineDataConverter.h"

#include <physx\PxPhysicsAPI.h>
#include <memory>
#include <string>
#include <wrl.h>
using namespace Microsoft::WRL;
using namespace std;

#include "../02. GraphicsEngine/Types.h"
#include "../02. GraphicsEngine/Define.h"
#include "../02. GraphicsEngine/RenderManager.h"

void DebugCapsule(physx::PxRigidActor* _body, physx::PxShape* _shape);
void DebugBox(physx::PxRigidActor* _body, physx::PxShape* _shape);
void DebugSphere(physx::PxRigidActor* _body, physx::PxShape* _shape);
void DebugConvexMesh(physx::PxRigidActor* _body, physx::PxShape* _shape);
