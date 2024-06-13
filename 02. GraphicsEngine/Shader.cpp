#include "pch.h"
#include "Shader.h"

#include "RenderManager.h"
#include "Graphics.h"

namespace GraphicsEngine
{
	Shader::Shader() 
		: ResourceBase(ResourceType::Shader)
	{
	}
	Shader::~Shader()
	{
	}

	void Shader::Init(const wstring& _path, ShaderType _type)
	{
		m_ShaderType = _type;

		createVS(_path);
		createPS(_path);
		createInputLayout();
	}

	void Shader::createVS(const wstring& _path)
	{
		// 건재 : 쉐이더 파일 지정 및 시작 함수 지정
		LoadShaderFromFile(_path, "VS", "vs_5_0", m_vsBlob);
		HRESULT hr = DEVICE->CreateVertexShader(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	void Shader::createPS(const wstring& _path)
	{
		// 건재 : 쉐이더 파일 지정 및 시작 함수 지정
		LoadShaderFromFile(_path, "PS", "ps_5_0", m_psBlob);
		HRESULT hr = DEVICE->CreatePixelShader(m_psBlob->GetBufferPointer(), m_psBlob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	void Shader::createInputLayout()
	{
		// 건재 : 쉐이더 타입(스켈레탈 or 스태틱)에 따라 인풋 레이아웃 생성
		switch (m_ShaderType)
		{
		case ShaderType::SkeletalMesh:
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			const int count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
			DEVICE->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
		}
		break;
		case ShaderType::StaticMesh:
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			const int count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
			DEVICE->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
		}
		break;
		case ShaderType::Physics:
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			const int count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
			DEVICE->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
		}
		break;
		}
	}

	void Shader::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
	{
		// 건재 : 쉐이더 파일 컴파일
		const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

		HRESULT hr = ::D3DCompileFromFile(
			path.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			name.c_str(),
			version.c_str(),
			compileFlag,
			0,
			blob.GetAddressOf(),
			nullptr);

		assert(SUCCEEDED(hr));
	}
}

