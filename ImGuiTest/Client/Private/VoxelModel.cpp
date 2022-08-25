#include "stdafx.h"
#include "..\Public\VoxelModel.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "CubeManager.h"

CVoxelModel::CVoxelModel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CVoxelModel::CVoxelModel(const CVoxelModel & rhs)
	: CGameObject(rhs)
{
}


HRESULT CVoxelModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVoxelModel::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	VOXELMODELDESC* ModelDesc = (VOXELMODELDESC*)pArg;
	m_sFildName = ModelDesc->sTag;



	if (FAILED(SetUp_Components()))
		return E_FAIL;


	// CCubeManager::Get_Instance()->Set_Cube(m_pTransformCom);

	return S_OK;
}

void CVoxelModel::Tick(_float fTimeDelta)
{


	m_pTransformCom->RotationThree(_float3(1.0f, 0.f, 0.f), CImGui_Manager::Get_Instance()->Get_XAix(), _float3(0.f, 1.f, 0.f), CImGui_Manager::Get_Instance()->Get_YAix(), _float3(0.f, 0.f, 1.f), CImGui_Manager::Get_Instance()->Get_ZAix());
	m_pTransformCom->Set_Scaled(CImGui_Manager::Get_Instance()->Get_Scale());



	if (CImGui_Manager::Get_Instance()->Get_CanCreate())
		return;

	_float x = ((_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().x) + ((_float)CImGui_Manager::Get_Instance()->Get_ModelPos().x);
	_float y = ((_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().y) + ((_float)CImGui_Manager::Get_Instance()->Get_ModelPos().y);
	_float z = ((_float)CImGui_Manager::Get_Instance()->Get_BlockInfo().z) + ((_float)CImGui_Manager::Get_Instance()->Get_ModelPos().z);

	_float3 tempPos{ x, y, z };
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, tempPos);

}

void CVoxelModel::LateTick(_float fTimeDelta)
{


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CVoxelModel::Render()
{
	if (!(CImGui_Manager::Get_Instance()->Get_Mode() == CImGui_Manager::MODE_TOTAL))
		return S_OK;

	if (CImGui_Manager::Get_Instance()->Get_CurVoxel() != m_sFildName)
		return S_OK;


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

HRESULT CVoxelModel::Set_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (CImGui_Manager::Get_Instance()->GetCulMode())
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CVoxelModel::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CVoxelModel::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	
	
	_tchar tempFile[256] = { 0 };
	for (int i = 0; i < m_sFildName.size(); ++i)
	{
		tempFile[i] = m_sFildName[i];
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, tempFile, TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
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



CVoxelModel * CVoxelModel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVoxelModel*		pInstance = new CVoxelModel(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVoxelModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CVoxelModel::Clone(void* pArg)
{
	CVoxelModel*		pInstance = new CVoxelModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVoxelModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVoxelModel::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

