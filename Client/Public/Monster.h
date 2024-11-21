#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "MasterCube.h"
#include "UI_Enemy_HPBar.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CCollider;
class CCollision;
class CBattle;
class CAniTransform;
END

BEGIN(Client)

class MyTerrain;
class CPlayer;
class CMonster abstract : public CLandObject
{
public:
	enum TYPE { TYPE_TWOFEET, TYPE_FOURFEET, TYPE_WINGS, TYPE_END };
	enum STATE { STATE_IDLE, STATE_WALK, STATE_ATTACK, STATE_DIE, STATE_END };
	enum ATTACKMOTION { ATKMOTION_1, ATKMOTION_2, ATKMOTION_END };
	enum LEVEL { LEVEL_1, LEVEL_2, LEVEL_3, LEVEL_END };

protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Search_Target(const _tchar* pTargetTag);
	void	Attack_Target();
	void	AttackMotion1();

public:

	// 0511
	void	Set_FakeDead(); // ¿”Ω√
	void	Set_Freeze() { m_bFreeze = true; }
	HRESULT	Create_Item();

public:
	HRESULT Set_MasterCube(CMasterCube* MasterCube);
	HRESULT Set_HPBar(CUI_Enemy_HPBar* HPBar);
	HRESULT Set_BindingTransform_HPBar();
	HRESULT Set_BindingTransform_HPBar(CAniTransform* _pAniTransform);

protected:
	bool	m_bHpShow = false;
	_float m_fHpBarShowLimit = 0.5f;
	_float m_fHpBarShowTime = 0.f;
	int  m_iHp = 0;
protected:
	bool	m_bFreeze = false;
	_float	m_fFreezeLimit = 10.f;
	_float	m_fFreezeTime = 0.f;

protected:
	bool	m_bAttacked = false;
	_float	m_fAttackLimit = 1.f;
	_float	m_fAttackTime = 0.f;


protected:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CBattle*				m_pBattleCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CCollision*				m_pCollisionCom = nullptr;

	CShader*				m_pShaderCom = nullptr;

protected:
	TEST_STATE m_eAniState = TEST_STATE_IDLE;
	TEST_STATE m_eAniMemoState = TEST_STATE_END;

protected:
	LEVEL					m_eLevel = LEVEL_1;

protected:
	CMasterCube*			m_MasterCube = nullptr;
	CUI_Enemy_HPBar*		m_pHPBar = nullptr;


protected:
	int						m_iAttackDelay = 0;
	ATTACKMOTION			m_eAtkMotion = ATKMOTION_END;
	float					m_fAttackAngle = 0.f;

protected:
	STATE					m_eState = STATE_IDLE;
	CGameObject*			m_pTarget = nullptr;
	CPlayer*				m_pPlayer = nullptr; // 05/04 ¿”»Òº∑ √ﬂøÏ∞°
	CAniTransform*			m_pAniTransformCom;


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END