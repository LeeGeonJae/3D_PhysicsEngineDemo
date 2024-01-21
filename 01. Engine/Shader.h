#pragma once
#include "ResourceBase.h"
#include "SkeletalMeshResource.h"
#include "StaticMeshResource.h"

namespace Engine
{
	// ���� : ���̴� Ÿ��
	enum class ShaderType
	{
		SkeletalMesh = 0,
		StaticMesh,


		End,
	};

	// ���� : ���̴� Ŭ����
	class Shader : public ResourceBase
	{
	public:
		Shader();
		virtual ~Shader();

	public:
		void Init(const wstring& _path, ShaderType _type);

	public:
		inline ComPtr<ID3D11InputLayout> GetInputLayout();
		inline ComPtr<ID3D11VertexShader> GetVertexShader();
		inline ComPtr<ID3D11PixelShader> GetPixelShader();

	private:
		void createVS(const wstring& _path);
		void createPS(const wstring& _path);
		void createInputLayout();
		void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

	public:
		ShaderType m_ShaderType;

		// ���� : Geometry
		ComPtr<ID3D11InputLayout> m_InputLayout = nullptr;
		// ���� : VS
		ComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
		ComPtr<ID3DBlob> m_vsBlob = nullptr;
		// ���� : PS
		ComPtr<ID3D11PixelShader> m_PixelShader = nullptr;
		ComPtr<ID3DBlob> m_psBlob = nullptr;
	};

	ComPtr<ID3D11InputLayout> Shader::GetInputLayout()
	{
		return m_InputLayout;
	}
	ComPtr<ID3D11VertexShader> Shader::GetVertexShader()
	{
		return m_VertexShader;
	}
	ComPtr<ID3D11PixelShader> Shader::GetPixelShader()
	{
		return m_PixelShader;
	}
}

