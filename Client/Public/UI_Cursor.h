#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CPlayer;
class CUI_Cursor final : public CUIObject
{
public:
	explicit CUI_Cursor(LPDIRECT3DDEVICE9 _GraphicDevice);
	explicit CUI_Cursor(const CUI_Cursor& rhs);
	virtual ~CUI_Cursor() override = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

private:
	CPlayer* m_pPlayer = nullptr;

private:
	_uint					m_iTextureNum = 0;
	_float					m_fFrame = 0.f;
	_float4x4				m_ProjMatrix;
	_float					m_fSizeX, m_fSizeY;
	_float2					m_vPos; // m_fX, m_fY
	_float					m_iPlayerExp = 0.f;
	_float					m_fAccTimeDelta = 0.f;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CUI_Cursor* Create(LPDIRECT3DDEVICE9 _GraphicDevice);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

