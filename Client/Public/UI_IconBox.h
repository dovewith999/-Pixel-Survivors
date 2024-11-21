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
class CUI_WeaponIcon;
class CUI_IconBox final : public CUIObject
{
public:
	explicit CUI_IconBox(LPDIRECT3DDEVICE9 _GraphicDevice);
	explicit CUI_IconBox(const CUI_IconBox& rhs);
	virtual ~CUI_IconBox() override = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_uint Get_WeaponName() { return m_iTextureNum; }

private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

private:
	CPlayer* m_pPlayer = nullptr;
	LPD3DXSPRITE			m_pSprite = nullptr;
	LPD3DXFONT				m_pFont = nullptr;

private:
	_uint					m_iTextureNum = 0;
	_float					m_fFrame = 0.f;
	_float4x4				m_ProjMatrix;
	_float					m_fSizeX, m_fSizeY;
	_float2					m_vPos; // m_fX, m_fY
	_float					m_iPlayerExp = 0.f;
	_bool					m_bRand = false;
	_tchar*					m_szWeaponName = nullptr;
	_uint					m_iWeaponLevel = 0;
	int						m_iPosNum;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CUI_IconBox* Create(LPDIRECT3DDEVICE9 _GraphicDevice);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END