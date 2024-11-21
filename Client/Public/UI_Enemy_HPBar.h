#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "UI_Enemy_HPElement.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CAniTransform;
END

BEGIN(Client)

class CUI_Enemy_HPBar final : public CUIObject
{
private:
	explicit CUI_Enemy_HPBar(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Enemy_HPBar(const CUI_Enemy_HPBar& rhs);
	virtual ~CUI_Enemy_HPBar() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Add_Element(CUI_Enemy_HPElement* Element);
	HRESULT Set_HPBar_UI(float iHP);
	void Set_Enemy_Transform(CTransform* pEnemyTransform);
	void Set_Enemy_Transform(CAniTransform * pEnemyTransform);
	void Set_Show(bool Show)
	{ 
		m_bShow = Show; 
		if(m_pHPElement)
			m_pHPElement->Set_Show(Show);
	}

private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pEnemyTransform = nullptr;
	CAniTransform*			m_pEnenmyAniTransform = nullptr;
private:
	_float2					m_vPos; // m_fX, m_fY
	float					m_fDistance = 0.f;
	bool					m_bShow = false;

	CUI_Enemy_HPElement*		m_pHPElement = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CUI_Enemy_HPBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END