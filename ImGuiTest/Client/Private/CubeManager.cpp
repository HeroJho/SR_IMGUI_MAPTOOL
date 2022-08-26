#include "stdafx.h"
#include "..\Public\CubeManager.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"


IMPLEMENT_SINGLETON(CCubeManager)


void CCubeManager::FillVoxelData()
{
	m_sVoxelData.push_back(string("Model_House1"));
	m_sVoxelData.push_back(string("Model_House2"));
	m_sVoxelData.push_back(string("Model_Wood1"));
	m_sVoxelData.push_back(string("Model_Wood2"));

	m_sVoxelData.push_back(string("Model_DHouse1"));
	m_sVoxelData.push_back(string("Model_DHouse2"));
	m_sVoxelData.push_back(string("Model_DMood1"));
	m_sVoxelData.push_back(string("Model_DWood2"));
}



CCubeManager::CCubeManager()
{
	
}

_bool CCubeManager::PosCheck(_float3 vPos)
{
	_bool sameCheck = false;
	for (auto& CubeDesc : m_CubeInfoList)
	{
		if (CubeDesc.vPos == vPos)
		{
			sameCheck = true;
			break;
		}
	}

	return sameCheck;
}

void CCubeManager::RemoveCubeDesc(_float3 vPos)
{
	auto iter = m_CubeInfoList.begin();
	for (; iter != m_CubeInfoList.end(); ++iter)
	{
		if (vPos == (*iter).vPos)
		{
			m_CubeInfoList.erase(iter);
			return;
		}
	}
}
void CCubeManager::RemoveAllCubeDesc()
{
	m_CubeInfoList.clear();
	CGameInstance::Get_Instance()->Clear(LEVEL_GAMEPLAY);
}

void CCubeManager::RemoveAllVoxelDesc()
{
	for (auto& Pair : m_Voxels)
	{
		Pair.second.clear();
	}
	m_Voxels.clear();
}







HRESULT CCubeManager::Init(LPDIRECT3DDEVICE9 pGraphic_device)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CComponent*			pComponent = pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Tile"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pTextureCom =  (CTexture*)pComponent;
	Safe_AddRef(pComponent);

	FillVoxelData();
	LoadVoxelData();

	_tchar* tempName = nullptr;

	for (auto& Pair : *(CCubeManager::Get_Instance()->Get_Voxels()))
	{
		tempName = new _tchar[256];
		ZeroMemory(tempName, sizeof(_tchar) * 256);
		for (int i = 0; i < Pair.first.size(); ++i)
		{
			tempName[i] = Pair.first[i];
		}
		

		list<CVIModelVoxel::VOXELCUBEDESC> tempList;
		for (auto& Desc : Pair.second)
		{
			CVIModelVoxel::VOXELCUBEDESC tempDesc;
			ZeroMemory(&tempDesc, sizeof(tempDesc));
			tempDesc.vPos = Desc.vPos;
			tempDesc.r = Desc.r;
			tempDesc.g = Desc.g;
			tempDesc.b = Desc.b;
			tempList.push_back(tempDesc);
		}

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, tempName,
			CVIModelVoxel::Create(pGraphic_device, tempList))))
			return E_FAIL;

	}





	Safe_Release(pGameInstance);

	return S_OK;
}



