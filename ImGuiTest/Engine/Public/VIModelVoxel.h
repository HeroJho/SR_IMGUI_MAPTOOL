#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIModelVoxel final : public CVIBuffer
{
public:
	typedef struct tagVoxelDesc
	{
		_float3 vPos;
		_float3 vScale;
		_float3 vRotationAix;
		_float  fAngle;
	}VOXELDESC;
	typedef struct tagCubeDesc
	{
		_float3	vPos;
		_int	iIndex;
		_int 	r, g, b;
	}VOXELCUBEDESC;

private:
	CVIModelVoxel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIModelVoxel(const CVIModelVoxel& rhs);
	virtual ~CVIModelVoxel() = default;

public:
	virtual HRESULT Initialize_Prototype(list<VOXELCUBEDESC> Descs);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIModelVoxel* Create(LPDIRECT3DDEVICE9 pGraphic_Device, list<VOXELCUBEDESC> Descs);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END