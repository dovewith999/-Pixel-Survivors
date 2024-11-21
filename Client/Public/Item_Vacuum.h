#pragma once

#include "Client_Defines.h"
#include "Item.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CItem_Vacuum final : public CItem
{
private:
	explicit CItem_Vacuum(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem_Vacuum(const CItem_Vacuum& rhs);
	virtual ~CItem_Vacuum() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Billboard();

public:
	void Active_ItemExp();

private:
	_uint		m_iExp = 0;

private:
	list<class CGameObject*> m_ItemExpList;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CItem_Vacuum* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END