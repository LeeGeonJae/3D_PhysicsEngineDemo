#pragma once
#include "ResourceBase.h"

namespace Engine
{
	enum class TextureType
	{
		BASECOLOR = 0,
		DIFFUSE,
		NORMAL,
		SPECULAR,
		OPACITY,
		EMISSIVE,
		METALNESS,
		ROUGHNESS,

		END
	};



    class Texture :
        public ResourceBase
    {
    public:
        Texture();
        virtual ~Texture();

	public:
		void Create(string _path) override;

	public:
		inline void SetTextureType(TextureType _type);
		inline TextureType GetTextureType();

	private:
		TextureType m_TextureType;
    };



	void Texture::SetTextureType(TextureType _type)
	{
		m_TextureType = _type;
	}
	TextureType Engine::Texture::GetTextureType()
	{
		return m_TextureType;
	}
}