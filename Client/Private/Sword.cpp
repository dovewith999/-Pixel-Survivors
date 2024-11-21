#include "stdafx.h"
#include "..\Public\Sword.h"

CSword::CSword(LPDIRECT3DDEVICE9 _GraphicDevice)
	: CWeapon(_GraphicDevice)
{
}

CSword::CSword(const CSword & rhs)
	: CWeapon(rhs)
{
}

HRESULT CSword::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSword::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	//m_pTransformCom->Turn180(D3DXVECTOR3(0.f, 1.f, 0.f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(3.f, 3.f, 7.f));
	Set_WeaponInfo();

	m_tagWeaponStats.szName = L"Sword";
	m_tagWeaponStats.iAttackDmg = 10;
	m_tagWeaponStats.eWeaponType = WEAPONSTATS::WEAPON_SWORD;

	Set_TurnAngle(D3DXVECTOR3(0.f, 1.f, 0.f), 90.f);
	Set_TurnAngle(D3DXVECTOR3(1.f, 0.f, 0.f), 30.f);

	return S_OK;
}

void CSword::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);

	if(m_pMasterCube!=nullptr)
		m_pMasterCube->Set_Positon(m_pTransformCom->Get_State(CTransform::STATE_POSITION));


	Safe_Release(pGameInstance);
}

void CSword::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);

	__super::Affected_By_Gravity(m_pTransformCom, fTimeDelta);

	State_Motion();

}


HRESULT CSword::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	//m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSword::Set_MasterCube(CMasterCube * MasterCube)
{
	if (nullptr == MasterCube)
		return E_FAIL;

	m_pMasterCube = MasterCube;

	return S_OK;
}

HRESULT CSword::Set_Turn(D3DXVECTOR3 vAxis, float fTimeDelta)
{
	m_pTransformCom->Turn(vAxis, fTimeDelta);

	return S_OK;
}

HRESULT CSword::Set_TurnAngle(D3DXVECTOR3 vAxis, float fAngle)
{
	m_pTransformCom->TurnAngle(vAxis, fAngle);

	return S_OK;
}

HRESULT CSword::Set_WeaponPos(D3DXVECTOR3 vPos, D3DXVECTOR3 vRight, D3DXVECTOR3 vUp, D3DXVECTOR3 vLook)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	D3DXVECTOR3 vPosition = vPos;
	D3DXVECTOR3 lvRight, lvUp, lvLook;

	vPosition.y += 0.2f;

	D3DXVec3Normalize(&lvRight, &vRight);
	D3DXVec3Normalize(&lvUp, &vUp);
	D3DXVec3Normalize(&lvLook, &vLook);

	m_vRight = vRight;
	m_vUp = vUp;
	m_vLook = vLook;

	vPosition += lvLook * 0.25f;
	vPosition += lvRight * 0.18f;

	vPosition += lvUp * (-0.1f /*- m_fAngle*0.001f*/);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	Safe_Release(pGameInstance);
	return S_OK;
}


HRESULT CSword::Set_WeaponInfo()
{
	m_eCollisionInfo.fAttack = 3.f;
	m_eCollisionInfo.vSize = D3DXVECTOR3(1.f, 1.f, 1.f);
	return S_OK;
}

ATTACK_COLLISION * CSword::Set_StateAttack()
{
	if (m_eState != WEAPON_IDLE)
		return nullptr;

	m_eState = WEAPON_SWING;

	m_eCollisionInfo.vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + (m_vLook * 0.5f);

	return &m_eCollisionInfo;
}

void CSword::State_Motion()
{
	switch (m_eState)
	{
	case WEAPON_SWING:
		Set_TurnAngle(m_vRight, m_fMotionSpeed);
		m_fAngle += m_fMotionSpeed;
		if (m_fAngle >= 150)
			m_eState = WEAPON_RETURN;
		break;

	case WEAPON_RETURN:
		Set_TurnAngle(m_vRight, -m_fMotionSpeed);
		m_fAngle -= m_fMotionSpeed;
		if (m_fAngle <= 0)
			m_eState = WEAPON_IDLE;
		break;

	case WEAPON_IDLE:
		break;
	}
}

HRESULT CSword::Render_Cube()
{
	if (FAILED(__super::Render_Cube()))
	{
		return E_FAIL;
	}

	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	//모습이 될 큐브 생성

	if (nullptr != m_pMasterCube)
	{
		m_pMasterCube->Set_Dead();
	}

	HANDLE		hFile = CreateFile(L"../../Data/Weapon1.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	ReadFile(hFile, &MasterCubeSize, sizeof(D3DXVECTOR3), &dwByte, nullptr);
	ReadFile(hFile, &AllCubeSize, sizeof(D3DXVECTOR3), &dwByte, nullptr);
	ReadFile(hFile, &AllVertexPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
	ReadFile(hFile, &CubeCount, sizeof(int), &dwByte, nullptr);

	cout << "큐브 숫자 : " << CubeCount << endl;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_MasterCube", L"Prototype_GameObject_MasterCube", MasterCubeSize)))
		return E_FAIL;


	m_pMasterCube = dynamic_cast<CMasterCube*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_MasterCube"));

	m_pMasterCube->Set_BindTransform(m_pTransformCom);

	for (int ti = 0; ti < CubeCount; ++ti)
	{
		ReadFile(hFile, &eCube, sizeof(CUBE), &dwByte, nullptr);
		ReadFile(hFile, &iIndex, sizeof(int), &dwByte, nullptr);

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Cube", L"Prototype_GameObject_Cube", &eCube)))
			return E_FAIL;;

		m_pCube = dynamic_cast<CCube*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Cube"));
		m_pMasterCube->Add_Cube(iIndex, eCube.vSize, m_pCube);
		m_pCube->Set_Position(m_pMasterCube->Get_Position());
	}
	m_pMasterCube->Set_CubeSize(AllCubeSize);
	m_pMasterCube->Set_VertexPos(AllVertexPos);

	//this->Set_TurnAngle(D3DXVECTOR3(0.f, 1.f, 0.f), 60.f);
	//this->Set_TurnAngle(D3DXVECTOR3(1.f, 0.f, 0.f), 60.f);

	if (m_tagWeaponStats.eWeaponType == WEAPONSTATS::WEAPON_SPEAR)
		m_pMasterCube->Set_CubeSize(D3DXVECTOR3(0.015f, 0.015f, 0.015f));
	else
	{
		m_pMasterCube->Set_CubeSize(AllCubeSize);
		m_pMasterCube->Set_CubeSize(D3DXVECTOR3(0.4f, 0.4f, 0.07f));
	}

	CloseHandle(hFile);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CSword::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Renderer `*/
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSword::SetUp_RenderState()
{

	if (nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSword::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		return E_FAIL;
	}

	return S_OK;
}

CSword * CSword::Create(LPDIRECT3DDEVICE9 _Graphic_Device)
{
	CSword* pInstance = new CSword(_Graphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Error : Failed Created Sword Prototype"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSword::Clone(void * pArg)
{
	CSword* pInstance = new CSword(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Error : Failed Cloned Sword Copy"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSword::Free()
{
	__super::Free();


	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
