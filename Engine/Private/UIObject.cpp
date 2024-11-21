#include "..\Public\UIObject.h"


CUIObject::CUIObject(LPDIRECT3DDEVICE9 _GraphicDevice)
	: CGameObject(_GraphicDevice)
{
}

CUIObject::CUIObject(const CUIObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUIObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CUIObject::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CUIObject::Tick(_float fTimeDelta)
{
}

void CUIObject::Late_Tick(_float fTimeDelta)
{
}

HRESULT CUIObject::Render()
{
	return S_OK;
}

void CUIObject::Free()
{
	__super::Free();
}