void CCubeManager::Tick(_float fTimeDelta)
{
	m_vCubePos = { (_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().x, (_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().y, (_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().z };

	if (!CImGui_Manager::Get_Instance()->GetKeyMode())
	{
		KeyInput();
	}


}

void CCubeManager::Render()
{
	m_bCol = false;

}

void CCubeManager::KeyInput()
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKState(DIK_LEFT) & 0x80)
	{
		if (!m_bKeyDown)
			CImGui_Manager::Get_Instance()->Left_Cube();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_RIGHT) & 0x80)
	{
		if (!m_bKeyDown)
			CImGui_Manager::Get_Instance()->Right_Cube();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_UP) & 0x80)
	{
		if (!m_bKeyDown)
			CImGui_Manager::Get_Instance()->Front_Cube();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_DOWN) & 0x80)
	{
		if (!m_bKeyDown)
			CImGui_Manager::Get_Instance()->Back_Cube();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_8) & 0x80)
	{
		if (!m_bKeyDown)
			CImGui_Manager::Get_Instance()->Up_Cube();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_2) & 0x80)
	{
		if (!m_bKeyDown)
			CImGui_Manager::Get_Instance()->Down_Cube();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_Q) & 0x80)
	{
		if (!m_bKeyDown)
			CImGui_Manager::Get_Instance()->DecreaseIndex();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_E) & 0x80)
	{
		if (!m_bKeyDown)
			CImGui_Manager::Get_Instance()->IncreaseIndex();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_SPACE) & 0x80)
	{
		if (CImGui_Manager::Get_Instance()->Get_CanCreate())
			return;

		if (!m_bKeyDown)
			CCubeManager::Get_Instance()->Create_Cube(m_vCubePos);
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIMKeyState(DIMK_LBUTTON) & 0x80)
	{
		if (CImGui_Manager::Get_Instance()->Get_CanCreate())
			return;

		if (!m_bKeyDown && m_bCol)
			CCubeManager::Get_Instance()->Create_Cube(m_vCubePos);
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_Z) & 0x80)
	{
		if (!m_bKeyDown)
		{
			if(CImGui_Manager::Get_Instance()->Get_CanCreate())
				CImGui_Manager::Get_Instance()->Set_CanCreate(false);
			else
				CImGui_Manager::Get_Instance()->Set_CanCreate(true);
		}
			
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_M) & 0x80)
	{
		if (!m_bKeyDown)
		{
			if (CImGui_Manager::Get_Instance()->Get_CanMouse())
				CImGui_Manager::Get_Instance()->Set_CanMouse(false);
			else
				CImGui_Manager::Get_Instance()->Set_CanMouse(true);
		}

		m_bKeyDown = true;
	}
	else
	{
		m_bKeyDown = false;
	}


	Safe_Release(pGameInstance);
}


