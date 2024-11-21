#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

#define VTXINDEX_LENGTH 8
class ENGINE_DLL VIBuffer_TerrainCube final : public CVIBuffer
{
public:
	VIBuffer_TerrainCube(LPDIRECT3DDEVICE9 pGraphicDevice);
	VIBuffer_TerrainCube(const VIBuffer_TerrainCube& rhs);
	virtual ~VIBuffer_TerrainCube() override = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

public:
	void NativeConstruct_VertexHeight(D3DXVECTOR3* pVertexHeight);
	void Set_VertexHeight(_uint iDirection, _float fHeight);

protected:
	VERTEXTXT* m_pVertices = nullptr; // ¼ö¿ìÁ¤
	FACEINDICES16* m_pIndices = nullptr;
	_float3 m_CubePos;

public: // ÀÓÈñ¼· Ãß¿ì°¡ 
	bool	Check_Pick(RAY ray, float& tX, float& tY, float& tDisk);
	FACEINDICES16* Get_Indices() { return m_pIndices; }
	VERTEXTXT* Get_Vertices() { return m_pVertices; }
	_float3* Get_VerticesPos() { return m_pVerticesPos; }
	_float Get_Height(_float3 vPosition);
	void Set_VerticesPos(_float3 vCubePos) { m_CubePos = vCubePos; }

public:
	static VIBuffer_TerrainCube* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END