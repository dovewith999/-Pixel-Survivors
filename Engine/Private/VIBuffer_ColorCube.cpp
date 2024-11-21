#include "..\Public\VIBuffer_ColorCube.h"




CVIBuffer_ColorCube::CVIBuffer_ColorCube(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
}

CVIBuffer_ColorCube::CVIBuffer_ColorCube(const CVIBuffer_ColorCube & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_ColorCube::NativeConstruct_Prototype()
{
	m_iNumVertices = 8;
	m_iStride = sizeof(VERTEXCOLOR);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE/* | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0)*/;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	//VERTEXTXT* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	//pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	//pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	//pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	//pVertices[3].vTexUV = _vec2(0.f, 1.f);

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	//pVertices[4].vTexUV = _vec2(1.f, 0.f);

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	//pVertices[5].vTexUV = _vec2(0.f, 0.f);

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	//pVertices[6].vTexUV = _vec2(0.f, 1.f);

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	//pVertices[7].vTexUV = _vec2(1.f, 1.f);



	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	pIndices[2]._0 = 4;
	pIndices[2]._1 = 5;
	pIndices[2]._2 = 1;

	pIndices[3]._0 = 4;
	pIndices[3]._1 = 1;
	pIndices[3]._2 = 0;

	pIndices[4]._0 = 5;
	pIndices[4]._1 = 4;
	pIndices[4]._2 = 6;

	pIndices[5]._0 = 6;
	pIndices[5]._1 = 4;
	pIndices[5]._2 = 7;

	pIndices[6]._0 = 3;
	pIndices[6]._1 = 2;
	pIndices[6]._2 = 7;

	pIndices[7]._0 = 7;
	pIndices[7]._1 = 2;
	pIndices[7]._2 = 6;

	pIndices[8]._0 = 3;
	pIndices[8]._1 = 7;
	pIndices[8]._2 = 0;

	pIndices[9]._0 = 0;
	pIndices[9]._1 = 7;
	pIndices[9]._2 = 4;

	pIndices[10]._0 = 1;
	pIndices[10]._1 = 5;	
	pIndices[10]._2 = 2;

	pIndices[11]._0 = 2;
	pIndices[11]._1 = 5;
	pIndices[11]._2 = 6;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_ColorCube::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_ColorCube::Set_VertexResize(D3DXVECTOR3 vSize)
{
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(pVertices[0].vPosition.x * vSize.x, pVertices[0].vPosition.y *vSize.y, pVertices[0].vPosition.z *vSize.z);
	pVertices[1].vPosition = _float3(pVertices[1].vPosition.x * vSize.x, pVertices[1].vPosition.y *vSize.y, pVertices[1].vPosition.z *vSize.z);
	pVertices[2].vPosition = _float3(pVertices[2].vPosition.x * vSize.x, pVertices[2].vPosition.y *vSize.y, pVertices[2].vPosition.z *vSize.z);
	pVertices[3].vPosition = _float3(pVertices[3].vPosition.x * vSize.x, pVertices[3].vPosition.y *vSize.y, pVertices[3].vPosition.z *vSize.z);
	pVertices[4].vPosition = _float3(pVertices[4].vPosition.x * vSize.x, pVertices[4].vPosition.y *vSize.y, pVertices[4].vPosition.z *vSize.z);
	pVertices[5].vPosition = _float3(pVertices[5].vPosition.x * vSize.x, pVertices[5].vPosition.y *vSize.y, pVertices[5].vPosition.z *vSize.z);
	pVertices[6].vPosition = _float3(pVertices[6].vPosition.x * vSize.x, pVertices[6].vPosition.y *vSize.y, pVertices[6].vPosition.z *vSize.z);
	pVertices[7].vPosition = _float3(pVertices[7].vPosition.x * vSize.x, pVertices[7].vPosition.y *vSize.y, pVertices[7].vPosition.z *vSize.z);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_ColorCube::Set_VertexPosCubeCreate(D3DXVECTOR3 vPos)
{
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f + vPos.x, 0.f + vPos.y, -0.5f + vPos.z);

	pVertices[1].vPosition = _float3(0.5f + vPos.x, 0.f + vPos.y, -0.5f + vPos.z);

	pVertices[2].vPosition = _float3(0.5f + vPos.x, -1.f + vPos.y, -0.5f + vPos.z);

	pVertices[3].vPosition = _float3(-0.5f + vPos.x, -1.f + vPos.y, -0.5f + vPos.z);

	pVertices[4].vPosition = _float3(-0.5f + vPos.x, 0.f + vPos.y, 0.5f + vPos.z);

	pVertices[5].vPosition = _float3(0.5f + vPos.x, 0.f + vPos.y, 0.5f + vPos.z);

	pVertices[6].vPosition = _float3(0.5f + vPos.x, -1.f + vPos.y, 0.5f + vPos.z);

	pVertices[7].vPosition = _float3(-0.5f + vPos.x, -1.f + vPos.y, 0.5f + vPos.z);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_ColorCube::Set_VertexPos(D3DXVECTOR3 vPos, D3DXVECTOR3 vSize)
{
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(pVertices[0].vPosition.x + vPos.x*vSize.x, pVertices[0].vPosition.y + vPos.y*vSize.y, pVertices[0].vPosition.z + vPos.z*vSize.z);
	pVertices[1].vPosition = _float3(pVertices[1].vPosition.x + vPos.x*vSize.x, pVertices[1].vPosition.y + vPos.y*vSize.y, pVertices[1].vPosition.z + vPos.z*vSize.z);
	pVertices[2].vPosition = _float3(pVertices[2].vPosition.x + vPos.x*vSize.x, pVertices[2].vPosition.y + vPos.y*vSize.y, pVertices[2].vPosition.z + vPos.z*vSize.z);
	pVertices[3].vPosition = _float3(pVertices[3].vPosition.x + vPos.x*vSize.x, pVertices[3].vPosition.y + vPos.y*vSize.y, pVertices[3].vPosition.z + vPos.z*vSize.z);
	pVertices[4].vPosition = _float3(pVertices[4].vPosition.x + vPos.x*vSize.x, pVertices[4].vPosition.y + vPos.y*vSize.y, pVertices[4].vPosition.z + vPos.z*vSize.z);
	pVertices[5].vPosition = _float3(pVertices[5].vPosition.x + vPos.x*vSize.x, pVertices[5].vPosition.y + vPos.y*vSize.y, pVertices[5].vPosition.z + vPos.z*vSize.z);
	pVertices[6].vPosition = _float3(pVertices[6].vPosition.x + vPos.x*vSize.x, pVertices[6].vPosition.y + vPos.y*vSize.y, pVertices[6].vPosition.z + vPos.z*vSize.z);
	pVertices[7].vPosition = _float3(pVertices[7].vPosition.x + vPos.x*vSize.x, pVertices[7].vPosition.y + vPos.y*vSize.y, pVertices[7].vPosition.z + vPos.z*vSize.z);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_ColorCube::Return_VertexPos(D3DXVECTOR3 vPos)
{
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);

	m_pVB->Unlock();

	return S_OK;
}

void	CVIBuffer_ColorCube::Set_Color(const D3DXCOLOR& dwColor)
{
	for (int i = 0; i < VTXINDEX_LENGTH; ++i)
	{
		pVertices[i].dwColor = dwColor;
	}
}

CVIBuffer_ColorCube* CVIBuffer_ColorCube::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CVIBuffer_ColorCube* pInstance = new CVIBuffer_ColorCube(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CVIBuffer_ColorCube::Clone(void * pArg)
{
	CVIBuffer_ColorCube* pInstance = new CVIBuffer_ColorCube(*this);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

void CVIBuffer_ColorCube::Free()
{
	__super::Free();
}
