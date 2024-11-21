#pragma once
#include "GameObject.h"
 
//04/27 ¿”»Òº∑ √ﬂøÏ∞° 

BEGIN(Engine)
class ENGINE_DLL CUIObject abstract : public CGameObject 
{
public:
	explicit CUIObject(LPDIRECT3DDEVICE9 _GraphicDevice);
	explicit CUIObject(const CUIObject& rhs);
	virtual ~CUIObject() override = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};
END

