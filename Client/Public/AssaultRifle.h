#pragma once
#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Client)
class CAssaultRifle final : public CWeapon
{
public:
	explicit CAssaultRifle(LPDIRECT3DDEVICE9 _GraphicDevice);
	explicit CAssaultRifle(const CAssaultRifle& rhs);
	virtual ~CAssaultRifle() override = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	virtual HRESULT Render_Cube() override;

//public: // 05/04 ���� �߿찡
//	_float3 Get_RayDir() { return m_vWorldRayDir; }
//	_float3 Get_RayPos() { return m_vWorldRayPos; }
//
//public:
//	void Shot_Bullet();
//
//private:
//	_float3				m_vWorldRayDir, m_vWorldRayPos;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CAssaultRifle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

