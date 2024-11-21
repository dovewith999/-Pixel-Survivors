#include "stdafx.h"
#include "..\Public\Inventory.h"
#include "../Public/Weapon.h"

CInventory::CInventory()
{
	
}


HRESULT CInventory::NativeConstruct()
{
	return S_OK;
}


CInventory * CInventory::Create()
{
	CInventory* pInstance = new CInventory();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX(L"Error : Failed Created Inventory Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInventory::Free()
{
	/*for (auto& pair : m_Inventory)
	{
		Safe_Release(pair.second);
		pair.second = nullptr;
	}

	m_Inventory.clear();*/
}