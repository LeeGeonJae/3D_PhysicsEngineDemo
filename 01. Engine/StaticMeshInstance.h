#pragma once

namespace Engine
{
	class StaticMesh;
	class Material;

	class StaticMeshInstance
	{
	public:
		StaticMeshInstance();
		~StaticMeshInstance();

	public:
		void Create(StaticMesh* _pMeshResource, Material* _pMaterial, Matrix* _pTransform);

	public:
		inline StaticMesh* GetStaticMesh();
		inline Material* GetMaterial();
		inline Matrix* GetMatrix();

	private:
		StaticMesh* m_pMeshResource;
		Material* m_pMaterial;
		Matrix* m_pTransform;
	};

	StaticMesh* StaticMeshInstance::GetStaticMesh()
	{
		return m_pMeshResource;
	}
	Material* StaticMeshInstance::GetMaterial()
	{
		return m_pMaterial;
	}
	Matrix* StaticMeshInstance::GetMatrix()
	{
		return m_pTransform;
	}
}

