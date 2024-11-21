#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

#define VTXINDEX_LENGTH 8

class ENGINE_DLL CVIBuffer_ColorCube final : public CVIBuffer
{
public:
	CVIBuffer_ColorCube(LPDIRECT3DDEVICE9 pGraphicDevice);
	CVIBuffer_ColorCube(const CVIBuffer_ColorCube& rhs);
	virtual ~CVIBuffer_ColorCube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT Set_VertexResize(D3DXVECTOR3 vSize);
	HRESULT Set_VertexPosCubeCreate(D3DXVECTOR3 vPos);
	HRESULT Set_VertexPos(D3DXVECTOR3 vPos, D3DXVECTOR3 vSize);
	HRESULT Return_VertexPos(D3DXVECTOR3 vPos);

	FACEINDICES16* Get_Indices() { return pIndices; }
	VERTEXCOLOR* Get_Vertices() { return pVertices; }

	void	Set_Color(const D3DXCOLOR& dwColor);

protected:
	VERTEXCOLOR* pVertices = nullptr;
	FACEINDICES16* pIndices = nullptr;

public:
	static CVIBuffer_ColorCube* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END