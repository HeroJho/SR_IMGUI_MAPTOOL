#include "stdafx.h"
#include "..\Public\SetCube.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Cube.h"
#include "CubeManager.h"

CSetCube::CSetCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSetCube::CSetCube(const CSetCube & rhs)
	: CGameObject(rhs)
{
}


HRESULT CSetCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSetCube::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	if (nullptr != pArg)
	{
		CCube::CUBEDESC* pCubeDesc = (CCube::CUBEDESC*)pArg;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pCubeDesc->vPos);
		m_TileIndex = pCubeDesc->iIndex;
	}

	return S_OK;
}

void CSetCube::Tick(_float fTimeDelta)
{
	if (CImGui_Manager::Get_Instance()->Get_CanMouse())
		return;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_float3			vPickPos;
	_float3			vPickVec;

	if (m_pVIBufferCom->PickingCube(m_pTransformCom, &vPickPos, &vPickVec))
	{
		CCubeManager::Get_Instance()->Set_Col();

		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		CImGui_Manager::Get_Instance()->Get_BlockInfo().x = vPos.x + vPickVec.x;
		CImGui_Manager::Get_Instance()->Get_BlockInfo().y = vPos.y + vPickVec.y;
		CImGui_Manager::Get_Instance()->Get_BlockInfo().z = vPos.z + vPickVec.z;

		if (pGameInstance->Get_DIKState(DIK_X) & 0x80)
		{

			if (!m_bKeyDown)
			{
				CImGui_Manager::Get_Instance()->SetDeleteCube(this, vPos);
			}
			m_bKeyDown = true;
		}
		else
			m_bKeyDown = false;
	}
	

	Safe_Release(pGameInstance);
}

void CSetCube::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CSetCube::Render()
{
	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;	
	
	if (FAILED(m_pTextureCom->Bind_Texture(m_TileIndex)))
		return E_FAIL;

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSetCube::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return S_OK;
}

HRESULT CSetCube::Reset_RenderState()
{


	return S_OK;
}

HRESULT CSetCube::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Cube"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(TransformDesc));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CSetCube * CSetCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSetCube*		pInstance = new CSetCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSetCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSetCube::Clone(void* pArg)
{
	CSetCube*		pInstance = new CSetCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSetCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSetCube::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

