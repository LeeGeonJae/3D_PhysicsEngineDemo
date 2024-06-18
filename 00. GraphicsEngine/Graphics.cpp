#include "pch.h"
#include "Graphics.h"
#include "PipeLine.h"
#include <cassert>


namespace GraphicsEngine
{
	Graphics::Graphics()
	{
	}

	Graphics::~Graphics()
	{
	}

	void Graphics::Initalize(GraphicsInfo info)
	{
		m_Hwnd = info.m_hwnd;
		m_Width = info.m_Width;
		m_Height = info.m_Height;
		m_ClearColor = info.m_ClearColor;

		HRESULT hr = 0;

		// ����ü�� �Ӽ� ���� ����ü�� �ʱ�ȭ�մϴ�.
		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(swapDesc));
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �� ������ �ȼ� ������ �����մϴ�.
		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapDesc.BufferDesc.Width = m_Width;				// �� ������ �ʺ� �����մϴ�.
		swapDesc.BufferDesc.Height = m_Height;				// �� ������ ���̸� �����մϴ�.
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;				// ȭ�� �ֻ����� ���ڸ� �����մϴ�.
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;			// ȭ�� �ֻ����� �и� �����մϴ�.
		swapDesc.SampleDesc.Count = 1;								// ��Ƽ ���ø� ���� �����մϴ�.
		swapDesc.SampleDesc.Quality = 0;							// ��Ƽ ���ø� ǰ���� �����մϴ�.
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;
		swapDesc.OutputWindow = m_Hwnd;
		swapDesc.Windowed = true;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


		// 1. ����̽� ����. 2. ����ü�� ����. 3. ����̽� ���ؽ�Ʈ ����.
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swapDesc,
			m_SwapChain.GetAddressOf(),
			m_Device.GetAddressOf(),
			nullptr,
			m_DeviceContext.GetAddressOf());

		assert(SUCCEEDED(hr));

		// 4. ����Ÿ�ٺ� ���� (����۸� �̿��ϴ� ����Ÿ�ٺ�)
		ID3D11Texture2D* pBackBufferTexture = nullptr;
		hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);
		assert(SUCCEEDED(hr));

		hr = m_Device->CreateRenderTargetView(pBackBufferTexture, NULL, m_RenderTargetView.GetAddressOf()); // �� ���� �ؽ�ó�� ���� Ÿ�� ��� �����մϴ�.
		assert(SUCCEEDED(hr));

		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
		descDepth.Width = m_Width;
		descDepth.Height = m_Height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		ID3D11Texture2D* textureDepthStancil = nullptr;
		hr = m_Device->CreateTexture2D(&descDepth, nullptr, &textureDepthStancil);
		assert(SUCCEEDED(hr));

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		hr = m_Device->CreateDepthStencilView(textureDepthStancil, &descDSV, m_DepthStencilView.GetAddressOf());
		assert(SUCCEEDED(hr));

		assert(textureDepthStancil != nullptr);
		textureDepthStancil->Release();

		setViewPort();
	}

	void Graphics::RenderBegin()
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), (float*)(&m_ClearColor));
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
		m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_DeviceContext->RSSetViewports(1, &m_Viewport);
		m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	}

	void Graphics::RenderEnd()
	{
		HRESULT hr = m_SwapChain->Present(0, 0);
		assert(SUCCEEDED(hr));
	}


	void Graphics::setViewPort()
	{
		m_Viewport.TopLeftX = 0.f;
		m_Viewport.TopLeftY = 0.f;
		m_Viewport.Width = static_cast<float>(m_Width);
		m_Viewport.Height = static_cast<float>(m_Height);
		m_Viewport.MinDepth = 0.f;
		m_Viewport.MaxDepth = 1.f;
	}
}