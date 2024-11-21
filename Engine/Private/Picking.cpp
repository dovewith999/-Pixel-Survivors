#include "..\Public\Picking.h"
#include "VIBuffer.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
}

HRESULT CPicking::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd, _uint iWinCX, _uint iWinCY)
{
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);

	m_hWnd = hWnd;

	m_iWinCX = iWinCX;
	m_iWinCY = iWinCY;

	return S_OK;
}

void CPicking::Transform_ToWorldSpace()
{
	POINT		ptMouse;

	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	_float3		vMousePos;

	vMousePos.x = ((_float)ptMouse.x / (m_iWinCX >> 1)) - 1;
	vMousePos.y = ((_float)ptMouse.y / (m_iWinCY >> 1)) * -1 + 1;
	vMousePos.z = 0.f;

	_float4x4	ProjMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);
	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);

	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &ProjMatrix);

	_float3		vRayDir, vRayPos;
	vRayPos = _float3(0.f, 0.f, 0.f);
	vRayDir = vMousePos;

	_float4x4	ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	D3DXVec3TransformNormal(&m_vWorldRayDir, &vRayDir, &ViewMatrix);
	D3DXVec3TransformCoord(&m_vWorldRayPos, &vRayPos, &ViewMatrix);

}

bool CPicking::Picking(CVIBuffer * pVIBuffer, CTransform * pTransform, _float3 * pOut)
{
	_float3		vRayPos = *D3DXVec3TransformCoord(&vRayPos, &m_vWorldRayPos, &pTransform->Get_WorldMatrixInverse());
	_float3		vRayDir = *D3DXVec3TransformNormal(&vRayDir, &m_vWorldRayDir, &pTransform->Get_WorldMatrixInverse());
	D3DXVec3Normalize(&vRayDir, &vRayDir);

	const _float3* pVerticesPos = pVIBuffer->Get_VerticesPos();

	void* pIndices = pVIBuffer->Get_Indices();

	_uint	iNumFaces = pVIBuffer->Get_NumPrimitive();

	D3DFORMAT		eFormat = pVIBuffer->Get_IndexFormat();

	_uint		iIndexSize = eFormat == D3DFMT_INDEX16 ? 2 : 4;

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint		iIndices[3] = { 0 };

		for (_uint j = 0; j < 3; ++j)
		{
			if (iIndexSize == 2)
				iIndices[j] = *(_ushort*)((((_byte*)pIndices) + (iIndexSize * 3) * i) + iIndexSize * j);
			else
				iIndices[j] = *(_ulong*)((((_byte*)pIndices) + (iIndexSize * 3) * i) + iIndexSize * j);
		}

		_float		fU, fV, fDist;

		if (TRUE == D3DXIntersectTri(&pVerticesPos[iIndices[0]], &pVerticesPos[iIndices[1]], &pVerticesPos[iIndices[2]],
			&vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			_float3	vPickPos = vRayPos + vRayDir * fDist;

			D3DXVec3TransformCoord(pOut, &vPickPos, &pTransform->Get_WorldMatrix());

			return true;
		}

	}

	return false;
}

void CPicking::Free()
{
	Safe_Release(m_pGraphic_Device);
}
