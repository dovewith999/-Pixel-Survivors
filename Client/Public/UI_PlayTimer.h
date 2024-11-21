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
class CPlayer;
class CUI_PlayTimer final : public CUIObject
{
public:
	explicit CUI_PlayTimer(LPDIRECT3DDEVICE9 _GraphicDevice);
	explicit CUI_PlayTimer(const CUI_PlayTimer& rhs);
	virtual ~CUI_PlayTimer() override = default;
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

	LPD3DXSPRITE			m_pSprite = nullptr;
	LPD3DXFONT				m_pFont = nullptr;

private:
	_uint					m_iTextureNum = 0;
	_float					m_fFrame = 0.f;
	_float4x4				m_ProjMatrix;
	_float					m_fSizeX, m_fSizeY;
	_float2					m_vPos; // m_fX, m_fY
	_int					m_iSecondNum1 = 0; //05/09 ¿”»Òº∑ √ﬂøÏ∞°
	_int					m_iSecondNum2 = 0;
	_int					m_iMinuteNum1 = 0;
	_int					m_iMinuteNum2 = 0;
	_float					m_fTimeAcc = 0.f;

private:
	CPlayer* m_pPlayer = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CUI_PlayTimer* Create(LPDIRECT3DDEVICE9 _GraphicDevice);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
END
