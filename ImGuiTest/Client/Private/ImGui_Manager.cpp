#include "stdafx.h"
#include "..\Public\ImGui_Manager.h"
#include "CubeManager.h"

#include "Texture.h"

IMPLEMENT_SINGLETON(CImGui_Manager)

CImGui_Manager::CImGui_Manager()
	: m_pGraphic_device(nullptr)
	, m_bShow_Demo_Window(false)
	, m_bShow_Transform_Window(true)
	, m_bShow_Block_Window(false)
	, m_bShow_Voxel_Window(false)
	, m_FileName("")
{
	ZeroMemory(&m_Block_Info, sizeof(BLOCK_INFO));
	ZeroMemory(&m_vModelPos, sizeof(_float3));
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
	m_bShow_Demo_Window = false;
	m_bShow_Transform_Window = false;
	// m_Clear_Color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


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

	// TransformWindow
	{
		ImGui::Begin("Tool Box");                          // Create a window called "Hello, world!" and append into it.

		
		ImGui::Checkbox("Demo Window", &m_bShow_Demo_Window);
	



		

		ImGui::Checkbox("MapMode", &m_bMap);
		ImGui::SameLine();
		if (m_bMap)
		{
			m_eMode = MODE_MAP;
			m_bVoxel = false;
			m_bTotal = false;
		}
		ImGui::Checkbox("VoxelMode", &m_bVoxel);
		ImGui::SameLine();
		if (m_bVoxel)
		{
			m_eMode = MODE_VOXEL;
			m_bMap = false;
			m_bTotal = false;
		}
		ImGui::Checkbox("TotalMode", &m_bTotal);
		if (m_bTotal)
		{
			m_eMode = MODE_TOTAL;
			m_bMap = false;
			m_bVoxel = false;
		}
		if (!m_bMap && !m_bVoxel && !m_bTotal)
		{
			m_bMap = true;
			m_eMode = MODE_MAP;
			m_bVoxel = false;
			m_bTotal = false;
		}
	
		switch (m_eMode)
		{
		case Client::CImGui_Manager::MODE_MAP:
			ImGui::Checkbox("Tile List", &m_bShow_Block_Window);
			break;
		case Client::CImGui_Manager::MODE_VOXEL:
			ImGui::ColorEdit3("Color List", (float*)&m_Voxel_Color);
			break;
		case Client::CImGui_Manager::MODE_TOTAL:
			ImGui::Checkbox("Voxel List", &m_bShow_Voxel_Window);
			break;
		}



		ImGui::Text("[ X: %d, Y: %d, Z: %d ] [ TileIndex = %d ]", m_Block_Info.x, m_Block_Info.y, m_Block_Info.z, m_Block_Info.iTileCount);

		
		if (30 < m_Block_Info.iTileCount)
			m_Block_Info.iTileCount = 0;
		if (0 > m_Block_Info.iTileCount)
			m_Block_Info.iTileCount = 30;


		ImGui::Checkbox("CulMode", &m_bCulMode);
		ImGui::Checkbox("KeyInputAble", &m_bKeyMode);
		char Temp[256] = {};
		if (ImGui::InputText("FildName", Temp, 256))
		{
			if (GetKeyState(VK_RETURN) & 0x8000)
			{
				m_FileName = Temp;
				

				for (int i = 0; i < 256; i++)
				{
					m_cFileName[i] = Temp[i];
				}


			}
		}
		ImGui::Text("[CurFildName] : %s", m_FileName.c_str());

		if (ImGui::Button("Clear"))
			CCubeManager::Get_Instance()->RemoveAllCubeDesc();



		switch (m_eMode)
		{
		case Client::CImGui_Manager::MODE_MAP:
			if (ImGui::Button("SaveMapButton"))
				CCubeManager::Get_Instance()->SaveMapData();
			ImGui::SameLine();
			if (ImGui::Button("LoadMapButton"))
				CCubeManager::Get_Instance()->LoadMapData();
			break;
		case Client::CImGui_Manager::MODE_VOXEL:
			if (ImGui::Button("SaveVoxelButton"))
				CCubeManager::Get_Instance()->SaveVoxelData();
			ImGui::SameLine();
			if (ImGui::Button("LoadVoxelButton"))
				CCubeManager::Get_Instance()->LoadVoxelData();
			break;
		case Client::CImGui_Manager::MODE_TOTAL:
			break;
		}






		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}




	if (m_bShow_Block_Window)
	{
		ShowBlockTool();
	}

	if (m_bShow_Voxel_Window)
	{
		ShowVoxelTool();
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

void CImGui_Manager::ShowTransformTool()
{
}

void CImGui_Manager::ShowBlockTool()
{
	ImGui::Begin("Block Window", &m_bShow_Block_Window);
	ImGui::SetWindowSize(ImVec2{ 350.f, 500.f });
	ImGui::Text("Block Window");

	
	for (_int i = 0; i < 20; i++)
	{
		if (ImGui::ImageButton(CCubeManager::Get_Instance()->Get_TextureCom()->GetTexture(i), ImVec2(50.f, 50.f)))
		{
			m_Block_Info.iTileCount = i;
		}
		if((i+1)%5 != 0)
			ImGui::SameLine();
	}
	

	if (ImGui::Button("Close Me"))
		m_bShow_Block_Window = false;
	ImGui::End();
}

void CImGui_Manager::ShowVoxelTool()
{
	ImGui::Begin("Voxel Window", &m_bShow_Voxel_Window);
	ImGui::SetWindowSize(ImVec2{ 350.f, 500.f });
	ImGui::Text("Voxel Window");


	map<string, list<CCube::CUBEDESC>>* Voxels = CCubeManager::Get_Instance()->Get_Voxels();

	for (auto& Pair : *Voxels)
	{
		const char* temp = Pair.first.c_str();
		if (ImGui::Button(temp))
			m_sCurVoxel = Pair.first;
	}


	ImGui::Text("[ X_A: %.3f, Y_A: %.3f, Z_A: %.3f ]", m_fX_Axis, m_fY_Axis, m_fZ_Axis);
	ImGui::Text("[ Scale = %.3f ]", m_fScale);

	ImGui::SliderFloat("X_Axis", &m_fX_Axis, 0.0f, 360.0f);
	ImGui::SliderFloat("Y_Axis", &m_fY_Axis, 0.0f, 360.0f);
	ImGui::SliderFloat("Z_Axis", &m_fZ_Axis, 0.0f, 360.0f);
	ImGui::SliderFloat("Scale", &m_fScale, 0.1f, 10.0f);

	ImGui::InputFloat("Input PosX", &m_vModelPos.x);
	ImGui::InputFloat("Input PosY", &m_vModelPos.y);
	ImGui::InputFloat("Input PosZ", &m_vModelPos.z);



	if (ImGui::Button("SaveModel"))
		CCubeManager::Get_Instance()->SaveModelData();


	if (ImGui::Button("Close Me"))
		m_bShow_Voxel_Window = false;
	ImGui::End();
}



void CImGui_Manager::SetDeleteCube(CGameObject * pObj, _float3 vPos)
{
	_float4x4		ViewMatrix;

	m_pGraphic_device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	/* 카메라의 월드행렬이다. */
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	 //   *(_float3*)&m_WorldMatrix.m[eState][0];
	_float3  CarView = *(_float3*)&ViewMatrix.m[3][0];


	_float3 vDis = CarView - vPos;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis < m_MinDist)
	{
		m_pDeleteCubeTemp = pObj;
		m_vDeletePos = vPos;
		m_MinDist = fDis;
	}
}

void CImGui_Manager::DeleteCube()
{
	if (nullptr == m_pDeleteCubeTemp)
		return;

	CCubeManager::Get_Instance()->RemoveCubeDesc(m_vDeletePos);
	m_pDeleteCubeTemp->SetDead();
	m_pDeleteCubeTemp = nullptr;
	m_MinDist = 99999.f;
	ZeroMemory(&m_vDeletePos, sizeof(_float3));
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
