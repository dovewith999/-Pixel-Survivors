#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CBattle;
END

BEGIN(Client)
class CEffect abstract : public CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override; // 04/26 ÀÓÈñ¼· ¼ö¿ìÁ¤
	virtual HRESULT Render() override;

protected:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CBattle*				m_pBattleCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

protected:
	float m_fFrame = 0.f;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
