#pragma once

#include "Client_Defines.h"
#include "Item.h"

#define MONSTER_TYPE_COUNT 1

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CItem_Timer final : public CItem
{
public:
	_tchar* pMonsterTagList[MONSTER_TYPE_COUNT] = {
		L"Layer_Monster_Wave1",
	};

private:
	explicit CItem_Timer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem_Timer(const CItem_Timer& rhs);
	virtual ~CItem_Timer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Billboard();

public:
	void Active_Item();

private:
	_uint		m_iExp = 0;

private:
	list<class CGameObject*> m_MonsterList;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CItem_Timer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END