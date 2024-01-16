#include "pch.h"
#include "Texture.h"

#include "RenderManager.h"
#include "Graphics.h"

#include <Directxtk/WICTextureLoader.h>

namespace Engine
{
	Texture::Texture()
		:ResourceBase(ResourceType::Texture)
	{
	}

	Texture::~Texture()
	{
	}

	void Texture::LoadTexture(string _path)
	{
		HRESULT hr;
		DirectX::TexMetadata md;
		DirectX::ScratchImage img;

		wstring fileNames = wstring(_path.begin(), _path.end());

		if (fileNames.find(L".tga") != wstring::npos)
		{
			hr = ::LoadFromTGAFile(fileNames.c_str(), TGA_FLAGS_NONE, &md, img);
			assert(SUCCEEDED(hr));
		}
		else if (fileNames.find(L".dds") != wstring::npos)
		{
			hr = ::LoadFromDDSFile(fileNames.c_str(), DDS_FLAGS_NONE, &md, img);
			assert(SUCCEEDED(hr));
		}
		else
		{
			hr = ::LoadFromWICFile(fileNames.c_str(), WIC_FLAGS_NONE, &md, img);
			assert(SUCCEEDED(hr));
		}

		hr = ::CreateShaderResourceView(RENDER->GetGraphics()->GetDevice().Get(), img.GetImages(), img.GetImageCount(), md, m_Texture.GetAddressOf());
	}

	void Texture::LoadEmbeddedTexture(const aiTexture* _embeddedTexture)
	{
		HRESULT hr;

		// 내장 텍스처의 높이가 0이 아닌 경우(즉, 2D 텍스처인 경우) 처리합니다.
		if (_embeddedTexture->mHeight != 0)
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = _embeddedTexture->mWidth;			// 텍스처의 너비
			desc.Height = _embeddedTexture->mHeight;			// 텍스처의 높이
			desc.MipLevels = 1;								// 미입 레벨 수
			desc.ArraySize = 1;								// 배열 크기
			desc.SampleDesc.Count = 1;						// 샘플링 수
			desc.SampleDesc.Quality = 0;					// 샘플링 품질
			desc.Usage = D3D11_USAGE_DEFAULT;				// 사용 방식 (기본 사용)
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// 텍스처 형식 (8비트 Red, Green, Blue 및 8비트 Alpha 채널)
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// 바인딩 플래그 (Shader Resource로 바인딩)
			desc.CPUAccessFlags = 0;						// CPU 엑세스 플래그 (없음)
			desc.MiscFlags = 0;								// 기타 플래그 (없음)

			// 텍스처 데이터를 저장하는 서브리소스 데이터 구조체를 생성하고 초기화합니다.
			D3D11_SUBRESOURCE_DATA subresourceData;
			subresourceData.pSysMem = _embeddedTexture->pcData;  // 텍스처 데이터의 포인터
			subresourceData.SysMemPitch = _embeddedTexture->mWidth * 4;  // 텍스처의 한 행 크기 (4바이트 = RGBA 4개)
			subresourceData.SysMemSlicePitch = _embeddedTexture->mWidth * _embeddedTexture->mHeight * 4;  // 전체 텍스처 데이터 크기

			ComPtr<ID3D11Texture2D> texture2D = nullptr;
			hr = DEVICE->CreateTexture2D(&desc, &subresourceData, texture2D.GetAddressOf());
			assert(SUCCEEDED(hr));

			hr = DEVICE->CreateShaderResourceView(texture2D.Get(), nullptr, m_Texture.GetAddressOf());
			assert(SUCCEEDED(hr));
		}

		const size_t size = _embeddedTexture->mWidth;

		hr = DirectX::CreateWICTextureFromMemory(DEVICE.Get(), DEVICE_CONTEXT.Get(), reinterpret_cast<const unsigned char*>(_embeddedTexture->pcData), size, nullptr, m_Texture.GetAddressOf());
	}
}