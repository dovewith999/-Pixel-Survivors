#pragma once

#include "VIBuffer.h"

/* 네모를 그리기위한 클래스다. */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	_float Get_Height(_float3 vPosition);
	// CH 추가
	_uint Get_TerrainCX() { return m_iNumVerticesX; }
	_uint Get_TerrainCZ() { return m_iNumVerticesZ; }

public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pHeightMapPath);
	virtual HRESULT NativeConstruct(void* pArg) override;

private:
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ);
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapPath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END