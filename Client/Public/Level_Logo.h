#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
private:
	explicit CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float			m_fTimeAcc = 0.f;

private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Logo(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Start(const _tchar* pLayerTag);

public:
	static CLevel_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END

