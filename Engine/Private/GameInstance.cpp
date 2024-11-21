#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pInput_Device(CInput_Device::Get_Instance())
	, m_pPicking(CPicking::Get_Instance())
	, m_pSound_Manager(CSound_Manager::Get_Instance())
{
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pSound_Manager);
}


HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInstance, _uint iNumLevels, const GRAPHIC_DEVICE_DESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut)
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pObject_Manager || 
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->InitDevice(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppOut)))
		return E_FAIL;

	/* 입력장치 초기화. */
	if (FAILED(m_pInput_Device->Initialize_Input_Device(hInstance, GraphicDesc.hWnd)))
		return E_FAIL;

	/* 사운드장치 초기화. */

	/* 각각의 매니져들의 예약. */
	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	void Initialize_Manager();


	if (FAILED(m_pPicking->Initialize(*ppOut, GraphicDesc.hWnd, GraphicDesc.iWinCX, GraphicDesc.iWinCY)))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager || 
		nullptr == m_pInput_Device)
		return;

	m_pInput_Device->Update_InputDeviceState();

	//m_pPicking->Transform_ToWorldSpace();

	m_pObject_Manager->Tick(fTimeDelta);

	m_pObject_Manager->Late_Tick(fTimeDelta);

	m_pLevel_Manager->Tick(fTimeDelta);
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Render();	
}

HRESULT CGameInstance::Clear(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pObject_Manager->Clear(iLevelIndex);

	return S_OK;
}

void CGameInstance::Render_Begin(void)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End(HWND hWnd)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_End(hWnd);
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_long CGameInstance::Get_DIMMoveState(CInput_Device::DIMM eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMMoveState(eMouseMoveID);
}

_byte CGameInstance::Get_DIMButtonState(CInput_Device::DIMB eMouseButtonID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMButtonState(eMouseButtonID);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(pTimerTag);	
}

_float CGameInstance::Compute_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_TimeDelta(pTimerTag);
	
}

HRESULT CGameInstance::Open_Level(_uint iLevelID, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;
	
	return m_pLevel_Manager->Open_Level(iLevelID, pNewLevel);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_Layer(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Layer(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Delete_LayerObject(_uint iLevelIndex, const TCHAR * pLayerTag, _int iObjectIndex, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Delete_LayerObject(iLevelIndex, pLayerTag, iObjectIndex, pArg);
}

CGameObject * CGameInstance::Find_LayerObject(_uint iLevelIndex, const TCHAR * pLayerTag, _int iObjectIndex, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_LayerObject(iLevelIndex, pLayerTag, iObjectIndex, pArg);
}

CGameObject* CGameInstance::Get_Target(_float3 vPosition, _float fRange, _uint iLevelIndex, const TCHAR* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Target(vPosition, fRange, iLevelIndex, pLayerTag);
}

list<class CGameObject*>* CGameInstance::Get_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Layer(iLevelIndex, pLayerTag);
}

_int CGameInstance::Get_LayerCount(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	return m_pObject_Manager->Get_LayerCount(iLevelIndex, pLayerTag);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

bool CGameInstance::Picking(CVIBuffer * pVIBuffer, CTransform * pTransform, _float3 * pOut)
{
	if (nullptr == m_pPicking)
		return false;

	return m_pPicking->Picking(pVIBuffer, pTransform, pOut);
}

void CGameInstance::PlaySound(TCHAR * pSoundKey, CHANNELID eID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
	{
		return;
	}

	m_pSound_Manager->PlaySoundW(pSoundKey, eID, fVolume);
}

void CGameInstance::PlayBGM(TCHAR * pSoundKey, float fVolume)
{
	if (nullptr == m_pSound_Manager)
	{
		return;
	}
	m_pSound_Manager->PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::StopSound(CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
	{
		return;
	}
	m_pSound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	if (nullptr == m_pSound_Manager)
	{
		return;
	}
	m_pSound_Manager->StopAll();
}

void CGameInstance::SetChannelVolume(CHANNELID eID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
	{
		return;
	}

	m_pSound_Manager->SetChannelVolume(eID, fVolume);
}

void CGameInstance::Release_Engine()
{	
	CGameInstance::Get_Instance()->Destroy_Instance();

	CLevel_Manager::Get_Instance()->Destroy_Instance();	

	CObject_Manager::Get_Instance()->Destroy_Instance();

	CComponent_Manager::Get_Instance()->Destroy_Instance();

	CTimer_Manager::Get_Instance()->Destroy_Instance();

	CPicking::Get_Instance()->Destroy_Instance();

	CSound_Manager::Get_Instance()->Destroy_Instance();

	CInput_Device::Get_Instance()->Destroy_Instance();

	CGraphic_Device::Get_Instance()->Destroy_Instance();	

	
}

void CGameInstance::Free()
{
	Safe_Release(m_pPicking);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);	
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}
