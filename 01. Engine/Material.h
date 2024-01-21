#pragma once
#include "ResourceBase.h"
#include "Texture.h"

namespace Engine
{
    // 건재 : 머터리얼 클래스
    class Material : public ResourceBase
    {
    public:
        Material();
        ~Material();

    public:
        void Create(aiMaterial* _material, const aiScene* _aiScene, const string& _path);

    private:
        shared_ptr<Texture> loadMaterialTexture(aiMaterial* _aiMaterial, aiTextureType _type, string _typeName, const aiScene* _aiScene, const string& _path);

    public:
        inline shared_ptr<Texture> GetTexture(TextureType _textureType);
        inline void SetTexture(TextureType _textureType, shared_ptr<Texture> _texture);
        inline Vector3 GetBaseColor();
        inline void SetBaseColor(Vector3 _color);
        inline Vector3 GetEmissiveColor();
        inline void SetEmissiveColor(Vector3 _color);


    private:
        Vector3 m_BaseColor;
        Vector3 m_EmissiveColor;
        std::shared_ptr<Texture> m_Textures[static_cast<int>(TextureType::END)];
    };


    shared_ptr<Texture> Material::GetTexture(TextureType _textureType)
    {
        return m_Textures[static_cast<int>(_textureType)];
    }
    void Material::SetTexture(TextureType _textureType, shared_ptr<Texture> _texture)
    {
        m_Textures[static_cast<int>(_textureType)] = _texture;
    }
    Vector3 Material::GetBaseColor()
    {
        return m_BaseColor;
    }
    void Material::SetBaseColor(Vector3 _color)
    {
        m_BaseColor = _color;
    }
    Vector3 Material::GetEmissiveColor()
    {
        return m_EmissiveColor;
    }
    void Material::SetEmissiveColor(Vector3 _color)
    {
        m_EmissiveColor = _color;
    }
    
}