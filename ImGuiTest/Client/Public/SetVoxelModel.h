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

class CSetVoxelModel final : public CGameObject
{
private:
	CSetVoxelModel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSetVoxelModel(const CSetVoxelModel& rhs);
	virtual ~CSetVoxelModel() = default;

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
	_tchar m_sFildName[256];

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT SetUp_Components();



public:
	static CSetVoxelModel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END