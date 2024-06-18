#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <dxgi1_4.h>

namespace GraphicsEngine
{
	class ImGuiTool
	{
	public:
		ImGuiTool();
		~ImGuiTool();

	public:
		inline void SetInt(std::string _name, int* _pInt, int _min, int _max);
		inline void SetInt2(std::string _name, int* _pInt, int _min, int _max);
		inline void SetInt3(std::string _name, int* _pInt, int _min, int _max);
		inline void SetFloat(std::string _name, float* _pfloat, float _min, float _max);
		inline void SetFloat2(std::string _name, float* _pfloat, float _min, float _max);
		inline void SetFloat3(std::string _name, float* _pfloat, float _min, float _max);
		inline void SetFloat4(std::string _name, float* _pfloat, float _min, float _max);
		inline void SetBool(std::string _name, bool* _pbool);
		inline void SetString(std::string* _text);
		inline void DeleteInt(std::string _name);
		inline void DeleteInt2(std::string _name);
		inline void DeleteInt3(std::string _name);
		inline void DeleteFloat(std::string _name);
		inline void DeleteFloat2(std::string _name);
		inline void DeleteFloat3(std::string _name);
		inline void DeleteFloat4(std::string _name);
		inline void DeleteBool(std::string _name);

	public:
		void Init(HWND _hwnd, ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _devicecontext);
		void Render();

	private:
		void getVideoMemoryInfo(std::string& out);
		void getSystemMemoryInfo(std::string& out);

	private:
		map<string, tuple<int*, int, int>>		m_IntMap;
		map<string, tuple<int*, int, int>>		m_Int2Map;
		map<string, tuple<int*, int, int>>		m_Int3Map;
		map<string, tuple<float*, float, float>>	m_FloatMap;
		map<string, tuple<float*, float, float>>	m_Float2Map;
		map<string, tuple<float*, float, float>>	m_Float3Map;
		map<string, tuple<float*, float, float>>	m_Float4Map;
		map<string, bool*>		m_BoolMap;
		vector<string*>						m_TextVec;

		IDXGIFactory4* m_DXGIFactory;		// DXGI팩토리
		IDXGIAdapter3* m_DXGIAdapter;		// 비디오카드 정보에 접근 가능한 인터페이스
	};

	void ImGuiTool::SetInt(std::string _name, int* _pInt, int _min, int _max)
	{
		m_IntMap.emplace(make_pair(_name, make_tuple(_pInt, _min, _max)));
	}
	void ImGuiTool::SetInt2(std::string _name, int* _pInt, int _min, int _max)
	{
		m_Int2Map.emplace(make_pair(_name, make_tuple(_pInt, _min, _max)));
	}
	void ImGuiTool::SetInt3(std::string _name, int* _pInt, int _min, int _max)
	{
		m_Int3Map.emplace(make_pair(_name, make_tuple(_pInt, _min, _max)));
	}
	void ImGuiTool::SetFloat(std::string _name, float* _pfloat, float _min, float _max)
	{
		m_FloatMap.emplace(make_pair(_name, make_tuple(_pfloat, _min, _max)));
	}
	void ImGuiTool::SetFloat2(std::string _name, float* _pfloat, float _min, float _max)
	{
		m_Float2Map.emplace(make_pair(_name, make_tuple(_pfloat, _min, _max)));
	}
	void ImGuiTool::SetFloat3(std::string _name, float* _pfloat, float _min, float _max)
	{
		m_Float3Map.emplace(make_pair(_name, make_tuple(_pfloat, _min, _max)));
	}
	void ImGuiTool::SetFloat4(std::string _name, float* _pfloat, float _min, float _max)
	{
		m_Float4Map.emplace(make_pair(_name, make_tuple(_pfloat, _min, _max)));
	}
	void ImGuiTool::SetString(std::string* _text)
	{
		m_TextVec.push_back(_text);
	}
	void ImGuiTool::SetBool(std::string _name, bool* _pbool)
	{
		m_BoolMap.emplace(make_pair(_name, _pbool));
	}

	void ImGuiTool::DeleteInt(std::string _name)
	{
		m_IntMap.erase(m_IntMap.find(_name));
	}
	void ImGuiTool::DeleteInt2(std::string _name)
	{
		m_Int2Map.erase(m_Int2Map.find(_name));
	}
	void ImGuiTool::DeleteInt3(std::string _name)
	{
		m_Int3Map.erase(m_Int3Map.find(_name));
	}
	void ImGuiTool::DeleteFloat(std::string _name)
	{
		m_FloatMap.erase(m_FloatMap.find(_name));
	}
	void ImGuiTool::DeleteFloat2(std::string _name)
	{
		m_Float2Map.erase(m_Float2Map.find(_name));
	}
	void ImGuiTool::DeleteFloat3(std::string _name)
	{
		m_Float3Map.erase(m_Float3Map.find(_name));
	}
	void ImGuiTool::DeleteFloat4(std::string _name)
	{
		m_Float4Map.erase(m_Float4Map.find(_name));
	}
	void ImGuiTool::DeleteBool(std::string _name)
	{
		m_BoolMap.erase(m_BoolMap.find(_name));
	}
}
