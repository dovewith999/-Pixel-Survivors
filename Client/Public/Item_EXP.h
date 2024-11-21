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
class CItem_EXP final : public CItem
{
public:
	enum TYPE{ TYPE_BLUE, TYPE_GREEN, TYPE_RED };

private:
	explicit CItem_EXP(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem_EXP(const CItem_EXP& rhs);
	virtual ~CItem_EXP() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Billboard();

public:
	void Set_RedExp(_uint iExp) { m_iExp = iExp; }
	TYPE Get_Type() { return m_eType; }
	void Set_Chase() { m_bChase = true; }

private:
	_float		m_fTime = 0.f;
	bool		m_bChase = false;

private:
	TYPE		m_eType = TYPE_BLUE;
	_uint		m_iExp = 0;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CItem_EXP* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END