HRESULT CCubeManager::Create_Cube(_float3 vPos)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_float3 Scale = m_pCubeTranCom->Get_Scaled();

	_int XSize = Scale.x * Scale.x;
	_int ZSize = Scale.z * Scale.z;




	if (!(CImGui_Manager::Get_Instance()->Get_Mode() == CImGui_Manager::MODE_TOTAL))
	{

		for (_int z = 0; z < Scale.x; ++z)
		{
			for (_int x = 0; x < Scale.z; ++x)
			{
				_float3 vTempPos{ vPos.x + (x - _int(Scale.x * 0.5f)), vPos.y, vPos.z + _float(z - _int(Scale.z * 0.5f)) };

				if (!CCubeManager::Get_Instance()->PosCheck(vTempPos))
				{
					CCube::CUBEDESC CubeDesc;
					CubeDesc.vPos = vTempPos;
					CubeDesc.iIndex = CImGui_Manager::Get_Instance()->Get_BlockInfo().iTileCount;


					if (CImGui_Manager::Get_Instance()->Get_Mode() == CImGui_Manager::MODE_VOXEL)
					{
						if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SetVoxelCube"), LEVEL_GAMEPLAY, TEXT("SetCubes"), &CubeDesc)))
							return E_FAIL;

						CubeDesc.r = CImGui_Manager::Get_Instance()->GetVoxelColor().x * 255.f;
						CubeDesc.g = CImGui_Manager::Get_Instance()->GetVoxelColor().y * 255.f;
						CubeDesc.b = CImGui_Manager::Get_Instance()->GetVoxelColor().z * 255.f;

						CCubeManager::Get_Instance()->AddCubeDesc(CubeDesc);
					}
					else if (CImGui_Manager::Get_Instance()->Get_Mode() == CImGui_Manager::MODE_MAP)
					{
						if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SetCube"), LEVEL_GAMEPLAY, TEXT("SetCubes"), &CubeDesc)))
							return E_FAIL;

						CubeDesc.r = 0.f;
						CubeDesc.g = 0.f;
						CubeDesc.b = 0.f;

						CCubeManager::Get_Instance()->AddCubeDesc(CubeDesc);
					}
				}

			}
		}



	}
	else
	{
		_float x = (_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().x + (_float)CImGui_Manager::Get_Instance()->Get_ModelPos().x;
		_float y = (_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().y + (_float)CImGui_Manager::Get_Instance()->Get_ModelPos().y;
		_float z = (_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().z + (_float)CImGui_Manager::Get_Instance()->Get_ModelPos().z;
		_float3 vTempPos{ x, y, z };

		_float3 vTempAix = { CImGui_Manager::Get_Instance()->Get_XAix(), CImGui_Manager::Get_Instance()->Get_YAix(), CImGui_Manager::Get_Instance()->Get_ZAix() };

		CCube::MODELDESC ModelDesc;
		ModelDesc.vPos = vTempPos;
		ModelDesc.vAix = vTempAix;
		ModelDesc.fScale = CImGui_Manager::Get_Instance()->Get_Scale();


		string tempStr = CImGui_Manager::Get_Instance()->Get_CurVoxel();

		for (int i = 0; i < tempStr.size(); i++)
		{
			ModelDesc.cModelName[i] = tempStr[i];
		}
		

		if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SetVoxelModel"), LEVEL_STATIC, TEXT("Models"), &ModelDesc)))
			return E_FAIL;

		m_Models.push_back(ModelDesc);
	}



	Safe_Release(pGameInstance);

	return S_OK;
}

void CCubeManager::DrawAll_Cube()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	for (auto& CubeDesc : m_CubeInfoList)
	{

		if (FAILED(pGameInstance->Add_GameObjectToLayer(TEXT("Prototype_GameObject_SetCube"), LEVEL_GAMEPLAY, TEXT("SetCubes"), &CubeDesc)))
			return;

		CubeDesc.r = 0.f;
		CubeDesc.g = 0.f;
		CubeDesc.b = 0.f;

	}


	Safe_Release(pGameInstance);
}







void CCubeManager::SaveMapData()
{
	// 1. 파일 개방

	// CreateFile : api 파일 개방 함수, 개방 실패 시 INVALID_HANDLE_VALUE하기 때문에 null값으로 예외처리를 할 수 없다.

	string FilePath = CImGui_Manager::Get_Instance()->GetFileNamesByString();

	string temp = "../Data/Map/";

	FilePath = temp + FilePath + ".dat";

	wchar_t RealPath[256] = { 0 };

	for (int i = 0; i < FilePath.size(); i++)
	{
		RealPath[i] = FilePath[i];
	}


	HANDLE		hFile = CreateFile(RealPath,			// 파일 경로와 이름 명시
		GENERIC_WRITE,				// 파일 접근 모드 (GENERIC_WRITE 쓰기 전용, GENERIC_READ 읽기 전용)
		NULL,						// 공유방식, 파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가, NULL인 경우 공유하지 않는다
		NULL,						// 보안 속성, 기본값	
		CREATE_ALWAYS,				// 생성 방식, CREATE_ALWAYS는 파일이 없다면 생성, 있다면 덮어 쓰기, OPEN_EXISTING 파일이 있을 경우에면 열기
		FILE_ATTRIBUTE_NORMAL,		// 파일 속성(읽기 전용, 숨기 등), FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반 파일 생성
		NULL);						// 생성도리 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않아서 NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// 팝업 창을 출력해주는 기능의 함수
		// 1. 핸들 2. 팝업 창에 띄우고자하는 메시지 3. 팝업 창 이름 4. 버튼 속성
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);
		return;
	}

	// 2. 파일 쓰기

	DWORD		dwByte = 0;

	wchar_t* FileName = CImGui_Manager::Get_Instance()->GetFileNames();


	WriteFile(hFile, FileName, sizeof(wchar_t) * 256, &dwByte, nullptr);

	for (auto& Data : m_CubeInfoList)
	{
		// Data.vPos.y -= 0.5f;
		WriteFile(hFile, &Data, sizeof(CCube::CUBEDESC), &dwByte, nullptr);
	}


	// 3. 파일 소멸
	CloseHandle(hFile);
}
void CCubeManager::LoadMapData(void)
{
	RemoveAllCubeDesc();

	HANDLE		hFile = CreateFile(L"../Data/Map/Map_Henesys.dat",			// 파일 경로와 이름 명시
		GENERIC_READ,				// 파일 접근 모드 (GENERIC_WRITE 쓰기 전용, GENERIC_READ 읽기 전용)
		NULL,						// 공유방식, 파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가, NULL인 경우 공유하지 않는다
		NULL,						// 보안 속성, 기본값	
		OPEN_EXISTING,				// 생성 방식, CREATE_ALWAYS는 파일이 없다면 생성, 있다면 덮어 쓰기, OPEN_EXISTING 파일이 있을 경우에면 열기
		FILE_ATTRIBUTE_NORMAL,		// 파일 속성(읽기 전용, 숨기 등), FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반 파일 생성
		NULL);						// 생성도리 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않아서 NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// 팝업 창을 출력해주는 기능의 함수
		// 1. 핸들 2. 팝업 창에 띄우고자하는 메시지 3. 팝업 창 이름 4. 버튼 속성
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return;
	}

	// 2. 파일 쓰기

	DWORD		dwByte = 0;
	CCube::CUBEDESC		tInfo{};

	wchar_t* FileName = CImGui_Manager::Get_Instance()->GetFileNames();


	ReadFile(hFile, FileName, sizeof(wchar_t) * 256, &dwByte, nullptr);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(CCube::CUBEDESC), &dwByte, nullptr);

		if (0 == dwByte)	// 더이상 읽을 데이터가 없을 경우
			break;

		// tInfo.vPos.y += 1.f;

		m_CubeInfoList.push_back(CCube::CUBEDESC(tInfo));
	}

	// 3. 파일 소멸
	CloseHandle(hFile);


	DrawAll_Cube();
}

