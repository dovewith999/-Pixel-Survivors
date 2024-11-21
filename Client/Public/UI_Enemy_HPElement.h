#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Enemy_HPElement final : public CUIObject
{
private:
	explicit CUI_Enemy_HPElement(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Enemy_HPElement(const CUI_Enemy_HPElement& rhs);
	virtual ~CUI_Enemy_HPElement() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Show(bool bShow) { m_bShow = bShow; }
	void Set_Enemy_Transform(CTransform* pEnemyTransform) { m_pHPBarTransform = pEnemyTransform; }
	void Set_HP_UI(float iHP);

private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pHPBarTransform = nullptr;

private:
	float					m_iHP = 1.f;
	bool					m_bShow = true;
	float					m_fDistance = 0.f;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CUI_Enemy_HPElement* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END