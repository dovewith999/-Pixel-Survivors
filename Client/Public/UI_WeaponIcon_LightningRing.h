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
class CUI_WeaponIcon_LightningRing final : public CUIObject
{
public:
	explicit CUI_WeaponIcon_LightningRing(LPDIRECT3DDEVICE9 _GraphicDevice);
	explicit CUI_WeaponIcon_LightningRing(const CUI_WeaponIcon_LightningRing& rhs);
	virtual ~CUI_WeaponIcon_LightningRing() override = default;

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

	const _tchar*			m_szWeaponIconName = L"LightningRing";
private:
	_float					m_fFrame = 0.f;
	_float4x4				m_ProjMatrix;
	_float					m_fSizeX, m_fSizeY;
	_float2					m_vPos; // m_fX, m_fY

private:
	CPlayer* m_pPlayer = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CUI_WeaponIcon_LightningRing* Create(LPDIRECT3DDEVICE9 _GraphicDevice);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

