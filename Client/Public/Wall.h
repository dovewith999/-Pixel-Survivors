#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
class CCollider;
class CCollision;
END

class CWall final : public CGameObject
{
public:
	enum ROLE { ROLE_BACK, ROLE_FRONT, ROLE_LEFT, ROLE_RIGHT, ROLE_END };

private:
	explicit CWall(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CWall(const CWall& rhs);
	virtual ~CWall() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Role(ROLE eRole);
	void Set_Position(const _float3& vPosition);
	void Rotation(const _float3& vAxis, _float fRadian);

public:
	WALL m_tTerrain;
	//ROLE m_eRole;
	_float3 m_vColliderSize;

public:
	_uint					m_iTileCountX = 0;
	_uint					m_iTileCountY = 0;
	_uint					m_iTileCountZ = 0;

private:
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	CCollider*				m_pColliderCom = nullptr;
	CCollision*				m_pCollisionCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CWall* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};