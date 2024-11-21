#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CWeapon;
class CInventory final : public CBase
{
public:
	CInventory();
	virtual ~CInventory() override = default;

public:
	HRESULT NativeConstruct();

private:
	map<_uint, CWeapon*> m_Inventory;

public:
	static CInventory* Create();
	virtual void Free() override;
};

END

