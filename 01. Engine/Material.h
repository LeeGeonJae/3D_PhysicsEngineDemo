#pragma once
#include "ResourceBase.h"
#include "Texture.h"

namespace Engine
{
    struct Material
    {
        string m_Name;
        shared_ptr<Texture> m_Texture[static_cast<int>(TextureType::END)];
    };


    class MaterialResource : public ResourceBase
    {
    public:
        MaterialResource();
        virtual ~MaterialResource();

    public:
        void Create(const aiMaterial* _pMaterial);

    private:
        string m_Name;
        vector<Material> m_Materials;
    };
}