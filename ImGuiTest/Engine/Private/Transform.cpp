#include "..\Public\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
{

}

_float3 CTransform::Get_Scaled() const
{
	return _float3(D3DXVec3Length(&Get_State(CTransform::STATE_RIGHT)), 
		D3DXVec3Length(&Get_State(CTransform::STATE_UP)), 
		D3DXVec3Length(&Get_State(CTransform::STATE_LOOK)));
}

void CTransform::Set_Scaled(_float3 vScale)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

void CTransform::Set_Scaled(_float vScale)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale);
}

void CTransform::Increase_ScaledXZ()
{
	_float3		vScale = Get_Scaled();
	vScale.x += 2.f;
	vScale.z += 2.f;

	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}
void CTransform::Decrease_ScaledXZ()
{
	_float3		vScale = Get_Scaled();

	if (vScale.x < 1.1f)
	{
		return;
	}

	vScale.x -= 2.f;
	vScale.z -= 2.f;

	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

void CTransform::Increase_ScaledY()
{
	_float3		vScale = Get_Scaled();
	vScale.y += 2.f;

	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}
void CTransform::Decrease_ScaledY()
{
	_float3		vScale = Get_Scaled();

	if (vScale.y < 1.1f)
	{
		return;
	}

	vScale.y -= 2.f;
	
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

HRESULT CTransform::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

HRESULT CTransform::Bind_WorldMatrix()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);

	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Rotation(_float3 vAxis, _float fRadian)
{
	_float3		vScale = Get_Scaled();

	_float3		vRight = _float3(1.f, 0.f, 0.f) * vScale.x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * vScale.y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * vScale.z;

	_float4x4	RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, D3DXToRadian(fRadian));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

}

void CTransform::RotationThree(_float3 vAxis, _float fRadian, _float3 vAxis2, _float fRadian2, _float3 vAxis3, _float fRadian3)
{
	_float3		vScale = Get_Scaled();

	_float3		vRight = _float3(1.f, 0.f, 0.f) * vScale.x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * vScale.y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * vScale.z;

	_float4x4	RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, D3DXToRadian(fRadian));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);


	_float3		vRight2 = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp2 = Get_State(CTransform::STATE_UP);
	_float3		vLook2 = Get_State(CTransform::STATE_LOOK);

	_float4x4	RotationMatrix2;

	D3DXMatrixRotationAxis(&RotationMatrix2, &vAxis2, D3DXToRadian(fRadian2));

	D3DXVec3TransformNormal(&vRight2, &vRight2, &RotationMatrix2);
	D3DXVec3TransformNormal(&vUp2, &vUp2, &RotationMatrix2);
	D3DXVec3TransformNormal(&vLook2, &vLook2, &RotationMatrix2);

	Set_State(CTransform::STATE_RIGHT, vRight2);
	Set_State(CTransform::STATE_UP, vUp2);
	Set_State(CTransform::STATE_LOOK, vLook2);


	_float3		vRight3 = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp3 = Get_State(CTransform::STATE_UP);
	_float3		vLook3 = Get_State(CTransform::STATE_LOOK);

	_float4x4	RotationMatrix3;

	D3DXMatrixRotationAxis(&RotationMatrix3, &vAxis3, D3DXToRadian(fRadian3));

	D3DXVec3TransformNormal(&vRight3, &vRight3, &RotationMatrix3);
	D3DXVec3TransformNormal(&vUp3, &vUp3, &RotationMatrix3);
	D3DXVec3TransformNormal(&vLook3, &vLook3, &RotationMatrix3);

	Set_State(CTransform::STATE_RIGHT, vRight3);
	Set_State(CTransform::STATE_UP, vUp3);
	Set_State(CTransform::STATE_LOOK, vLook3);
}

void CTransform::Turn(_float3 vAxis, _float fTimeDelta)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	_float4x4	RotationMatrix;

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::LookAt(_float3 vTargetPos)
{
	_float3		vScale = Get_Scaled();
	
	_float3 vPosition = Get_State(CTransform::STATE_POSITION);

	_float3	vLook = vTargetPos - vPosition;

	_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);

	_float3 vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

void CTransform::LookAtForLandObject(_float3 vTargetPos)
{
	_float3		vScale = Get_Scaled();

	_float3 vPosition = Get_State(CTransform::STATE_POSITION);

	_float3	vLook = vTargetPos - vPosition;

	_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);

	_float3 vUp = _float3(0.f, 1.f, 0.f);

	vLook = *D3DXVec3Cross(&vLook, &vRight, &vUp);



	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

void CTransform::Chase(_float3 vTargetPos, _float fTimeDelta)
{
	LookAt(vTargetPos);

	Go_Straight(fTimeDelta);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*			pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*			pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}