void CCubeManager::SaveVoxelData()
{
	// 1. 파일 개방

	// CreateFile : api 파일 개방 함수, 개방 실패 시 INVALID_HANDLE_VALUE하기 때문에 null값으로 예외처리를 할 수 없다.

	string FilePath = CImGui_Manager::Get_Instance()->GetFileNamesByString();

	string temp = "../Data/Voxel/";

	FilePath = temp + FilePath + ".dat";

	wchar_t RealPath[256] = { 0 };

	for (int i = 0; i < FilePath.size(); i++)
	{
		RealPath[i] = FilePath[i];
	}


	HANDLE		hFile = CreateFile(RealPath,			// 파일 경로와 이름 명시
		GENERIC_WRITE,				// 파일 접근 모드 (GENERIC_WRITE 쓰기 전용, GENERIC_READ 읽기 전용)
		NULL,						// 공유방식, 파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가, NULL인 경우 공유하지 않는다
		NULL,						// 보안 속성, 기본값	
		CREATE_ALWAYS,				// 생성 방식, CREATE_ALWAYS는 파일이 없다면 생성, 있다면 덮어 쓰기, OPEN_EXISTING 파일이 있을 경우에면 열기
		FILE_ATTRIBUTE_NORMAL,		// 파일 속성(읽기 전용, 숨기 등), FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반 파일 생성
		NULL);						// 생성도리 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않아서 NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// 팝업 창을 출력해주는 기능의 함수
		// 1. 핸들 2. 팝업 창에 띄우고자하는 메시지 3. 팝업 창 이름 4. 버튼 속성
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);
		return;
	}

	// 2. 파일 쓰기

	DWORD		dwByte = 0;

	wchar_t* FileName = CImGui_Manager::Get_Instance()->GetFileNames();


	WriteFile(hFile, FileName, sizeof(wchar_t) * 256, &dwByte, nullptr);

	for (auto& Data : m_CubeInfoList)
	{
		// Data.vPos.y -= 0.5f;
		WriteFile(hFile, &Data, sizeof(CCube::CUBEDESC), &dwByte, nullptr);
	}


	// 3. 파일 소멸
	CloseHandle(hFile);
}
void CCubeManager::LoadVoxelData()
{
	RemoveAllVoxelDesc();

	for (auto& voxel : m_sVoxelData)
	{
		string FilePath = voxel;

		string temp = "../Data/Voxel/";

		FilePath = temp + FilePath + ".dat";

		wchar_t RealPath[256] = { 0 };

		for (int i = 0; i < FilePath.size(); i++)
		{
			RealPath[i] = FilePath[i];
		}




		HANDLE		hFile = CreateFile(RealPath,			// 파일 경로와 이름 명시
			GENERIC_READ,				// 파일 접근 모드 (GENERIC_WRITE 쓰기 전용, GENERIC_READ 읽기 전용)
			NULL,						// 공유방식, 파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가, NULL인 경우 공유하지 않는다
			NULL,						// 보안 속성, 기본값	
			OPEN_EXISTING,				// 생성 방식, CREATE_ALWAYS는 파일이 없다면 생성, 있다면 덮어 쓰기, OPEN_EXISTING 파일이 있을 경우에면 열기
			FILE_ATTRIBUTE_NORMAL,		// 파일 속성(읽기 전용, 숨기 등), FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반 파일 생성
			NULL);						// 생성도리 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않아서 NULL

		if (INVALID_HANDLE_VALUE == hFile)
		{
			// 팝업 창을 출력해주는 기능의 함수
			// 1. 핸들 2. 팝업 창에 띄우고자하는 메시지 3. 팝업 창 이름 4. 버튼 속성
			MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
			return;
		}

		// 2. 파일 쓰기

		DWORD		dwByte = 0;
		CCube::CUBEDESC		tInfo{};
		list<CCube::CUBEDESC> TempCubeInfoList;

		wchar_t FileName[256] = {0};


		ReadFile(hFile, &FileName, sizeof(wchar_t) * 256, &dwByte, nullptr);

		while (true)
		{
			ReadFile(hFile, &tInfo, sizeof(CCube::CUBEDESC), &dwByte, nullptr);

			if (0 == dwByte)	// 더이상 읽을 데이터가 없을 경우
				break;

			// tInfo.vPos.y += 0.5f;

			TempCubeInfoList.push_back(CCube::CUBEDESC(tInfo));
		}

		m_Voxels.emplace(voxel, TempCubeInfoList);

		// 3. 파일 소멸
		CloseHandle(hFile);

	}



}

