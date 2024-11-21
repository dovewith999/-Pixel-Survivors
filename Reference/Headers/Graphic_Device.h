#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:


private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;	

public:
	HRESULT		InitDevice(HWND hWnd, GRAPHIC_DEVICE_DESC::MODE eWinMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppOut);
	void		Render_Begin(void);
	void		Render_End(HWND hWnd = 0);	

private:
	LPDIRECT3D9				m_p3D = nullptr;
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;

	LPD3DXSPRITE			m_pSprite = nullptr;
	LPD3DXFONT				m_pFont = nullptr;

private:
	void	SetParameters(D3DPRESENT_PARAMETERS& d3dpp, HWND hWnd, GRAPHIC_DEVICE_DESC::MODE eWinMode, _uint iWinCX, _uint iWinCY);

public:
	virtual void Free() override;
};

END