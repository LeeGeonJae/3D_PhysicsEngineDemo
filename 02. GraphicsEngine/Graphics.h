#pragma once

#include <directxtk/SimpleMath.h>


namespace GraphicsEngine
{
	struct GraphicsInfo
	{
		HWND m_hwnd;
		float m_Width = 800.f;
		float m_Height = 600.f;
		DirectX::SimpleMath::Vector4 m_ClearColor = DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 0.5f);
	};

	class PipeLine;

	class Graphics
	{
	public:
		Graphics();
		~Graphics();

	public:
		void Initalize(GraphicsInfo info);

		void RenderBegin();
		void RenderEnd();

		void setViewPort();
	public:
		inline ComPtr<ID3D11Device> GetDevice();
		inline ComPtr<ID3D11DeviceContext> GetDeviceContext();
		inline float GetHieght();
		inline float GetWidth();

		std::weak_ptr<PipeLine> m_pPipeLine;

	private:
		HWND m_Hwnd;
		float m_Width;
		float m_Height;
		DirectX::SimpleMath::Vector4 m_ClearColor;

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
	float Graphics::GetHieght()
	{
		return m_Height;
	}
	float Graphics::GetWidth()
	{
		return m_Width;
	}
}
