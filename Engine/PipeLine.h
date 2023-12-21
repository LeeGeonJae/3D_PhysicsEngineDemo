#pragma once
class PipeLine
{
public:
	PipeLine();
	~PipeLine();

public:
	void Init(ComPtr<ID3D11Device> _device);

private:
	void createInputLayout();

private:
	ComPtr<ID3D11InputLayout> m_InputLayout;

	ComPtr<ID3D11RasterizerState> m_RasterizerState;

	ComPtr<ID3D11SamplerState> m_SamplerState;
	ComPtr<ID3D11BlendState> m_BlendState;
};

