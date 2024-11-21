#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_dwFVF(rhs.m_dwFVF)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_ePrimitiveType(rhs.m_ePrimitiveType)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iStride(rhs.m_iStride)
	, m_pIB(rhs.m_pIB)	
	, m_iPrimitiveIndicesSize(rhs.m_iPrimitiveIndicesSize)
	, m_pVerticesPos(rhs.m_pVerticesPos)
	, m_pTexture(rhs.m_pTexture)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{	
	this->m_pVertices = rhs.m_pVertices;
	this->m_pIndices = rhs.m_pIndices;

	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pTexture);

}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pVB)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetFVF(m_dwFVF)))
		return E_FAIL;	

	if (FAILED(m_pGraphic_Device->SetIndices(m_pIB)))
		return E_FAIL;


	return m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);	
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;	

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iPrimitiveIndicesSize * m_iNumPrimitive, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Set_Texture(const TCHAR* pTag)
{
	if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, pTag/*L"../Texture/1.png"*/, &m_pTexture)))
		return E_FAIL;

	m_pGraphic_Device->SetTexture(0, m_pTexture);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CVIBuffer::Release_Texture()
{

	m_pGraphic_Device->SetTexture(0, nullptr);

	return S_OK;
}


void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
	Safe_Release(m_pTexture);

	if(false == m_isCloned)
		Safe_Delete_Array(m_pVerticesPos);


}
