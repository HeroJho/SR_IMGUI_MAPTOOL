#include "stdafx.h"
#include "..\Public\VoxelCube.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "CubeManager.h"

CVoxelCube::CVoxelCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CVoxelCube::CVoxelCube(const CVoxelCube & rhs)
	: CGameObject(rhs)
{
}


HRESULT CVoxelCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVoxelCube::Initialize(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;	


	CCubeManager::Get_Instance()->Set_Cube(m_pTransformCom);

	return S_OK;
}

void CVoxelCube::Tick(_float fTimeDelta)
{
	_float3 tempPos{ (_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().x, (_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().y, (_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().z };
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, tempPos);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKState(DIK_P) & 0x80)
	{
		if (!m_bKeyDown)
			m_pTransformCom->Increase_ScaledXZ();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_O) & 0x80)
	{
		if (!m_bKeyDown)
			m_pTransformCom->Decrease_ScaledXZ();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_L) & 0x80)
	{
		if (!m_bKeyDown)
			m_pTransformCom->Increase_ScaledY();
		m_bKeyDown = true;
	}
	else if (pGameInstance->Get_DIKState(DIK_K) & 0x80)
	{
		if (!m_bKeyDown)
			m_pTransformCom->Decrease_ScaledY();
		m_bKeyDown = true;
	}
	else
		m_bKeyDown = false;

	Safe_Release(pGameInstance);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

void CVoxelCube::LateTick(_float fTimeDelta)
{


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CVoxelCube::Render()
{
	if (!(CImGui_Manager::Get_Instance()->Get_Mode() == CImGui_Manager::MODE_VOXEL))
		return S_OK;

	_int r = CImGui_Manager::Get_Instance()->GetVoxelColor().x * 255;
	_int g = CImGui_Manager::Get_Instance()->GetVoxelColor().y * 255;
	_int b = CImGui_Manager::Get_Instance()->GetVoxelColor().z * 255;


	if (CImGui_Manager::Get_Instance()->GetCulMode())
		m_pVIBufferCom->SetColor(255, 255, 255);
	else
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

HRESULT CVoxelCube::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if(CImGui_Manager::Get_Instance()->GetCulMode())
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CVoxelCube::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CVoxelCube::SetUp_Components()
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



CVoxelCube * CVoxelCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVoxelCube*		pInstance = new CVoxelCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVoxelCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CVoxelCube::Clone(void* pArg)
{
	CVoxelCube*		pInstance = new CVoxelCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVoxelCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVoxelCube::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

