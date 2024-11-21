#include "../Public/VIBuffer_Terrain_Cube.h"


VIBuffer_TerrainCube::VIBuffer_TerrainCube(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
}

VIBuffer_TerrainCube::VIBuffer_TerrainCube(const VIBuffer_TerrainCube & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT VIBuffer_TerrainCube::NativeConstruct_Prototype()
{

	m_iNumVertices = 8;
	m_iStride = sizeof(VERTEXTXT);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	m_pVB->Lock(0, 0, (void**)&m_pVertices, 0);

	//좌측하단
	m_pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, -0.5f);
	m_pVertices[0].vTexUV = _float2(0.f, 0.f);
	
	//우측하단
	m_pVertices[1].vPosition = m_pVerticesPos[1] = _float3(0.5f, 0.5f, -0.5f);
	m_pVertices[1].vTexUV = _float2(1.f, 0.f);

	m_pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.5f, -0.5f, -0.5f);
	m_pVertices[2].vTexUV = _float2(1.f, 1.f);

	m_pVertices[3].vPosition = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, -0.5f);
	m_pVertices[3].vTexUV = _float2(0.f, 1.f);

	//왼쪽상단
	m_pVertices[4].vPosition = m_pVerticesPos[4] = _float3(-0.5f, 0.5f, 0.5f);
	m_pVertices[4].vTexUV = _float2(1.f, 0.f);

	//우측상단
	m_pVertices[5].vPosition = m_pVerticesPos[5] = _float3(0.5f, 0.5f, 0.5f);
	m_pVertices[5].vTexUV = _float2(0.f, 0.f);

	m_pVertices[6].vPosition = m_pVerticesPos[6] = _float3(0.5f, -0.5f, 0.5f);
	m_pVertices[6].vTexUV = _float2(0.f, 1.f);

	m_pVertices[7].vPosition = m_pVerticesPos[7] = _float3(-0.5f, -0.5f, 0.5f);
	m_pVertices[7].vTexUV = _float2(1.f, 1.f);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&m_pIndices, 0);

	m_pIndices[0]._0 = 0;
	m_pIndices[0]._1 = 1;
	m_pIndices[0]._2 = 2;

	m_pIndices[1]._0 = 0;
	m_pIndices[1]._1 = 2;
	m_pIndices[1]._2 = 3;

	m_pIndices[2]._0 = 4;
	m_pIndices[2]._1 = 5;
	m_pIndices[2]._2 = 1;

	m_pIndices[3]._0 = 4;
	m_pIndices[3]._1 = 1;
	m_pIndices[3]._2 = 0;

	m_pIndices[4]._0 = 5;
	m_pIndices[4]._1 = 4;
	m_pIndices[4]._2 = 6;

	m_pIndices[5]._0 = 6;
	m_pIndices[5]._1 = 4;
	m_pIndices[5]._2 = 7;

	m_pIndices[6]._0 = 3;
	m_pIndices[6]._1 = 2;
	m_pIndices[6]._2 = 7;

	m_pIndices[7]._0 = 7;
	m_pIndices[7]._1 = 2;
	m_pIndices[7]._2 = 6;

	m_pIndices[8]._0 = 3;
	m_pIndices[8]._1 = 7;
	m_pIndices[8]._2 = 0;

	m_pIndices[9]._0 = 0;
	m_pIndices[9]._1 = 7;
	m_pIndices[9]._2 = 4;

	m_pIndices[10]._0 = 1;
	m_pIndices[10]._1 = 5;
	m_pIndices[10]._2 = 2;

	m_pIndices[11]._0 = 2;
	m_pIndices[11]._1 = 5;
	m_pIndices[11]._2 = 6;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT VIBuffer_TerrainCube::NativeConstruct(void * pArg)
{
	return S_OK;
}

void VIBuffer_TerrainCube::NativeConstruct_VertexHeight(D3DXVECTOR3* pVertexHeight)
{
	//if (nullptr == pVertexHeight)
	//	return;

	m_pVB->Lock(0, 0, (void**)&m_pVertices, 0);

	for (int i = 0; i < 8; ++i)
	{
		m_pVertices[i].vPosition.y = pVertexHeight[i].y;
		m_pVerticesPos[i].y = pVertexHeight[i].y;
	}

	m_pVB->Unlock();
}

