#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Voxel final : public CVIBuffer
{
private:
	CVIBuffer_Voxel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Voxel(const CVIBuffer_Voxel& rhs);
	virtual ~CVIBuffer_Voxel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual _bool Picking(class CTransform* pTransform, _float3* pOut);
	virtual _bool PickingCube(class CTransform* pTransform, _float3* pOutDis, _float3* pOutVec);
	void SetColor(_int r, _int g, _int b);


public:
	static CVIBuffer_Voxel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END