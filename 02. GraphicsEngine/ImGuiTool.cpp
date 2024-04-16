#include "pch.h"
#include "ImGuiTool.h"

#include <dxgidebug.h>
#include <Psapi.h>
#include <directxtk/GeometricPrimitive.h>

#pragma comment(lib,"dxgi.lib")

namespace GraphicsEngine
{
	ImGuiTool::ImGuiTool()
	{

	}

	ImGuiTool::~ImGuiTool()
	{

	}

	void ImGuiTool::Init(HWND _hwnd, ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _devicecontext)
	{
		HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&m_DXGIFactory);
		hr = m_DXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&m_DXGIAdapter));

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(_hwnd);
		ImGui_ImplDX11_Init(_device.Get(), _devicecontext.Get());

		ImGui::SetNextWindowSize({ 500.f, 500.f });
	}

	void ImGuiTool::Render()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("ImGui");

			for (auto input : m_IntMap)
			{
				ImGui::SliderInt(input.first.c_str(), get<0>(input.second), get<1>(input.second), get<2>(input.second));
			}
			for (auto input : m_Int2Map)
			{
				ImGui::SliderInt2(input.first.c_str(), get<0>(input.second), get<1>(input.second), get<2>(input.second));
			}
			for (auto input : m_Int3Map)
			{
				ImGui::SliderInt3(input.first.c_str(), get<0>(input.second), get<1>(input.second), get<2>(input.second));
			}
			for (auto input : m_FloatMap)
			{
				ImGui::SliderFloat(input.first.c_str(), get<0>(input.second), get<1>(input.second), get<2>(input.second));
			}
			for (auto input : m_Float2Map)
			{
				ImGui::SliderFloat2(input.first.c_str(), get<0>(input.second), get<1>(input.second), get<2>(input.second));
			}
			for (auto input : m_Float3Map)
			{
				ImGui::SliderFloat3(input.first.c_str(), get<0>(input.second), get<1>(input.second), get<2>(input.second));
			}
			for (auto input : m_Float4Map)
			{
				ImGui::SliderFloat4(input.first.c_str(), get<0>(input.second), get<1>(input.second), get<2>(input.second));
			}
			for (auto input : m_BoolMap)
			{
				ImGui::Checkbox(input.first.c_str(), input.second);
			}
			for (auto input : m_TextVec)
			{
				ImGui::Text(input->c_str());
			}

			string str;
			getVideoMemoryInfo(str);
			ImGui::Text("VideoMemory: %s", str.c_str());
			getSystemMemoryInfo(str);
			ImGui::Text("SystemMemory: %s", str.c_str());

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiTool::getVideoMemoryInfo(std::string& out)
	{
		DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
		m_DXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);

		out = std::to_string(videoMemoryInfo.CurrentUsage / 1024 / 1024) + " MB" + "/" + std::to_string(videoMemoryInfo.Budget / 1024 / 1024) + " MB";
	}

	void ImGuiTool::getSystemMemoryInfo(std::string& out)
	{
		HANDLE hProcess = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS_EX pmc;
		pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
		GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		out = std::to_string((pmc.PagefileUsage) / 1024 / 1024) + " MB";
	}
}

