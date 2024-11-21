#pragma once

#include "Component.h"

/* 1. ȭ�鿡 �׷����� ��ü���� ��Ƽ� �����Ѵ�.(������ �������.) */
/* 2. ������ ������� ��ü���� �����Լ��� ȣ�����ش�. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP {
		RENDER_PRIORITY,
		RENDER_TERRAIN,
		RENDER_NONALPHABLEND,
		RENDER_ALPHABLEND,
		RENDER_UI,
		RENDER_END
	};
private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;

public:
	HRESULT Add_RenderList(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Draw_Renderer();

private:
	list<class CGameObject*>			m_RenderList[RENDER_END];

private:
	HRESULT Render_Priority();
	HRESULT Render_Terrain();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_AlphaBlend();
	HRESULT Render_UI();
private:
	class ZFrustum_Manager* m_pZFrustum_Manager = nullptr;


public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

};

END