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
class CBible final : public CEffect
{
private:
	explicit CBible(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBible(const CBible& rhs);
	virtual ~CBible() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void CreateAttackCollision();
	void Get_TurnVector(D3DXVECTOR3 vAxis, _float3 &vLook, float fAngle);

private:
	float m_fAttackDelay = 0.f;
	float m_fDeadTime = 3.f;
	float m_fSpeed = 1.f;

	int m_iEffect = 0;
	D3DXVECTOR3	  m_vCollisionScale = D3DXVECTOR3(0.8f, 0.8f, 0.8f);
	D3DXVECTOR3	  m_vLook;

	CGameObject*			m_pTarget = nullptr;

protected:
	HRESULT SetUp_Components(ACTIVE_RECIEVE_DATA eData);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CBible* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END