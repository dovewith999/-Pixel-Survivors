#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "UI_HPElement.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_HPBar final : public CUIObject
{
private:
	explicit CUI_HPBar(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_HPBar(const CUI_HPBar& rhs);
	virtual ~CUI_HPBar() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Add_Element(CUI_HPElement* Element);
	HRESULT Set_HPBar_UI(_uint iHP) {
		int ti = 0;
		for (auto& iter : m_HPElementList)
		{
			if (ti <= (int)iHP - 1 && iHP != 0)
				iter->Set_Show(true);
			else
				iter->Set_Show(false);
			++ti;
		}

		return S_OK;
	}

private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

private:
	_float4x4				m_ProjMatrix;
	_float					m_fSizeX, m_fSizeY;
	_float2					m_vPos; // m_fX, m_fY

	list<CUI_HPElement*>		m_HPElementList;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CUI_HPBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END