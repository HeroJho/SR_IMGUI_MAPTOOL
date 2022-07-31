#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Cube.h"

BEGIN(Client)

class CImGui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager)

public:
	enum MODE { MODE_MAP, MODE_VOXEL, MODE_TOTAL, MODE_END };

private:
	CImGui_Manager();
	virtual ~CImGui_Manager() = default;

public:
	BLOCK_INFO& Get_BlockInfo() { return m_Block_Info; }


	MODE Get_Mode() { return m_eMode; }
	void Set_Mode(MODE eMode) { m_eMode = eMode; }

	string Get_CurVoxel() { return m_sCurVoxel; }

	_bool GetCulMode(){ return m_bCulMode; }
	_bool GetKeyMode() { return m_bKeyMode; }
	wchar_t* GetFileNames() { return m_cFileName; }
	string GetFileNamesByString() { return m_FileName; }

	ImVec4 GetVoxelColor() { return m_Voxel_Color; }

	void Left_Cube() { --m_Block_Info.x; }
	void Right_Cube() { ++m_Block_Info.x; }
	void Front_Cube() { ++m_Block_Info.z; }
	void Back_Cube() { --m_Block_Info.z; }
	void Up_Cube() { ++m_Block_Info.y; }
	void Down_Cube() { --m_Block_Info.y; }
	
	void IncreaseIndex() { ++m_Block_Info.iTileCount; }
	void DecreaseIndex() { --m_Block_Info.iTileCount; }



	_float Get_Scale() { return m_fScale; }
	_float Get_XAix() { return m_fX_Axis; }
	_float Get_YAix() { return m_fY_Axis; }
	_float Get_ZAix() { return m_fZ_Axis; }


public:
	HRESULT Init(LPDIRECT3DDEVICE9 pDevice);
	void Tick(_float fTimeDelta);
	void Render();


private:
	void ShowTransformTool();
	void ShowBlockTool();
	void ShowVoxelTool();

private:
	LPDIRECT3DDEVICE9 m_pGraphic_device;

private:
	bool m_bShow_Demo_Window;

	bool m_bShow_Transform_Window;
	bool m_bShow_Block_Window;
	bool m_bShow_Voxel_Window;
	ImVec4 m_Voxel_Color;

	MODE m_eMode = MODE_MAP;

	_bool m_bCulMode = false;
	_bool m_bKeyMode = false;

	string m_FileName;
	wchar_t m_cFileName[256];

	_bool m_bMap = false;
	_bool m_bVoxel = false;
	_bool m_bTotal = false;

	string m_sCurVoxel;

private:
	BLOCK_INFO m_Block_Info;
	
	_float m_fScale = 1.f;
	_float m_fX_Axis = 0.f;
	_float m_fY_Axis = 0.f;
	_float m_fZ_Axis = 0.f;

public:
	virtual void Free() override;

};

END
