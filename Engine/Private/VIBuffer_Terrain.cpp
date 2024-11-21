#include "..\Public\VIBuffer_Terrain.h"


CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{
}

_float CVIBuffer_Terrain::Get_Height(_float3 vPosition)
{
	_uint iIndex = _uint(vPosition.z) * m_iNumVerticesX + _uint(vPosition.x);

	_uint iIndices[] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex
	};

	_float		fWidth = vPosition.x - m_pVerticesPos[iIndices[0]].x;
	_float		fDepth = m_pVerticesPos[iIndices[0]].z - vPosition.z;

	/* ax + by + cz + d = 0 */
	/* y = (- ax - cz - d) / b */

	D3DXPLANE			Plane;

	/* 오른쪽 위 삼각형. */
	if (fWidth > fDepth)
	{
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[2]]);
	}
	/* 왼쪽 하단. */
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[2]], &m_pVerticesPos[iIndices[3]]);
	}

	return (-Plane.a * vPosition.x - Plane.c * vPosition.z - Plane.d) / Plane.b;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{

	//#pragma region VERTEX_BUFFER
	//	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	//	m_iStride = sizeof(VTXTEX);
	//	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	//	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	//	m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;
	//
	//	m_iNumVerticesX = iNumVerticesX;
	//	m_iNumVerticesZ = iNumVerticesZ;
	//
	//	if (FAILED(__super::Create_VertexBuffer()))
	//		return E_FAIL;
	//
	//	VTXTEX*		pVertices = nullptr;
	//
	//	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	//
	//	
	//	for (_uint i = 0; i < iNumVerticesZ; ++i)
	//	{
	//		for (_uint j = 0; j < iNumVerticesX; ++j)
	//		{
	//			_uint iIndex = i * iNumVerticesX + j;
	//
	//			pVertices[iIndex].vPosition = _float3((float)j, 0.0f, (float)i);
	//			pVertices[iIndex].vTexUV = _float2((j / _float(iNumVerticesX - 1)) * 20.f, (i / (_float(iNumVerticesZ - 1)) * 20.f));
	//		}
	//	}
	//	
	//
	//	m_pVB->Unlock();
	//
	//#pragma endregion
	//
	//#pragma region INDEX_BUFFER
	//	m_iPrimitiveIndicesSize = sizeof(FACEINDICES32);
	//	m_eIndexFormat = D3DFMT_INDEX32;
	//
	//	if (FAILED(__super::Create_IndexBuffer()))
	//		return E_FAIL;
	//
	//	_uint			iNumFace = 0;
	//
	//	FACEINDICES32*		pIndices = nullptr;
	//
	//	m_pIB->Lock(0, 0, (void**)&pIndices, 0);
	//
	//	for (_uint i = 0; i < iNumVerticesZ - 1; ++i)
	//	{
	//		for (_uint j = 0; j < iNumVerticesX - 1; ++j)
	//		{
	//			_uint		iIndex = i * iNumVerticesX + j;
	//
	//			_uint		iIndices[] = {
	//				iIndex + iNumVerticesX,
	//				iIndex + iNumVerticesX + 1,
	//				iIndex + 1,
	//				iIndex
	//			};
	//
	//			pIndices[iNumFace]._0 = iIndices[0];
	//			pIndices[iNumFace]._1 = iIndices[1];
	//			pIndices[iNumFace]._2 = iIndices[2];
	//			++iNumFace;
	//
	//			pIndices[iNumFace]._0 = iIndices[0];
	//			pIndices[iNumFace]._1 = iIndices[2];
	//			pIndices[iNumFace]._2 = iIndices[3];
	//			++iNumFace;
	//		}
	//	}
	//
	//	m_pIB->Unlock();
	//
	//
	//#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar * pHeightMapPath)
{
	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER			fh;
	BITMAPINFOHEADER			ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*		pPixel = new _ulong[ih.biWidth * ih.biHeight];
	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

#pragma region VERTEX_BUFFER
	m_iNumVertices = ih.biWidth * ih.biHeight;

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1)*/;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_pVerticesPos = new _float3[m_iNumVertices];
	ZeroMemory(m_pVerticesPos, sizeof(_float3) * m_iNumVertices);

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);


	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3((float)j, (pPixel[iIndex] & 0x000000ff) / 10.0f, (float)i);
			pVertices[iIndex].vTexUV = _float2(((float)j / _float(m_iNumVerticesX - 1)) * 1.f, ((float)i / (_float(m_iNumVerticesZ - 1)) * 1.f));
		}
	}


	m_pVB->Unlock();

