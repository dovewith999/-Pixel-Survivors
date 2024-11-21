#pragma once
#include "Client_Defines.h"
#include "Camera.h"
#include "Player.h"

BEGIN(Client)

class CCamera_Player final : public CCamera
{
protected:
	explicit CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Player(const CCamera_Player& rhs);
	virtual ~CCamera_Player() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_Player(CPlayer* pPlayer);


private:
	float m_fDistance = -10.f;
	CPlayer* m_pPlayer=nullptr;

public:
	static CCamera_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END