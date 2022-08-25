#include "stdafx.h"
#include "..\Public\SetVoxelCube.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Cube.h"
#include "CubeManager.h"

CSetVoxelCube::CSetVoxelCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSetVoxelCube::CSetVoxelCube(const CSetVoxelCube & rhs)
	: CGameObject(rhs)
{
}


HRESULT CSetVoxelCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSetVoxelCube::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		CCube::CUBEDESC* pCubeDesc = (CCube::CUBEDESC*)pArg;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pCubeDesc->vPos);
		m_TileIndex = pCubeDesc->iIndex;

		r = CImGui_Manager::Get_Instance()->GetVoxelColor().x * 255;
		g = CImGui_Manager::Get_Instance()->GetVoxelColor().y * 255;
		b = CImGui_Manager::Get_Instance()->GetVoxelColor().z * 255;
	}

	return S_OK;
}

void CSetVoxelCube::Tick(_float fTimeDelta)
{
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

void CSetVoxelCube::LateTick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CSetVoxelCube::Render()
{
	m_pVIBufferCom->SetColor(r, g, b);

	if (FAILED(m_pTransformCom->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphic_Device->SetTexture(0, nullptr);

	if (FAILED(Set_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Reset_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSetVoxelCube::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return S_OK;
}

HRESULT CSetVoxelCube::Reset_RenderState()
{


	return S_OK;
}

HRESULT CSetVoxelCube::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_VoxelCube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
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

CSetVoxelCube * CSetVoxelCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSetVoxelCube*		pInstance = new CSetVoxelCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSetVoxelCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSetVoxelCube::Clone(void* pArg)
{
	CSetVoxelCube*		pInstance = new CSetVoxelCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSetVoxelCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSetVoxelCube::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

