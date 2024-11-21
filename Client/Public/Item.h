#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "Player.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
class CCollision;
END

BEGIN(Client)
class CItem abstract : public CLandObject
{
protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Player(CPlayer* pPlayer);

protected:
	ITEM	m_tItemInit;

protected:
	float	m_fFrame = 0.f;
	_uint	m_iCollisionCnt = 0;
	_float	m_fRange = 1.f;
	_float3 m_vDirection;
	bool	m_bReturn = false;

protected:
	CPlayer* m_pPlayer = nullptr;

protected:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pColliderCom = nullptr;
	CCollision*				m_pCollisionCom = nullptr;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
