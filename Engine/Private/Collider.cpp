#include "..\Public\Collider.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_dwFVF(rhs.m_dwFVF)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_ePrimitiveType(rhs.m_ePrimitiveType)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iStride(rhs.m_iStride)
	, m_pIB(rhs.m_pIB)	
	, m_iPrimitiveIndicesSize(rhs.m_iPrimitiveIndicesSize)
	//, m_pVerticesPos(rhs.m_pVerticesPos)
	//, m_pVertices(rhs.m_pVertices)
{	
	this->m_pIndices = rhs.m_pIndices;
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);

	//m_pVertices = new COLLIDER[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVB->Lock(0, 0, (void**)&m_pVertices, 0);

	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_pVertices[i].vPosition = m_pVerticesPos[i] = rhs.m_pVerticesPos[i];

	m_pVB->Unlock();

	m_vMinPoint = m_pVerticesPos[3];
	m_vMaxPoint = m_pVerticesPos[5];
}

HRESULT CCollider::NativeConstruct_Prototype()
{
#pragma region VERTEX_BUFFER
	m_iNumVertices = 8;
	m_iStride = sizeof(COLLIDER);
	m_dwFVF = D3DFVF_XYZ;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	//COLLIDER*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&m_pVertices, 0);

	m_pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, -0.5f);
	m_pVertices[1].vPosition = m_pVerticesPos[1] = _float3(0.5f, 0.5f, -0.5f);
	m_pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.5f, -0.5f, -0.5f);
	m_pVertices[3].vPosition = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, -0.5f);
	m_pVertices[4].vPosition = m_pVerticesPos[4] = _float3(-0.5f, 0.5f, 0.5f);
	m_pVertices[5].vPosition = m_pVerticesPos[5] = _float3(0.5f, 0.5f, 0.5f);
	m_pVertices[6].vPosition = m_pVerticesPos[6] = _float3(0.5f, -0.5f, 0.5f);
	m_pVertices[7].vPosition = m_pVerticesPos[7] = _float3(-0.5f, -0.5f, 0.5f);

	m_vMinPoint = m_pVerticesPos[3];
	m_vMaxPoint = m_pVerticesPos[5];
	m_vCenter = (m_pVerticesPos[5] - m_pVerticesPos[3]) / 2;
	m_vSize = _float3(1.f, 1.f, 1.f);

	m_pVB->Unlock();

#pragma endregion

#pragma region INDEX_BUFFER
	m_iPrimitiveIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16*		m_pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&m_pIndices, 0);

	/* +X */
	m_pIndices[0]._0 = 1; m_pIndices[0]._1 = 5; m_pIndices[0]._2 = 6;
	m_pIndices[1]._0 = 1; m_pIndices[1]._1 = 6; m_pIndices[1]._2 = 2;

	/* -X */
	m_pIndices[2]._0 = 4; m_pIndices[2]._1 = 0; m_pIndices[2]._2 = 3;
	m_pIndices[3]._0 = 4; m_pIndices[3]._1 = 3; m_pIndices[3]._2 = 7;

	/* +Y */
	m_pIndices[4]._0 = 4; m_pIndices[4]._1 = 5; m_pIndices[4]._2 = 1;
	m_pIndices[5]._0 = 4; m_pIndices[5]._1 = 1; m_pIndices[5]._2 = 0;

	/* -Y */
	m_pIndices[6]._0 = 3; m_pIndices[6]._1 = 2; m_pIndices[6]._2 = 6;
	m_pIndices[7]._0 = 3; m_pIndices[7]._1 = 6; m_pIndices[7]._2 = 7;

	/* +Z */
	m_pIndices[8]._0 = 5; m_pIndices[8]._1 = 4; m_pIndices[8]._2 = 7;
	m_pIndices[9]._0 = 5; m_pIndices[9]._1 = 7; m_pIndices[9]._2 = 6;

	/* -Z */
	m_pIndices[10]._0 = 0; m_pIndices[10]._1 = 1; m_pIndices[10]._2 = 2;
	m_pIndices[11]._0 = 0; m_pIndices[11]._1 = 2; m_pIndices[11]._2 = 3;

	m_pIB->Unlock();


#pragma endregion

	return S_OK;
}

HRESULT CCollider::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		_float3 vScale = *(_float3*)pArg;

		m_pVB->Lock(0, 0, (void**)&m_pVertices, 0);

		for (_uint i = 0; i < m_iNumVertices; ++i)
		{
			m_pVertices[i].vPosition.x = m_pVerticesPos[i].x *= vScale.x;
			m_pVertices[i].vPosition.y = m_pVerticesPos[i].y *= vScale.y;
			m_pVertices[i].vPosition.z = m_pVerticesPos[i].z *= vScale.z;
		}

		m_vMinPoint = m_pVerticesPos[3];
		m_vMaxPoint = m_pVerticesPos[5];
		m_vCenter = (m_pVerticesPos[5] - m_pVerticesPos[3]) / 2;
		m_vSize = vScale;

		m_pVB->Unlock();
	}

	return S_OK;
}

void CCollider::Set_Location(const _float3& vLocation)
{
	_float3 vMin = m_vMinPoint;
	_float3 vMax = m_vMaxPoint;
	_float3 vCenter = m_vCenter;

	m_vCenter = vLocation;
	m_vMinPoint = vLocation - (m_vSize / 2.f);
	m_vMaxPoint = vLocation + (m_vSize / 2.f);
}

void CCollider::Set_Position(const _float3& vPosition)
{
	m_vMinPoint = m_pVerticesPos[3];
	m_vMaxPoint = m_pVerticesPos[5];
	m_vCenter = (m_pVerticesPos[5] - m_pVerticesPos[3]) / 2;

	m_vMinPoint += vPosition;
	m_vMaxPoint += vPosition;
	m_vCenter += vPosition;
}

HRESULT CCollider::Render()
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

HRESULT CCollider::Create_VertexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;	

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::Create_IndexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iPrimitiveIndicesSize * m_iNumPrimitive, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	return S_OK;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider*	pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider*	pInstance = new CCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CCollider"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	if (true == m_isCloned)
	{
		//Safe_Delete_Array(m_pVertices); // 이거 땜에 릭나는데 삭제가 안됨!!!(두번 삭제한다 나옴)
	}
	Safe_Delete_Array(m_pVerticesPos);

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	//if(false == m_isCloned)
	//	Safe_Delete_Array(m_pVerticesPos);

}