void VIBuffer_TerrainCube::Set_VertexHeight(_uint iDirection, _float fHeight)
{
	m_pVB->Lock(0, 0, (void**)&m_pVertices, 0);

	if (0 == iDirection)
	{
		m_pVertices[4].vPosition.y += fHeight;
		m_pVertices[5].vPosition.y += fHeight;

		m_pVerticesPos[4].y += fHeight;
		m_pVerticesPos[5].y += fHeight;
	}

	if (2 == iDirection)
	{
		m_pVertices[0].vPosition.y += fHeight;
		m_pVertices[1].vPosition.y += fHeight;

		m_pVerticesPos[0].y += fHeight;
		m_pVerticesPos[1].y += fHeight;
	}

	if (3 == iDirection)
	{
		m_pVertices[0].vPosition.y += fHeight;
		m_pVertices[4].vPosition.y += fHeight;

		m_pVerticesPos[0].y += fHeight;
		m_pVerticesPos[4].y += fHeight;
	}

	if (1 == iDirection)
	{
		m_pVertices[1].vPosition.y += fHeight;
		m_pVertices[5].vPosition.y += fHeight;

		m_pVerticesPos[1].y += fHeight;
		m_pVerticesPos[5].y += fHeight;
	}

	m_pVB->Unlock();
}

bool VIBuffer_TerrainCube::Check_Pick(RAY ray, float& tX, float& tY, float& tDisk)
{
	for (int ti = 0; ti < 12; ++ti)
	{
		BOOL b = D3DXIntersectTri(&m_pVertices[m_pIndices[ti]._0].vPosition, &m_pVertices[m_pIndices[ti]._1].vPosition, &m_pVertices[m_pIndices[ti]._2].vPosition
			, &ray.vOrigin, &ray.vDirection, &tX, &tY, &tDisk);

		if (b)
			cout << "충돌" << endl;
	}

	return true;
}

_float VIBuffer_TerrainCube::Get_Height(_float3 vPosition)
{
	_uint iIndex = _uint(vPosition.z) * m_iNumVerticesX + _uint(vPosition.x);

	_uint iIndices[] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex
	};

	//m_pVerticesPos[4] *= TERRAIN_CUBE_SIZE;
	//m_pVerticesPos[5] *= TERRAIN_CUBE_SIZE;
	//m_pVerticesPos[1] *= TERRAIN_CUBE_SIZE;
	//m_pVerticesPos[0] *= TERRAIN_CUBE_SIZE;

	_float		fWidthRightUp = vPosition.x - (m_pVerticesPos[4].x + m_CubePos.x);
	_float		fDepthRightUp = (m_pVerticesPos[4].z + m_CubePos.z) - vPosition.z;

	_float		fWidthLeftDown = (m_pVerticesPos[1].x + m_CubePos.x) - vPosition.x;
	_float		fDepthLeftDown = vPosition.z - (m_pVerticesPos[1].z + m_CubePos.z);

	/* ax + by + cz + d = 0 */
	/* y = (- ax - cz - d) / b */

	D3DXPLANE			Plane;


	/* 오른쪽 위 삼각형. */
	if (fWidthRightUp > fDepthRightUp)
	{
		D3DXPlaneFromPoints(&Plane, &(m_pVerticesPos[4] + m_CubePos), &(m_pVerticesPos[5] + m_CubePos), &(m_pVerticesPos[1] + m_CubePos));
	}
	/* 왼쪽 하단. */
	else
	{
		D3DXPlaneFromPoints(&Plane, &(m_pVerticesPos[4] + m_CubePos ), &(m_pVerticesPos[1] + m_CubePos), &(m_pVerticesPos[0] + m_CubePos));
	}

	//cout << "왼쪽 상단 : " << m_pVerticesPos[4].y << "오른쪽 상단 : " << m_pVerticesPos[5].y << "왼쪽 하단 : " << m_pVerticesPos[0].y << "오른쪽 하단 : " << m_pVerticesPos[1].y << endl;

	//if (m_pVerticesPos[4].y == m_pVerticesPos[5].y && m_pVerticesPos[5].y == m_pVerticesPos[1].y &&  m_pVerticesPos[1].y == m_pVerticesPos[0].y )
	//{
	//	
	//	return (((-Plane.a * vPosition.x - Plane.c * vPosition.z - Plane.d) / Plane.b) * TERRAIN_CUBE_SIZE);// +1.f; // + ObjectSize;
	//}

	//else
	//{
		return ((-Plane.a * vPosition.x - Plane.c * vPosition.z - Plane.d) / Plane.b) * TERRAIN_CUBE_SIZE + 0.5f;
	//}
	

}

VIBuffer_TerrainCube* VIBuffer_TerrainCube::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	VIBuffer_TerrainCube* pInstance = new VIBuffer_TerrainCube(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * VIBuffer_TerrainCube::Clone(void * pArg)
{
	VIBuffer_TerrainCube* pInstance = new VIBuffer_TerrainCube(*this);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

void VIBuffer_TerrainCube::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pVerticesPos);


}
