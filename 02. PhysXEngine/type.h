#pragma once

#include "directxtk\SimpleMath.h"

#include "../Libraries/Include/CUDA/vector_types.h"

namespace PhysicsEngine
{
	struct PhysicsVertex
	{
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector2 uv;
		DirectX::SimpleMath::Vector3 normal;
		DirectX::SimpleMath::Vector3 tangent;
		DirectX::SimpleMath::Vector3 biTangent;
	};

	struct PhysicsClothGetData
	{
		DirectX::SimpleMath::Matrix worldTransform;
		DirectX::SimpleMath::Vector3* vertices;
		DirectX::SimpleMath::Vector3* nomals;
		DirectX::SimpleMath::Vector3* tangents;
		DirectX::SimpleMath::Vector3* biTangents;
		DirectX::SimpleMath::Vector2* uv;
		unsigned int vertexSize;
		unsigned int* indices;
		unsigned int indexSize;
	};

	struct PhysicsClothMaterialInfo
	{
		float friction = 0.8f;
		float damping = 0.001f;
		float adhesion = 1e+7f;
		float viscosity = 0.001f;
		float vorticityConfinement = 0.5f;
		float surfaceTension = 0.005f;
		float cohesion = 0.05f;
		float lift = 0.f;
		float drag = 0.f;
		float cflCoefficient = 1.f;
		float gravityScale = 2.f;
	};

	struct PhysicsClothInfo
	{
		unsigned int id = 0;
		unsigned int layerNumber = 0;
		PhysicsClothMaterialInfo materialInfo;
		DirectX::SimpleMath::Matrix worldTransform = {};
		void* DirectXBuffer = nullptr;
		DirectX::SimpleMath::Vector3* vertices = nullptr;
		DirectX::SimpleMath::Vector2* uv = nullptr;
		unsigned int vertexSize = 0;
		unsigned int* indices = nullptr;
		unsigned int indexSize = 0;
		unsigned int* activeVertexIndices = nullptr;
		unsigned int activeVertexSize = 0;
		float totalClothMass = 1.f;
	};

	struct PhysicsClothSetData
	{
		DirectX::SimpleMath::Matrix worldTransform;
	};
}


extern "C" void launchCalculateNormals(
	float3 * vertices,
	float2 * uvs,
	unsigned int vertexSize,
	unsigned int* indices,
	unsigned int indexSize,
	PhysicsEngine::PhysicsVertex * buffer);