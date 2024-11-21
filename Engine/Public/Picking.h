#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd, _uint iWinCX, _uint iWinCY);
	void Transform_ToWorldSpace();
	bool Picking(class CVIBuffer* pVIBuffer, class CTransform* pTransform, _float3* pOut);
private:
	HWND				m_hWnd;
	_uint				m_iWinCX;
	_uint				m_iWinCY;
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;

private:
	_float3				m_vWorldRayDir, m_vWorldRayPos;	


public:
	virtual void Free() override;
};

END