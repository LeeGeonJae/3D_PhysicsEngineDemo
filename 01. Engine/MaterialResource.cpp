#include "pch.h"
#include "MaterialResource.h"

namespace Engine
{



	MaterialSceneResource::MaterialSceneResource()
	{
		m_ResourceType = ResourceType::Material;
	}

	MaterialSceneResource::~MaterialSceneResource()
	{
	}

	void MaterialSceneResource::Create(const string _path)
	{
		Assimp::Importer importer;

		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ 노드 생성안함
		unsigned int importFlags = aiProcess_ConvertToLeftHanded;	// 왼손 좌표계로 변환
		const aiScene* pScene = importer.ReadFile(_path, importFlags);

		assert(pScene);
		m_Name = pScene->mName.C_Str();

		for (int i = 0; i < pScene->mNumMaterials; i++)
		{
			aiMaterial* aimaterial = pScene->mMaterials[i];
			Material material;
			material.Create(aimaterial);
			m_Materials.push_back(material);
		}
	}


	// Material 생성 
	void Material::Create(aiMaterial* _aimaterial)
	{
		m_Name = _aimaterial->GetName().C_Str();

		aiColor3D color(0.f, 0.f, 0.f);
		_aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		m_BaseColor = Vector3(color.r, color.g, color.b);
		_aimaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		m_EmissiveColor = Vector3(color.r, color.g, color.b);

		

	}
}
