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

	void PipeLine::Initalize(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext)
	{
		m_Device = _device;
		m_DeviceContext = _deviceContext;

		createRasterizerState();
		createBlendState();
		createSamplerState();

		createVS();
		createPS();
		createInputLayout();
	}

	void PipeLine::Update()
	{
		m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_DeviceContext->RSSetState(m_RasterizerState.Get());
		m_DeviceContext->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());

		m_DeviceContext->IASetInputLayout(m_InputLayout.Get());
		m_DeviceContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);
		m_DeviceContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	}

	void PipeLine::createVS()
	{
		LoadShaderFromFile(L"../Resources/Shader/Defalut.hlsl", "VS", "vs_5_0", m_vsBlob);
		HRESULT hr = m_Device->CreateVertexShader(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	void PipeLine::createPS()
	{
		LoadShaderFromFile(L"../Resources/Shader/Defalut.hlsl", "PS", "ps_5_0", m_psBlob);
		HRESULT hr = m_Device->CreatePixelShader(m_psBlob->GetBufferPointer(), m_psBlob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	void PipeLine::createInputLayout()
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
		m_Device->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
	}

	void PipeLine::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
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
	}

	void PipeLine::createRasterizerState()
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.FrontCounterClockwise = false;		// 정점이 시계방향으로 돌면서 삼각형을 그리면 정면이 앞부분으로 간다. true면 정면이 반대방향을 향한다.

		HRESULT hr = m_Device->CreateRasterizerState(&desc, m_RasterizerState.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	void PipeLine::createBlendState()
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = m_Device->CreateBlendState(&desc, m_BlendState.GetAddressOf());
		assert(SUCCEEDED(hr));
	}

	void PipeLine::createSamplerState()
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

		HRESULT hr = m_Device->CreateSamplerState(&desc, m_SamplerState.GetAddressOf());
		assert(SUCCEEDED(hr));
	}
}