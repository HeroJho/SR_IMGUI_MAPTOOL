#include "stdafx.h"
#include "..\Public\SetVoxelModel.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "CubeManager.h"
#include "Cube.h"

CSetVoxelModel::CSetVoxelModel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSetVoxelModel::CSetVoxelModel(const CSetVoxelModel & rhs)
	: CGameObject(rhs)
{
}


HRESULT CSetVoxelModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSetVoxelModel::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CCube::MODELDESC* ModelDesc = (CCube::MODELDESC*)pArg;

	CopyMemory(m_sFildName, ModelDesc->cModelName, sizeof(_tchar) * 256);

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, ModelDesc->vPos);
	m_pTransformCom->Set_Scaled(ModelDesc->fScale);
	m_pTransformCom->RotationThree(_float3(1.f, 0.f, 0.f), ModelDesc->vAix.x, _float3(0.f, 1.f, 0.f), ModelDesc->vAix.y, _float3(0.f, 0.f, 1.f), ModelDesc->vAix.z);


	return S_OK;
}

void CSetVoxelModel::Tick(_float fTimeDelta)
{


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

void CSetVoxelModel::LateTick(_float fTimeDelta)
{


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CSetVoxelModel::Render()
{
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

HRESULT CSetVoxelModel::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (CImGui_Manager::Get_Instance()->GetCulMode())
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CSetVoxelModel::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CSetVoxelModel::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;




	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_sFildName, TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
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



CSetVoxelModel * CSetVoxelModel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSetVoxelModel*		pInstance = new CSetVoxelModel(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVoxelModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSetVoxelModel::Clone(void* pArg)
{
	CSetVoxelModel*		pInstance = new CSetVoxelModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVoxelModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSetVoxelModel::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

