#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIModelVoxel;
END


BEGIN(Client)

class CVoxelModel final : public CGameObject
{
public:
	typedef struct tagVoxelModelDesc
	{
		string sTag;
	}VOXELMODELDESC;
private:
	CVoxelModel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVoxelModel(const CVoxelModel& rhs);
	virtual ~CVoxelModel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIModelVoxel*			m_pVIBufferCom = nullptr;

private:
	_float3			m_vTargetPos = _float3(0.f, 0.f, 0.f);
	_bool  m_bKeyDown = false;
	string m_sFildName;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();



public:
	static CVoxelModel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END