#include "pch.h"
#include "ImguiManager.h"
#include "DeviceManager.h"

IMPLEMENT_SINGLETON(CImguiManager)

HRESULT CImguiManager::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2((_float)Tool::g_iWidth, (_float)Tool::g_iHeight);
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
	// io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplDX11_Init(m_pDevice, m_pDeviceContext);
	ImGui_ImplWin32_Init(g_hWnd);

	return S_OK;
}

void CImguiManager::UpdateBegin()
{
	ImGui_ImplDX11_NewFrame();//TODO: IMGUI »ç¿ë
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
}

void CImguiManager::UpdateEnd()
{
	ImGui::EndFrame();
}

void CImguiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CImguiManager::Release()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	m_upInstance.reset();
}
