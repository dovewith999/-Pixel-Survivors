#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Picking.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Sound_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* 장치의 기능을 이용하기위하여 필수적으로 셋팅(초기화)되어야할 일들을 수행하낟. */
	HRESULT Initialize_Engine(HINSTANCE hInstance, _uint iNumLevels, const GRAPHIC_DEVICE_DESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut);
	void Tick_Engine(_float fTimeDelta);
	HRESULT Render_Engine();
	HRESULT Clear(_uint iLevelIndex);

public: /* For.Graphic_Device */	
	void		Render_Begin(void);
	void		Render_End(HWND hWnd = 0);

public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte byKeyID);
	_long Get_DIMMoveState(CInput_Device::DIMM eMouseMoveID);
	_byte Get_DIMButtonState(CInput_Device::DIMB eMouseButtonID);


public: /* For.Timer_Manager */
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Compute_TimeDelta(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelID, class CLevel* pNewLevel);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_Layer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	HRESULT Delete_LayerObject(_uint iLevelIndex, const TCHAR* pLayerTag, _int iObjectIndex = -1, void* pArg = nullptr);
	CGameObject* Find_LayerObject(_uint iLevelIndex, const TCHAR* pLayerTag, _int iObjectIndex = -1, void* pArg = nullptr);
	CGameObject* Get_Target(_float3 vPosition, _float fRange, _uint iLevelIndex, const TCHAR* pLayerTag); // CH 추가
	list<class CGameObject*>* Get_Layer(_uint iLevelIndex, const _tchar * pLayerTag);
	_int Get_LayerCount(_uint iLevelIndex, const _tchar * pLayerTag);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: /* For.Picking */
	bool Picking(class CVIBuffer* pVIBuffer, class CTransform* pTransform, _float3* pOut);

public: /* For.Sound_Manager */
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	CGraphic_Device*			m_pGraphic_Device = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CPicking*					m_pPicking = nullptr;
	CSound_Manager*				m_pSound_Manager = nullptr;
public:
	static void Release_Engine();
	virtual void Free() override;
};

END