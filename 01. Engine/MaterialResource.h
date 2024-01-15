#pragma once
#include "ResourceBase.h"
#include "Texture.h"

namespace Engine
{
    // 머터리얼 구조체
    class Material
    {
    public:
        Material() {}
        ~Material() {}

    public:
        void Create(aiMaterial* _material);

    public:
        inline string GetName();
        inline void SetName(string _name);
        inline shared_ptr<Texture> GetTexture(TextureType _textureType);
        inline void SetTexture(TextureType _textureType, shared_ptr<Texture> _texture);
        inline Vector3 GetBaseColor();
        inline Vector3 GetEmissiveColor();


    private:
        string m_Name;

        Vector3 m_BaseColor;
        Vector3 m_EmissiveColor;
        shared_ptr<Texture> m_Texture[static_cast<int>(TextureType::END)];
    };

    string Material::GetName()
    {
        return m_Name;
    }
    void Material::SetName(string _name)
    {
        m_Name = _name;
    }
    shared_ptr<Texture> Material::GetTexture(TextureType _textureType)
    {
        return m_Texture[static_cast<int>(_textureType)];
    }
    void Material::SetTexture(TextureType _textureType, shared_ptr<Texture> _texture)
    {
        m_Texture[static_cast<int>(_textureType)] = _texture;
    }
    Vector3 Material::GetBaseColor()
    {
        return m_BaseColor;
    }
    Vector3 Material::GetEmissiveColor()
    {
        return m_EmissiveColor;
    }


    // 머터리얼 리소스 구조체
    class MaterialSceneResource : public ResourceBase
    {
    public:
        MaterialSceneResource();
        virtual ~MaterialSceneResource();

    public:
        void Create(const string _path) override;

    public:
        inline vector<Material> GetMaterials();
        inline void AddMaterial(Material _material);

    private:
        vector<Material> m_Materials;
    };

    vector<Material> MaterialSceneResource::GetMaterials()
    {
        return m_Materials;
    }
    void MaterialSceneResource::AddMaterial(Material _material)
    {
        m_Materials.push_back(_material);
    }
}