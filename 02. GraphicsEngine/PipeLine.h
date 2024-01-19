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

		void StateSetDefault();
	public:

		void SetRasterizerState(ComPtr<ID3D11RasterizerState> _RS);
		void SetSamplerState(ComPtr<ID3D11SamplerState> _SS);
		void SetBlendState(ComPtr<ID3D11BlendState> _BS);



	private:
		void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

	private:
		ComPtr<ID3D11Device> m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;


		//default state
		ComPtr<ID3D11RasterizerState>	m_DefaultRasterizerState;
		ComPtr<ID3D11SamplerState>		m_DefaultSamplerState;
		ComPtr<ID3D11BlendState>		m_DefaultBlendState;

	private:
		ID3D11RasterizerState* m_pRasterizerState = nullptr;
		ID3D11SamplerState* m_pSamplerState = nullptr;
		ID3D11BlendState* m_pBlendState = nullptr;
	};
}