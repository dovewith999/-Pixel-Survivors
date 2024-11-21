#pragma once

#include "Base.h"

/* ���� �����ϴ� ��� : ���� �������� ����ϴ� �ڿ����� �����Ѵ�. */
/* ���� �����ִ� ������ �����Ѵ�.  */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager() = default;
	virtual ~CLevel_Manager() = default;

public:
	/* ���� ��ü�Ѵ�. */
	HRESULT Open_Level(_uint iLevelID, class CLevel* pNewLevel);
	void Tick(_float fTimeDelta);
	HRESULT Render();	

private:
	_uint					m_iLevelID = 0;
	class CLevel*			m_pCurrentLevel = nullptr;

public:
	virtual void Free() override;
	
};

END