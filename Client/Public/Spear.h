#pragma once
#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Client)
class CSpear final : public CWeapon
{
public:
	explicit CSpear(LPDIRECT3DDEVICE9 _GraphicDevice);
	explicit CSpear(const CSpear& rhs);
	virtual ~CSpear() override = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Render_Cube() override;
//private:
//	CTransform*				m_pTransformCom = nullptr;
//	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
//	CRenderer*				m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CSpear* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
END

