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
class CItem_Object final : public CItem
{
public:
	enum TYPE{ TYPE_BLUE_BRAZIER, TYPE_RED_BRAZIER, TYPE_CANDLESTICK, TYPE_STREETLAMP, TYPE_END };

private:
	explicit CItem_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem_Object(const CItem_Object& rhs);
	virtual ~CItem_Object() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Billboard();

public:
	TYPE Get_Type() { return m_eType; }

public:
	void Create_Itme();

private:
	TYPE m_eType = TYPE_BLUE_BRAZIER;
	_tchar* m_pTextureTag = L"";
	_uint m_iFrameCnt = 0;

private:
	_float3 vPosition;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CItem_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END