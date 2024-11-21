#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	explicit CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevelID);
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Layer_Loading(const _tchar* pLayerTag);
private:
	class CLoader*			m_pLoader = nullptr;
	LEVEL					m_eNextLevelID = LEVEL_END;

public:
	static CLevel_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;
};

END

