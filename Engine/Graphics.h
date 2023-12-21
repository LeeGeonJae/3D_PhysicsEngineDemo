#pragma once

class Graphics
{
	SINGLETON(Graphics);
public:
	void Init(HWND hwnd);

	void RenderBegin();
	void RenderEnd();

private:
	void createDeviceAndSwapChain();
	void createRenderTargetView();
	void createDepthStencilView();
	void setViewPort();

public:
	inline ComPtr<ID3D11Device> GetDevice();
	inline ComPtr<ID3D11DeviceContext> GetDeviceContext();

private:
	HWND m_Hwnd;

	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	ComPtr<IDXGISwapChain> m_SwapChain;

	ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

	D3D11_VIEWPORT m_Viewport;
};

ComPtr<ID3D11Device> Graphics::GetDevice()
{
	return m_Device;
}
ComPtr<ID3D11DeviceContext> Graphics::GetDeviceContext()
{
	return m_DeviceContext;
}