#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Voxel;
END


BEGIN(Client)

class CSetVoxelCube final : public CGameObject
{
private:
	CSetVoxelCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSetVoxelCube(const CSetVoxelCube& rhs);
	virtual ~CSetVoxelCube() = default;

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
	CVIBuffer_Voxel*		m_pVIBufferCom = nullptr;

private:
	_float3			m_vTargetPos = _float3(0.f, 0.f, 0.f);
	_int			m_TileIndex = 0;
	_bool			m_bKeyDown = false;

	_int r, g, b;

private:
	HRESULT Set_RenderState();
	HRESULT Reset_RenderState();
private:
	HRESULT SetUp_Components();

public:
	static CSetVoxelCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END