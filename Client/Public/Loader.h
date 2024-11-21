#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* ���� �Ҵ��� ���� ������ �ڿ����� �����Ѵ�. */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	explicit CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CriticalSection() const {
		return m_CriticalSection;
	}

	LEVEL Get_NextLevelID() const {
		return m_eNextLevelID;
	}

	const _tchar* Get_String() const {
		return m_szLoadingText;
	}

	_bool Get_isFinished() const {
		return m_isFinished;
	}

public:
	HRESULT NativeConstruct(LEVEL eNextLevelID);
	_uint Loading_ForLogo();
	_uint Loading_ForGamePlay();

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_CriticalSection;
	LEVEL				m_eNextLevelID = LEVEL_END;


	_tchar				m_szLoadingText[MAX_PATH] = TEXT("");
	_bool				m_isFinished = false;

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;
};

END