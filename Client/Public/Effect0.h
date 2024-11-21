#pragma once

#include "Client_Defines.h"
#include "Effect.h"
#include "Terrain.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEffect0 final : public CEffect
{
private:
	explicit CEffect0(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect0(const CEffect0& rhs);
	virtual ~CEffect0() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void CreateAttackCollision();

private:
	D3DXVECTOR3	  m_vCollisionScale = D3DXVECTOR3(2.f, 2.f, 2.f);


protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CEffect0* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END