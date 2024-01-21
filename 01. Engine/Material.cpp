#include "pch.h"
#include "Material.h"
#include "ResourceManager.h"

namespace Engine
{
	Material::Material()
		:ResourceBase(ResourceType::Material)
	{
	}

	Material::~Material()
	{
	}

	// ���� : Material ���� 
	void Material::Create(aiMaterial* _aimaterial, const aiScene* _aiScene, const string& _path)
	{
		m_Name = _aimaterial->GetName().C_Str();

		aiColor3D color(0.f, 0.f, 0.f);
		_aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		m_BaseColor = Vector3(color.r, color.g, color.b);
		_aimaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		m_EmissiveColor = Vector3(color.r, color.g, color.b);

		shared_ptr<Texture> diffuse = loadMaterialTexture(_aimaterial, aiTextureType_DIFFUSE, "texture_diffuse", _aiScene, _path);
		if (diffuse != nullptr)
		{
			//m_BaseColor = { 0,0,0 };
			diffuse->SetTextureType(TextureType::DIFFUSE);
			m_Textures[static_cast<int>(TextureType::DIFFUSE)] = diffuse;
		}
		shared_ptr<Texture> normal = loadMaterialTexture(_aimaterial, aiTextureType_NORMALS, "texture_normals", _aiScene, _path);
		if (normal != nullptr)
		{
			normal->SetTextureType(TextureType::NORMAL);
			m_Textures[static_cast<int>(TextureType::NORMAL)] = normal;
		}
		shared_ptr<Texture> specular = loadMaterialTexture(_aimaterial, aiTextureType_SPECULAR, "texture_specular", _aiScene, _path);
		if (specular != nullptr)
		{
			specular->SetTextureType(TextureType::SPECULAR);
			m_Textures[static_cast<int>(TextureType::SPECULAR)] = specular;
		}
		shared_ptr<Texture> opacity = loadMaterialTexture(_aimaterial, aiTextureType_OPACITY, "texture_opacity", _aiScene, _path);
		if (opacity != nullptr)
		{
			opacity->SetTextureType(TextureType::OPACITY);
			m_Textures[static_cast<int>(TextureType::OPACITY)] = opacity;
		}
		shared_ptr<Texture> emissive = loadMaterialTexture(_aimaterial, aiTextureType_EMISSIVE, "texture_emissive", _aiScene, _path);
		if (emissive != nullptr)
		{
			emissive->SetTextureType(TextureType::EMISSIVE);
			m_Textures[static_cast<int>(TextureType::EMISSIVE)] = emissive;
		}
		shared_ptr<Texture> metalness = loadMaterialTexture(_aimaterial, aiTextureType_METALNESS, "texture_matalness", _aiScene, _path);
		if (metalness != nullptr)
		{
			metalness->SetTextureType(TextureType::METALNESS);
			m_Textures[static_cast<int>(TextureType::METALNESS)] = metalness;
		}
		shared_ptr<Texture> roughness = loadMaterialTexture(_aimaterial, aiTextureType_SHININESS, "texture_roughness", _aiScene, _path);
		if (roughness != nullptr)
		{
			roughness->SetTextureType(TextureType::ROUGHNESS);
			m_Textures[static_cast<int>(TextureType::ROUGHNESS)] = roughness;
		}

	}

	shared_ptr<Texture> Material::loadMaterialTexture(aiMaterial* _aiMaterial, aiTextureType _type, string _typeName, const aiScene* _aiScene, const string& _path)
	{
		shared_ptr<Texture> texture = nullptr;

		// ���� : �ش� ��Ƽ���� �ش� �ؽ��� Ÿ���� �ؽ��İ� ���� ���
		if (_aiMaterial->GetTextureCount(_type) > 0)
		{
			aiString str;
			_aiMaterial->GetTexture(_type, 0, &str);

			// ���� : ������ �ؽ��� ������ ��� ������ ��ȯ
			string strtemp = str.C_Str();
			if (strtemp.find('\\') != string::npos)
				strtemp.erase(strtemp.begin(), strtemp.begin() + strtemp.find_last_of('\\') + 1);
			string textureName = "../Resources/Texture/" + strtemp;
			texture = RESOURCE->Find<Texture>(textureName);
			if (texture != nullptr)
				return texture;

			// ���� : ������ �ؽ��� ������ ��� ������ ��ȯ
			textureName = _path + '/' + _typeName;
			texture = RESOURCE->Find<Texture>(textureName);
			if (texture != nullptr)
				return texture;

			// ���� : ��ã���� �ؽ��� ����
			const aiTexture* embeddedTexture = _aiScene->GetEmbeddedTexture(str.C_Str());
			// ���� : ���� �ؽ��ĸ� ����� ���
			if (embeddedTexture != nullptr)
			{
				textureName = _path + '/' + _typeName;
				texture = RESOURCE->Create<Texture>(textureName);
				texture->LoadEmbeddedTexture(embeddedTexture);
			}
			else // ���� : ���Ͽ��� �ؽ��ĸ� �ε��ؾ� �ϴ� ���
			{
				textureName = "../Resources/Texture/" + strtemp;
				texture = RESOURCE->Create<Texture>(textureName);
				texture->LoadTexture(textureName);
			}
		}

		return texture;
	}
}
