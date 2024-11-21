#include "..\Public\CollisionLayer.h"
#include "GameObject.h"

CCollisionLayer::CCollisionLayer()
{
}

CComponent * CCollisionLayer::Get_ComponentPtr(const _tchar * pComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Find_Component(pComponentTag);
}

HRESULT CCollisionLayer::NativeConstruct()
{

	return S_OK;
}

HRESULT CCollisionLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

HRESULT CCollisionLayer::Delete_GameObject(_uint iObjectIndex)
{
	int ti = 0;
	for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ++iter)
	{
		if (ti == iObjectIndex)
		{
			Safe_Release(*iter);
			m_GameObjects.erase(iter++);
			return S_OK;
		}

		++ti;
	}

	return E_FAIL;
}

CGameObject * CCollisionLayer::Find_GameObject(_uint iObjectIndex)
{
	int ti = 0;
	for (auto& iter : m_GameObjects)
	{
		if (ti == iObjectIndex)
			return iter;

		++ti;
	}

	return nullptr;
}

void CCollisionLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Tick(fTimeDelta);
	}
}

void CCollisionLayer::Late_Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Tick(fTimeDelta);
	}
}

CCollisionLayer * CCollisionLayer::Create()
{
	CCollisionLayer*	pInstance = new CCollisionLayer();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX(TEXT("Failed to Created : CCollisionLayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollisionLayer::Free()
{
	//for (auto& pGameObject : m_GameObjects)
	//	Safe_Release(pGameObject);

	m_GameObjects.clear();

}

