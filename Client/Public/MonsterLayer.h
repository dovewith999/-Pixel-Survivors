#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CMonsterLayer final : public CBase
{
public:
	CMonsterLayer();
	virtual ~CMonsterLayer() = default;

public:
	//class CComponent* Get_ComponentPtr(const _tchar* pComponentTag, _uint iIndex);

public:
	HRESULT NativeConstruct();

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	HRESULT Delete_GameObject(_uint iObjectIndex);
	CGameObject* Find_GameObject(_uint iObjectIndex);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);

public:
	list<class CGameObject*>			m_GameObjects;
	typedef list<class CGameObject*>	GAMEOBJECTS;

public:
	static CMonsterLayer* Create();
	virtual void Free() override;
};

END