void CCubeManager::SaveModelData()
{
	// 1. 파일 개방

	// CreateFile : api 파일 개방 함수, 개방 실패 시 INVALID_HANDLE_VALUE하기 때문에 null값으로 예외처리를 할 수 없다.

	string FilePath = CImGui_Manager::Get_Instance()->GetFileNamesByString();

	string temp = "../Data/Model/";

	FilePath = temp + FilePath + ".dat";

	wchar_t RealPath[256] = { 0 };

	for (int i = 0; i < FilePath.size(); i++)
	{
		RealPath[i] = FilePath[i];
	}


	HANDLE		hFile = CreateFile(RealPath,			// 파일 경로와 이름 명시
		GENERIC_WRITE,				// 파일 접근 모드 (GENERIC_WRITE 쓰기 전용, GENERIC_READ 읽기 전용)
		NULL,						// 공유방식, 파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가, NULL인 경우 공유하지 않는다
		NULL,						// 보안 속성, 기본값	
		CREATE_ALWAYS,				// 생성 방식, CREATE_ALWAYS는 파일이 없다면 생성, 있다면 덮어 쓰기, OPEN_EXISTING 파일이 있을 경우에면 열기
		FILE_ATTRIBUTE_NORMAL,		// 파일 속성(읽기 전용, 숨기 등), FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반 파일 생성
		NULL);						// 생성도리 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않아서 NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// 팝업 창을 출력해주는 기능의 함수
		// 1. 핸들 2. 팝업 창에 띄우고자하는 메시지 3. 팝업 창 이름 4. 버튼 속성
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);
		return;
	}

	// 2. 파일 쓰기

	DWORD		dwByte = 0;

	wchar_t* FileName = CImGui_Manager::Get_Instance()->GetFileNames();


	WriteFile(hFile, FileName, sizeof(wchar_t) * 256, &dwByte, nullptr);

	for (auto& Data : m_Models)
	{
		// Data.vPos.y -= 0.5f;
		WriteFile(hFile, &Data, sizeof(CCube::MODELDESC), &dwByte, nullptr);
	}


	// 3. 파일 소멸
	CloseHandle(hFile);
}





void CCubeManager::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pCubeTranCom);
}
