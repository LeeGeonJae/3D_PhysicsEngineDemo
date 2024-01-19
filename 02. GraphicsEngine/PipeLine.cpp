#include "pch.h"
#include "PipeLine.h"

namespace GraphicsEngine
{
	PipeLine::PipeLine()
	{
	}

	PipeLine::~PipeLine()
	{
	}

	void PipeLine::Initalize(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext, UINT _textureNum)
	{
		m_Device = _device;
		m_DeviceContext = _deviceContext;

		m_pTexture.resize(_textureNum);

#pragma region CreateDefault
		{
			{
				D3D11_RASTERIZER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.FillMode = D3D11_FILL_SOLID;
				desc.CullMode = D3D11_CULL_NONE;
				desc.FrontCounterClockwise = false;		// 정점이 시계방향으로 돌면서 삼각형을 그리면 정면이 앞부분으로 간다. true면 정면이 반대방향을 향한다.

				HRESULT hr = m_Device->CreateRasterizerState(&desc, m_DefaultRasterizerState.GetAddressOf());
				assert(SUCCEEDED(hr));
			}
			{
				D3D11_SAMPLER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				desc.MaxLOD = 0;
				desc.MaxLOD = D3D11_FLOAT32_MAX;

				HRESULT hr = m_Device->CreateSamplerState(&desc, m_DefaultSamplerState.GetAddressOf());
				assert(SUCCEEDED(hr));
			}
			{
				D3D11_BLEND_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
				desc.AlphaToCoverageEnable = false;
				desc.IndependentBlendEnable = false;

				desc.RenderTarget[0].BlendEnable = false;
				desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
				desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
				desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

				HRESULT hr = m_Device->CreateBlendState(&desc, m_DefaultBlendState.GetAddressOf());
				assert(SUCCEEDED(hr));
			}
		}
#pragma endregion

	}

	void PipeLine::Update()
	{

	}

	void PipeLine::StateSetDefault()
	{
		SetRasterizerState(m_DefaultRasterizerState);
		SetSamplerState(m_DefaultSamplerState);
		SetBlendState(m_DefaultBlendState);
	}

	

	void PipeLine::SetSamplerState(ComPtr<ID3D11SamplerState> _SS)
	{
		if (m_pSamplerState != _SS.Get())
		{
			m_pSamplerState = _SS.Get();
			m_DeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
		}
	}

	void PipeLine::SetBlendState(ComPtr<ID3D11BlendState> _BS)
	{
		if (m_pBlendState != _BS.Get())
		{
			m_pBlendState = _BS.Get();
			m_DeviceContext->OMSetBlendState(m_pBlendState,NULL,0xffff);

		}
	}

	/*void PipeLine::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
	{
		const unsigned int compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

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
	}*/
}