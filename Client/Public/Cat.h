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

class CCat final : public CEffect
{
private:
	explicit CCat(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCat(const CCat& rhs);
	virtual ~CCat() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void CreateAttackCollision();

private:
	int m_iEffect = 0;

	float m_fEffectCool = 0.f;

	float m_fAttackDelay = 0.f;
	float m_fDeadTime = 6.f;

	float m_fTargetSearchCool = 0.f;
	D3DXVECTOR3 m_vTargetPos;
	D3DXVECTOR3	  m_vCollisionScale = D3DXVECTOR3(1.f, 0.5f, 0.7f);

	CGameObject*			m_pTarget = nullptr;

protected:
	HRESULT SetUp_Components(ACTIVE_RECIEVE_DATA eData);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CCat* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END