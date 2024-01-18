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

	private:
		void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

	private:
		ComPtr<ID3D11Device> m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;

		ComPtr<ID3D11RasterizerState> m_RasterizerState;
		ComPtr<ID3D11SamplerState> m_SamplerState;
		ComPtr<ID3D11BlendState> m_BlendState;
	};
}