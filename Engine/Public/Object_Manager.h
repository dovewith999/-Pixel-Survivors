#pragma once

#include "Base.h"

/* ���� ��ü���� ��Ƽ� �����Ѵ�. */
/* �纻 ��ü���� ����, ��Ƽ� �������� ���̾�� �׷����� �����Ѵ�. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_ComponentPtr(_uint iLevelID, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
public:
	HRESULT Reserve_Manager(_uint iNumLevels);
public:
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_Layer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	HRESULT Delete_LayerObject(_uint iLevelIndex, const TCHAR* pLayerTag, _int iObjectIndex = -1, void* pArg = nullptr);
	CGameObject* Find_LayerObject(_uint iLevelIndex, const TCHAR* pLayerTag, _int iObjectIndex = -1, void* pArg = nullptr); // ����
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	HRESULT Clear(_uint iLevelIndex);
	
public:
	// CH �߰�
	CGameObject* Get_Target(_float3 vPosition, _float fRange, _uint iLevelIndex, const TCHAR* pLayerTag);
	list<class CGameObject*>* Get_Layer(_uint iLevelIndex, const _tchar * pLayerTag);
	_int Get_LayerCount(_uint iLevelIndex, const _tchar * pLayerTag);

private:
	map<const _tchar*, class CGameObject*>				m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>		PROTOTYPES;

private:
	map<const _tchar*, class CLayer*>*					m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>			LAYERS;
	_uint												m_iNumLevels = 0;

public:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END