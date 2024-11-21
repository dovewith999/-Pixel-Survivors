#include "stdafx.h"
#include "..\Public\Item.h"
#include "GameInstance.h"
#include "Player.h"


CItem::CItem(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLandObject(pGraphic_Device)
{
}

CItem::CItem(const CItem & rhs)
	: CLandObject(rhs)
{
}

HRESULT CItem::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CItem::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CItem::Tick(_float fTimeDelta)
{
}

void CItem::Late_Tick(_float fTimeDelta)
{
}

HRESULT CItem::Render()
{
	return S_OK;
}

void CItem::Set_Player(CPlayer* pPlayer)
{
	if (nullptr == pPlayer)
		return;

	m_pPlayer = pPlayer;

	Safe_AddRef(m_pPlayer);
}

void CItem::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pCollisionCom);
}