#pragma endregion

#pragma region INDEX_BUFFER
	m_iPrimitiveIndicesSize = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_uint			iNumFace = 0;

	FACEINDICES32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[1];
			pIndices[iNumFace]._2 = iIndices[2];
			++iNumFace;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[2];
			pIndices[iNumFace]._2 = iIndices[3];
			++iNumFace;
		}
	}

	m_pIB->Unlock();


#pragma endregion

	Safe_Delete_Array(pPixel);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		_float3 vSize = *(_float3*)pArg;

#pragma region VERTEX_BUFFER
		m_iNumVertices = (_uint)(vSize.x * vSize.z);
		m_iStride = sizeof(VERTEXTXT);
		m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
		m_ePrimitiveType = D3DPT_TRIANGLELIST;
		m_iNumPrimitive = (_uint)(vSize.x * vSize.z * 2);

		m_iNumVerticesX = (_uint)vSize.x;
		m_iNumVerticesZ = (_uint)vSize.z;

		if (FAILED(__super::Create_VertexBuffer()))
			return E_FAIL;

		VERTEXTXT*		pVertices = nullptr;

		m_pVB->Lock(0, 0, (void**)&pVertices, 0);

		for (_uint i = 0; i < m_iNumVerticesZ; ++i)
		{
			for (_uint j = 0; j < m_iNumVerticesX; ++j)
			{
				_uint iIndex = i * m_iNumVerticesX + j;

				pVertices[iIndex].vPosition = _float3(j, 0.0f, i);

				if (m_iNumVerticesX >= m_iNumVerticesZ)
					pVertices[iIndex].vTexUV = _float2((j / _float(m_iNumVerticesX - 1)) * 50.f, (i / (_float(m_iNumVerticesX - 1)) * 50.f));
				else
					pVertices[iIndex].vTexUV = _float2((j / _float(m_iNumVerticesZ - 1)) * 40.f, (i / (_float(m_iNumVerticesZ - 1)) * 40.f));
			}
		}


		m_pVB->Unlock();

#pragma endregion

#pragma region INDEX_BUFFER
		m_iPrimitiveIndicesSize = sizeof(FACEINDICES32);
		m_eIndexFormat = D3DFMT_INDEX32;

		if (FAILED(__super::Create_IndexBuffer()))
			return E_FAIL;

		_uint			iNumFace = 0;

		FACEINDICES32*		pIndices = nullptr;

		m_pIB->Lock(0, 0, (void**)&pIndices, 0);

		for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
		{
			for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
			{
				_uint		iIndex = i * m_iNumVerticesX + j;

				_uint		iIndices[] = {
					iIndex + m_iNumVerticesX,
					iIndex + m_iNumVerticesX + 1,
					iIndex + 1,
					iIndex
				};

				pIndices[iNumFace]._0 = iIndices[0];
				pIndices[iNumFace]._1 = iIndices[1];
				pIndices[iNumFace]._2 = iIndices[2];
				++iNumFace;

				pIndices[iNumFace]._0 = iIndices[0];
				pIndices[iNumFace]._1 = iIndices[2];
				pIndices[iNumFace]._2 = iIndices[3];
				++iNumFace;
			}
		}

		m_pIB->Unlock();
#pragma endregion
	}
	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSGBOX(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pHeightMapPath)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(pHeightMapPath)))
	{
		MSGBOX(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}
