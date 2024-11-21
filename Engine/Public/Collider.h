#pragma once

#include "Component.h"

// 큐브 형태의 충돌체 컴포넌트

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render();

public:
	_float3& Get_MinPoint() { return m_vMinPoint; }
	_float3& Get_MaxPoint() { return m_vMaxPoint; }
	_float3& Get_Center() { return m_vCenter; }
	_float3& Get_Size() { return m_vSize; }

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

public:
	void Set_Location(const _float3& vLocation);
	void Set_Position(const _float3& vPosition);

private:
	_float3						m_vMinPoint;
	_float3						m_vMaxPoint;
	_float3						m_vCenter;
	_float3						m_vSize;

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;

	COLLIDER*					m_pVertices = nullptr;

	_uint						m_iNumVertices = 0;
	_uint						m_iStride = 0; /* 정점구조체하나의 크기(byte)*/
	_ulong						m_dwFVF = 0;
	D3DPRIMITIVETYPE			m_ePrimitiveType;
	_uint						m_iNumPrimitive = 0;

	_float3*					m_pVerticesPos = nullptr;
	FACEINDICES16*				m_pIndices = nullptr;

private:
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
	_uint						m_iPrimitiveIndicesSize = 0;
	D3DFORMAT					m_eIndexFormat = D3DFMT_INDEX16;

public:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END