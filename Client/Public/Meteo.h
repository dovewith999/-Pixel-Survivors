#pragma once

#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CBattle;
END

BEGIN(Client)
class CMeteo final : public CEffect
{
private:
	explicit CMeteo(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMeteo(const CMeteo& rhs);
	virtual ~CMeteo() = default;

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
	float m_fDeadTime = 3.f;
	float m_fSpeed = 1.f;

	int m_iEffect = 0;
	D3DXVECTOR3	  m_vCollisionScale = D3DXVECTOR3(0.8f, 0.8f, 0.8f);

	CGameObject*			m_pTarget = nullptr;

protected:
	HRESULT SetUp_Components(ACTIVE_RECIEVE_DATA eData);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CMeteo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END