#pragma once

#include "Base.h"

/* Ŭ���̾�Ʈ���� ������ ���� Ŭ�������� �θ� �Ǵ� Ŭ������. */

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	explicit CLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;

public:
	virtual void Free() override;
};

END