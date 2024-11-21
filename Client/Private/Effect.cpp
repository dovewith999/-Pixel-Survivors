#include "stdafx.h"
#include "..\Public\Effect.h"
#include "GameInstance.h"

#include "Player.h"
#include "../Public/MyTerrain.h"


CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEffect::CEffect(const CEffect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CEffect::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CEffect::Tick(_float fTimeDelta)
{
}

void CEffect::Late_Tick(_float fTimeDelta)
{
}

HRESULT CEffect::Render()
{
	return S_OK;
}

void CEffect::Free()
{
	__super::Free();


}
