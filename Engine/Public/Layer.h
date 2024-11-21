#pragma once

#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
public:
	CLayer();
	virtual ~CLayer() = default;

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
	
public: // CH Ãß°¡
	_uint Get_ObjectCount();

public:
	list<class CGameObject*>			m_GameObjects;
	typedef list<class CGameObject*>	GAMEOBJECTS;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END