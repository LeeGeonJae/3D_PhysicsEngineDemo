#pragma once

namespace GraphicsEngine
{
	struct PipelineInfo
	{

	};

	class PipeLine
	{
	public:
		PipeLine();
		~PipeLine();

	public:
		void Initalize(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext);
		void Update();

	private:
		void createRasterizerState();
		void createBlendState();
		void createSamplerState();
		void createVS();
		void createPS();
		void createInputLayout();

	private:
		void LoadShaderFromFile(const string& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

	private:
		ComPtr<ID3D11Device> m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;

		ComPtr<ID3D11RasterizerState> m_RasterizerState;
		ComPtr<ID3D11SamplerState> m_SamplerState;
		ComPtr<ID3D11BlendState> m_BlendState;

		ComPtr<ID3D11InputLayout> m_InputLayout;

		ComPtr<ID3D11VertexShader> m_VertexShader;
		ComPtr<ID3DBlob> m_vsBlob;

		ComPtr<ID3D11PixelShader> m_PixelShader;
		ComPtr<ID3DBlob> m_psBlob;
	};
}