#pragma once

#include "Base.h"

/* 인게임에 출현할 수 있는 게임오브젝트들의 부모다. */
BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	class CComponent* Find_Component(const _tchar* pComponentTag);

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	CString						m_strGameObjecttag;
	TCHAR*				m_pSoundKey = nullptr;
public:
	CString						Get_Tag();
	TCHAR*				Get_SoundKey()
	{
		return m_pSoundKey;
	}
public:
	void Set_Dead() { m_bDead = true; }
	bool Get_Dead(void) { return m_bDead; }

public:
	// 0511
	void Set_Active() { m_bActive = true; }
	bool Get_Active(void) { return m_bActive; }

protected:
	map<const _tchar*, class CComponent*>			m_Components;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

	CString m_strMonstertag;

	bool m_bDead = false;
	bool m_bActive = false; // 0511
public:
	/* 부모의 컨테이너에도 보관. 자식의 멤버변수에도 보관. */
	HRESULT Add_Components(_uint iLevelID, const _tchar * pPrototypeTag, const _tchar* pComponentTag, CComponent ** ppOut, void * pArg = nullptr);
	HRESULT Bind(_uint iLevelID, const _tchar * pPrototypeTag, const _tchar* pComponentTag, CComponent ** ppOut, void * pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END