#pragma once
#include "Client_Defines.h"
#include "LandObject.h"
#include "MasterCube.h"
#include "Weapon.h"


BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
class CCollision;
class CBattle;
END

BEGIN(Client)
class CAttackCollision final : public CLandObject
{
private:
	explicit CAttackCollision(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CAttackCollision(const CAttackCollision& rhs);
	virtual ~CAttackCollision() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override; // 04/26 ÀÓÈñ¼· ¼ö¿ìÁ¤
	virtual HRESULT Render() override;

public:

private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CBattle*				m_pBattleCom = nullptr;

	// CH Ãß°¡
	CCollider*				m_pColliderCom = nullptr;
	CCollision*				m_pCollisionCom = nullptr;

private:
	ATKCLASS m_AtkClass;
	_float3 m_vSize;

private:
	HRESULT SetUp_Components(ATTACK_COLLISION AtkColInfo);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CAttackCollision* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
