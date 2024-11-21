#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Tile final : public CVIBuffer
{
public:
	explicit CVIBuffer_Tile(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CVIBuffer_Tile(const CVIBuffer& rhs);
	virtual ~CVIBuffer_Tile() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

protected:
	VERTEXTXT* m_pVertices = nullptr; // ¼ö¿ìÁ¤
	FACEINDICES16* m_pIndices = nullptr;


public:
	FACEINDICES16* Get_Indices() { return m_pIndices; }
	VERTEXTXT* Get_Vertices() { return m_pVertices; }

public:
	static CVIBuffer_Tile* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END