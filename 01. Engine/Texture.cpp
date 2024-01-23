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

		hr = ::CreateShaderResourceView(DEVICE.Get(), img.GetImages(), img.GetImageCount(), md, m_Texture.GetAddressOf());
	}

	void Texture::LoadEmbeddedTexture(const aiTexture* _embeddedTexture)
	{
		HRESULT hr;

		// ���� : ���� �ؽ�ó�� ���̰� 0�� �ƴ� ���(��, 2D �ؽ�ó�� ���) ó���մϴ�.
		if (_embeddedTexture->mHeight != 0)
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = _embeddedTexture->mWidth;			// ���� : �ؽ�ó�� �ʺ�
			desc.Height = _embeddedTexture->mHeight;		// ���� : �ؽ�ó�� ����
			desc.MipLevels = 1;								// ���� : ���� ���� ��
			desc.ArraySize = 1;								// ���� : �迭 ũ��
			desc.SampleDesc.Count = 1;						// ���� : ���ø� ��
			desc.SampleDesc.Quality = 0;					// ���� : ���ø� ǰ��
			desc.Usage = D3D11_USAGE_DEFAULT;				// ���� : ��� ��� (�⺻ ���)
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// ���� : �ؽ�ó ���� (8��Ʈ Red, Green, Blue �� 8��Ʈ Alpha ä��)
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// ���� : ���ε� �÷��� (Shader Resource�� ���ε�)
			desc.CPUAccessFlags = 0;						// ���� : CPU ������ �÷��� (����)
			desc.MiscFlags = 0;								// ���� : ��Ÿ �÷��� (����)

			// ���� : �ؽ�ó �����͸� �����ϴ� ���긮�ҽ� ������ ����ü�� �����ϰ� �ʱ�ȭ�մϴ�.
			D3D11_SUBRESOURCE_DATA subresourceData;
			subresourceData.pSysMem = _embeddedTexture->pcData;  // ���� : �ؽ�ó �������� ������
			subresourceData.SysMemPitch = _embeddedTexture->mWidth * 4;  // ���� : �ؽ�ó�� �� �� ũ�� (4����Ʈ = RGBA 4��)
			subresourceData.SysMemSlicePitch = _embeddedTexture->mWidth * _embeddedTexture->mHeight * 4;  // ���� : ��ü �ؽ�ó ������ ũ��

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