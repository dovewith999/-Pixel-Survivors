#pragma once

#include "Component.h"

/* 각종 모델 클ㅇ래스들의 부모가 되는 클래스다. */
/* V : Vertex */
/* I : Index */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render();

public:
	const _float3* Get_VerticesPos() {
		return m_pVerticesPos;
	}

	void* Get_Indices() {
		return m_pIndices;
	}

	_uint Get_NumPrimitive() {
		return m_iNumPrimitive;
	}

	D3DFORMAT Get_IndexFormat() {
		return m_eIndexFormat;
	}


protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DTEXTURE9			m_pTexture = nullptr;
	_uint						m_iNumVertices = 0;
	_uint						m_iStride = 0; /* 정점구조체하나의 크기(byte)*/
	_ulong						m_dwFVF = 0;
	D3DPRIMITIVETYPE			m_ePrimitiveType;
	_uint						m_iNumPrimitive = 0;

	_float3*					m_pVerticesPos = nullptr;
	_float3*		        	m_pIndecsPos = nullptr;

protected:
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
	_uint						m_iPrimitiveIndicesSize = 0;
	D3DFORMAT					m_eIndexFormat;

protected: // 임희섭 추우가
	VERTEXTXT*					m_pVertices = nullptr;
	FACEINDICES16*				m_pIndices = nullptr;

protected:
	_uint						m_iNumVerticesX = 0;
	_uint						m_iNumVerticesZ = 0;


public:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();
	HRESULT Set_Texture(const TCHAR* pTag);
	HRESULT Release_Texture(); // 추가

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END