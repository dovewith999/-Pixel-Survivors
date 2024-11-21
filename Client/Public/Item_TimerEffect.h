#pragma once

#include "Client_Defines.h"
#include "Item.h"

#define MONSTER_TYPE_COUNT 1

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CItem_TimerEffect final : public CItem
{
private:
	explicit CItem_TimerEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem_TimerEffect(const CItem_TimerEffect& rhs);
	virtual ~CItem_TimerEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Billboard();

private:
	bool	m_bGlow = false;
	_float	m_fGlow = 0.f;

private:
	_float m_fDeadTime = 3.f;

private:
	CShader*		m_pShaderCom = nullptr;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CItem_TimerEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END