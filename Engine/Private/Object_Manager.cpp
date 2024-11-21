#include "..\Public\Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"

#include "Transform.h" // CH �߰�

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}


CComponent * CObject_Manager::Get_ComponentPtr(_uint iLevelID, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (iLevelID >= m_iNumLevels)
		return nullptr;

	CLayer*		pLayer = Find_Layer(iLevelID, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_ComponentPtr(pComponentTag, iIndex);	
}

HRESULT CObject_Manager::Reserve_Manager(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevels];
	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == pPrototype || 
		nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_Layer(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pLayers || 
		iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
	{
		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CObject_Manager::Delete_LayerObject(_uint iLevelIndex, const TCHAR * pLayerTag, _int iObjectIndex, void * pArg)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		//ERRMSG("���̾ ����");
		return E_FAIL;
	}

	if (-1 == iObjectIndex)
	{
		iObjectIndex = (int)pLayer->m_GameObjects.size() - 1;
	}

	if (FAILED(pLayer->Delete_GameObject((_uint)iObjectIndex)))
	{
		//ERRMSG("������Ʈ�� ����");
		return E_FAIL;
	}

	return S_OK;
}

CGameObject * CObject_Manager::Find_LayerObject(_uint iLevelIndex, const TCHAR * pLayerTag, _int iObjectIndex, void * pArg)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		//ERRMSG("���̾ ����");
		return nullptr;
	}

	if (-1 == iObjectIndex)
	{
		iObjectIndex = (int)pLayer->m_GameObjects.size() - 1;
	}

	CGameObject* pGameObject = pLayer->Find_GameObject((_uint)iObjectIndex);
	if (nullptr == pGameObject)
	{
		//ERRMSG("������Ʈ�� ����");
		return nullptr;
	}

	return pGameObject;
}

// CH �߰�
CGameObject* CObject_Manager::Get_Target(_float3 vPosition, _float fRange, _uint iLevelIndex, const TCHAR* pLayerTag)
{
	// Range ���� ��ġ�� Ư�� ������Ʈ�� Ž��
	// ex �÷��̾� Ž�� > LEVEL_GAMEPLAY, L"Layer_Player"

	_int iObjectIndex = -1;
	float fMinDistance = 0xffffff;

	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (pLayer->Get_ObjectCount() <= 0)
		return nullptr;

	for (_uint i = 0; i < pLayer->Get_ObjectCount(); ++i)
	{
		CComponent* pTargetTransformCom = Get_ComponentPtr(iLevelIndex, pLayerTag, L"Com_Transform", i);
		_float3 vTargetPos = dynamic_cast<CTransform*>(pTargetTransformCom)->Get_State(CTransform::STATE_POSITION);

		_float3 vDirVec = vTargetPos - vPosition;

		_float fDistance = D3DXVec3Length(&vDirVec);

		if (fRange >= fDistance && fMinDistance > fDistance)
		{
			fMinDistance = fDistance;
			iObjectIndex = i;
		}
	}

	if (-1 == iObjectIndex)
		return nullptr;

	return Find_LayerObject(iLevelIndex, pLayerTag, iObjectIndex);

}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Tick(fTimeDelta);			
		}
	}	
}

void CObject_Manager::Late_Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Late_Tick(fTimeDelta);
		}
	}
}

HRESULT CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();

	return S_OK;
}

CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{
	auto		iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTagFinder(pPrototypeTag));

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;	
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto		iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

list<class CGameObject*>* CObject_Manager::Get_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto		iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return &iter->second->m_GameObjects;
}

_int CObject_Manager::Get_LayerCount(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto		iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (iter == m_pLayers[iLevelIndex].end())
		return -1;

	return iter->second->m_GameObjects.size();
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();
}
