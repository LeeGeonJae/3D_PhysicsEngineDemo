#pragma once

namespace Engine
{
	class StaticMeshResource;
	class Material;

	class StaticMeshInstance
	{
	public:
		StaticMeshInstance();
		~StaticMeshInstance();

	public:
		void Create(StaticMeshResource* _pMeshResource, Material* _pMaterial, Matrix* _pTransform);

	private:
		StaticMeshResource* m_pMeshResource;
		Material* m_pMaterial;
		Matrix* m_pTransform;
	};
}

