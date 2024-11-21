#pragma once

#include "Client_Defines.h"
#include "Effect.h"
#include "Terrain.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
class CCollision;
class CBattle;
END

BEGIN(Client)

class CFireBall final : public CEffect
{
private:
	explicit CFireBall(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFireBall(const CFireBall& rhs);
	virtual ~CFireBall() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void CreateAttackCollision();

private:
	float m_fAttackDelay = 0.f;
	float m_fDeadTime = 10.f;
	int m_iEffect = 0;
	bool m_bAttacted = false;
	D3DXVECTOR3	  m_vCollisionScale = D3DXVECTOR3(0.7f, 0.7f, 0.7f);
	_uint m_iTerrainIndex;

	CTerrain* m_pTerrain = nullptr;
	CGameObject*			m_pTarget = nullptr;

private:
	CCollider*				m_pColliderCom = nullptr;
	CCollision*				m_pCollisionCom = nullptr;

protected:
	HRESULT SetUp_Components(ACTIVE_RECIEVE_DATA eData);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CFireBall* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END