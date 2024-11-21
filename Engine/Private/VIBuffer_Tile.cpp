#include "..\Public\VIBuffer_Tile.h"

CVIBuffer_Tile::CVIBuffer_Tile(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
}

CVIBuffer_Tile::CVIBuffer_Tile(const CVIBuffer & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Tile::NativeConstruct_Prototype()
{
	m_iNumVertices = 4;
	m_iStride = sizeof(VERTEXTXT);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	//VERTEXTXT* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&m_pVertices, 0);

	m_pVertices[0].vPosition = _float3(-0.5f, 0.f, 0.5f);
	m_pVertices[0].vTexUV = _float2(0.f, 0.f);

	m_pVertices[1].vPosition = _float3(0.5f, 0.f, 0.5f);
	m_pVertices[1].vTexUV = _float2(1.f, 0.f);

	m_pVertices[2].vPosition = _float3(0.5f, 0.f, -0.5f);
	m_pVertices[2].vTexUV = _float2(1.f, 1.f);

	m_pVertices[3].vPosition = _float3(-0.5f, 0.0f, -0.5f);
	m_pVertices[3].vTexUV = _float2(0.f, 1.f);

	m_pVB->Unlock();

#pragma region INDEX_BUFFER
	m_iNumPrimitive = 2;
	m_iPrimitiveIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&m_pIndices, 0);

	m_pIndices[0]._0 = 0;
	m_pIndices[0]._1 = 1;
	m_pIndices[0]._2 = 2;

	m_pIndices[1]._0 = 0;
	m_pIndices[1]._1 = 2;
	m_pIndices[1]._2 = 3;

	m_pIB->Unlock();

#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Tile::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Tile * CVIBuffer_Tile::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CVIBuffer_Tile* pInstance = new CVIBuffer_Tile(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(L"Failed to Created : CVIBuffer_Tile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Tile::Clone(void * pArg)
{
	CVIBuffer_Tile* pInstance = new CVIBuffer_Tile(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(L"Failed to Cloned : CVIBuffer_Tile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Tile::Free()
{
	__super::Free();
}
