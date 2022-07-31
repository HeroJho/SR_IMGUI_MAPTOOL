#include "..\Public\VIModelVoxel.h"

#include "GameInstance.h"

CVIModelVoxel::CVIModelVoxel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIModelVoxel::CVIModelVoxel(const CVIModelVoxel & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIModelVoxel::Initialize_Prototype(list<VOXELCUBEDESC> Descs)
{
	list<tagVertexColor> VoxelDatas;

	for (auto& Data : Descs)
	{
		tagVertexColor temp;
		temp.vPosition = Data.vPos;
		temp.dwColor = D3DCOLOR_XRGB(Data.r, Data.g, Data.b);
		VoxelDatas.push_back(temp);
	}


	m_iStride = sizeof(VTXCOL);
	m_iNumVertices = 8 * VoxelDatas.size();
	m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 16 * VoxelDatas.size();

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXCOL*		pVertices = nullptr;

	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	_int iIndex = 0;
	for (auto& Data : VoxelDatas)
	{
		pVertices[iIndex].vPosition = _float3(-0.5f + Data.vPosition.x, 0.5f + Data.vPosition.y, -0.5f + Data.vPosition.z);
		pVertices[iIndex].dwColor = Data.dwColor;
		pVertices[1 + iIndex].vPosition = _float3(0.5f + Data.vPosition.x, 0.5f + Data.vPosition.y, -0.5f + Data.vPosition.z);
		pVertices[1 + iIndex].dwColor = Data.dwColor;
		pVertices[2 + iIndex].vPosition = _float3(0.5f + Data.vPosition.x, -0.5f + Data.vPosition.y, -0.5f + Data.vPosition.z);
		pVertices[2 + iIndex].dwColor = Data.dwColor;
		pVertices[3 + iIndex].vPosition = _float3(-0.5f + Data.vPosition.x, -0.5f + Data.vPosition.y, -0.5f + Data.vPosition.z);
		pVertices[3 + iIndex].dwColor = Data.dwColor;

		pVertices[4 + iIndex].vPosition = _float3(-0.5f + Data.vPosition.x, 0.5f + Data.vPosition.y, 0.5f + Data.vPosition.z);
		pVertices[4 + iIndex].dwColor = Data.dwColor;
		pVertices[5 + iIndex].vPosition = _float3(0.5f + Data.vPosition.x, 0.5f + Data.vPosition.y, 0.5f + Data.vPosition.z);
		pVertices[5 + iIndex].dwColor = Data.dwColor;
		pVertices[6 + iIndex].vPosition = _float3(0.5f + Data.vPosition.x, -0.5f + Data.vPosition.y, 0.5f + Data.vPosition.z);
		pVertices[6 + iIndex].dwColor = Data.dwColor;
		pVertices[7 + iIndex].vPosition = _float3(-0.5f + Data.vPosition.x, -0.5f + Data.vPosition.y, 0.5f + Data.vPosition.z);
		pVertices[7 + iIndex].dwColor = Data.dwColor;

		iIndex += 8;
	}

	m_pVB->Unlock();





	m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	iIndex = 0;
	for (_int i = 0; i < VoxelDatas.size(); i++)
	{

		/* +x */
		pIndices[0 + iIndex]._0 = 1 + i * 8; pIndices[0 + iIndex]._1 = 5 + i * 8; pIndices[0 + iIndex]._2 = 6 + i * 8;
		pIndices[1 + iIndex]._0 = 1 + i * 8; pIndices[1 + iIndex]._1 = 6 + i * 8; pIndices[1 + iIndex]._2 = 2 + i * 8;

		/* -x */
		pIndices[2 + iIndex]._0 = 4 + i * 8; pIndices[2 + iIndex]._1 = 0 + i * 8; pIndices[2 + iIndex]._2 = 3 + i * 8;
		pIndices[3 + iIndex]._0 = 4 + i * 8; pIndices[3 + iIndex]._1 = 3 + i * 8; pIndices[3 + iIndex]._2 = 7 + i * 8;

		/* +y */
		pIndices[4 + iIndex]._0 = 4 + i * 8; pIndices[4 + iIndex]._1 = 5 + i * 8; pIndices[4 + iIndex]._2 = 1 + i * 8;
		pIndices[5 + iIndex]._0 = 4 + i * 8; pIndices[5 + iIndex]._1 = 1 + i * 8; pIndices[5 + iIndex]._2 = 0 + i * 8;

		/* -y */
		pIndices[6 + iIndex]._0 = 3 + i * 8; pIndices[6 + iIndex]._1 = 2 + i * 8; pIndices[6 + iIndex]._2 = 6 + i * 8;
		pIndices[7 + iIndex]._0 = 3 + i * 8; pIndices[7 + iIndex]._1 = 6 + i * 8; pIndices[7 + iIndex]._2 = 7 + i * 8;

		/* +z */
		pIndices[8 + iIndex]._0 = 5 + i * 8; pIndices[8 + iIndex]._1 = 4 + i * 8; pIndices[8 + iIndex]._2 = 7 + i * 8;
		pIndices[9 + iIndex]._0 = 5 + i * 8; pIndices[9 + iIndex]._1 = 7 + i * 8; pIndices[9 + iIndex]._2 = 6 + i * 8;

		/* -z */
		pIndices[10 + iIndex]._0 = 0 + i * 8; pIndices[10 + iIndex]._1 = 1 + i * 8; pIndices[10 + iIndex]._2 = 2 + i * 8;
		pIndices[11 + iIndex]._0 = 0 + i * 8; pIndices[11 + iIndex]._1 = 2 + i * 8; pIndices[11 + iIndex]._2 = 3 + i * 8;


		iIndex += 12;
	}



	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIModelVoxel::Initialize(void * pArg)
{
	return S_OK;
}

CVIModelVoxel * CVIModelVoxel::Create(LPDIRECT3DDEVICE9 pGraphic_Device, list<VOXELCUBEDESC> Descs)
{
	CVIModelVoxel*			pInstance = new CVIModelVoxel(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(Descs)))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIModelVoxel::Clone(void * pArg)
{
	CVIModelVoxel*			pInstance = new CVIModelVoxel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIModelVoxel::Free()
{
	__super::Free();
}
