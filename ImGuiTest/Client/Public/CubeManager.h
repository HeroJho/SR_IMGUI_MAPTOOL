#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Cube.h"

#include "Transform.h"

BEGIN(Client)

class CCubeManager final : public CBase
{
	DECLARE_SINGLETON(CCubeManager)


private:
	CCubeManager();
	virtual ~CCubeManager() = default;

public:
	_bool PosCheck(_float3 vPos);
	void AddCubeDesc(CCube::CUBEDESC Info) { m_CubeInfoList.push_back(Info); }
	void RemoveCubeDesc(_float3 vPos);
	void RemoveAllCubeDesc();
	void RemoveAllVoxelDesc();

	map<string, list<CCube::CUBEDESC>>* Get_Voxels() { return &m_Voxels; }

	void SaveMapData();
	void LoadMapData();
	void SaveVoxelData();
	void LoadVoxelData();

	void SaveModelData();

	CTexture* Get_TextureCom() { return m_pTextureCom; }
	void		Set_Cube(CTransform* pCube) { m_pCubeTranCom = pCube; Safe_AddRef(m_pCubeTranCom); }
	void		Set_Col() { m_bCol = true; }

	HRESULT Create_Cube(_float3 vPos);
	void DrawAll_Cube();

	list<string> GetVoxelString() { return m_sVoxelData; }




public:
	HRESULT Init(LPDIRECT3DDEVICE9 pDevice);
	void FillVoxelData();
	void Tick(_float fTimeDelta);
	void Render();
	void KeyInput();

private:
	list<CCube::CUBEDESC> m_CubeInfoList;

	map<string, list<CCube::CUBEDESC>> m_Voxels;
	list<CCube::MODELDESC> m_Models;

	list<string> m_sVoxelData;

	_float3			m_vCubePos;
	_int			m_TileIndex;
	_bool			m_bKeyDown = false;
	_bool			m_bClick = false;
	_bool			m_bCol = false;


private:
	CTexture*					m_pTextureCom = nullptr;
	CTransform*					m_pCubeTranCom = nullptr;

public:
	virtual void Free() override;

};

END
