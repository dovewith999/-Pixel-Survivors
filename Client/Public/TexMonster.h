#pragma once

#include "Monster.h"

BEGIN(Client)
class CTexMonster final : public CMonster
{
public:
	enum TYPE { TYPE_TWOFEET, TYPE_FOURFEET, TYPE_WINGS, TYPE_END };
	enum STATE { STATE_IDLE, STATE_WALK, STATE_ATTACK, STATE_DIE, STATE_END };
	enum ATTACKMOTION { ATKMOTION_1, ATKMOTION_2, ATKMOTION_END };

private:
	explicit CTexMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexMonster(const CTexMonster& rhs);
	virtual ~CTexMonster() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

	float m_fFrame = 0;
	TEXMONSTER m_eTexKind = MONSTER_BAT;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CTexMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END