#pragma once

#include "Base.h"

BEGIN(Engine)

class CCollisionLayer final : public CBase
{
public:
	CCollisionLayer();
	virtual ~CCollisionLayer() = default;

public:
	class CComponent* Get_ComponentPtr(const _tchar* pComponentTag, _uint iIndex);

public:
	HRESULT NativeConstruct();

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	HRESULT Delete_GameObject(_uint iObjectIndex);
	CGameObject* Find_GameObject(_uint iObjectIndex);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);

public:
	_uint Get_ObjectCount() { return static_cast<_uint>(m_GameObjects.size()); }

public:
	list<class CGameObject*>			m_GameObjects;
	typedef list<class CGameObject*>	GAMEOBJECTS;

public:
	static CCollisionLayer* Create();
	virtual void Free() override;
};

END