#include "stdafx.h"
#include "..\Public\ImGui_Manager.h"


IMPLEMENT_SINGLETON(CImGui_Manager)

CImGui_Manager::CImGui_Manager()
	: m_pGraphic_device(nullptr)
	, m_bShow_Demo_Window(false)
	, m_bShow_Another_Window(false)
{
}

HRESULT CImGui_Manager::Init(LPDIRECT3DDEVICE9 pGraphic_device)
{
	m_pGraphic_device = pGraphic_device;
	Safe_AddRef(m_pGraphic_device);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(pGraphic_device);

	// Our state
	m_bShow_Demo_Window = true;
	m_bShow_Another_Window = false;
	m_Clear_Color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	return S_OK;
}

void CImGui_Manager::Tick(_float fTimeDelta)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (m_bShow_Demo_Window)
		ImGui::ShowDemoWindow(&m_bShow_Demo_Window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &m_bShow_Demo_Window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &m_bShow_Another_Window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&m_Clear_Color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (m_bShow_Another_Window)
	{
		ImGui::Begin("Another Window", &m_bShow_Another_Window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			m_bShow_Another_Window = false;
		ImGui::End();
	}

	ImGui::EndFrame();
	// Handle loss of D3D9 device
	//if (result == D3DERR_DEVICELOST && pGraphic_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	//	ResetDevice();

}

void CImGui_Manager::Render()
{

	// Rendering
	m_pGraphic_device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	if (m_pGraphic_device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		m_pGraphic_device->EndScene();
	}

	m_pGraphic_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

	HRESULT result = m_pGraphic_device->Present(NULL, NULL, NULL, NULL);
}

void CImGui_Manager::Free()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	/*	CleanupDeviceD3D();
	::DestroyWindow(hWnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);*/

	Safe_Release(m_pGraphic_device);
}
