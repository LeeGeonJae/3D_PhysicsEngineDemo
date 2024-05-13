#pragma once

namespace physics
{
	class PhysicsCookingMeshTool
	{
	public:
		PhysicsCookingMeshTool();
		~PhysicsCookingMeshTool();

		/// <summary>
		/// �� ���ؽ� �������� �о ���� �� Ŭ������ �����մϴ�.
		/// </summary>
		bool Initialize(physx::PxPhysics* physics);

		/// <summary>
		/// �� �������� ���ؽ� �������� �о ConvexMesh �����͸� �����մϴ�.
		/// </summary>
		/// <returns> PxShape�� �� ConvexMesh�� �����Ͽ� ��ȯ�մϴ�. </returns>
		physx::PxConvexMesh* CookingConvexMesh(DirectX::SimpleMath::Vector3* vertices, int vertexSize, int polygonLimit);

		/// <summary>
		/// �� �������� ���ؽ� ������ ���� �о �ش� �������� Ű ���� ������ �ִ� mConvexMeshMap�� ConvexMesh�� ã�� ��ȯ�մϴ�.
		/// </summary>
		/// <returns> �̹� ���ҽ� ������ ������ ConvexMesh ��ȯ, ������ nullptr�� ��ȯ�մϴ�. </returns>
		inline physx::PxConvexMesh* FindConvexMesh(DirectX::SimpleMath::Vector3* vertices);

	private:
		physx::PxPhysics* mPhysics;
		std::unordered_map<DirectX::SimpleMath::Vector3*, physx::PxConvexMesh*> mConvexMeshMap;
	};

	physx::PxConvexMesh* PhysicsCookingMeshTool::FindConvexMesh(DirectX::SimpleMath::Vector3* vertices)
	{
		if (mConvexMeshMap.find(vertices) != mConvexMeshMap.end())
			return  mConvexMeshMap.find(vertices)->second;

		return nullptr;
	}
}