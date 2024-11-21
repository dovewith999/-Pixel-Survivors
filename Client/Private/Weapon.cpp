#include "stdafx.h"
#include "..\Public\Weapon.h"
#include "GameInstance.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
{

}

CWeapon::CWeapon(const CWeapon & rhs)
	: CLandObject(rhs)
	, m_tagWeaponStats(rhs.m_tagWeaponStats)// 04/28 임희섭 추우가
	, m_fAngle(rhs.m_fAngle)
	, m_fMotionSpeed(rhs.m_fMotionSpeed)
	, m_vRight(rhs.m_vRight)
	, m_vUp(rhs.m_vUp)
	, m_vLook(rhs.m_vLook)
	, m_eState(rhs.m_eState)
	, m_bShow(rhs.m_bShow)
	, m_eCollisionInfo(rhs.m_eCollisionInfo)
	, dwByte(rhs.dwByte)
	, MasterCubeSize(rhs.MasterCubeSize)
	, AllCubeSize(rhs.AllCubeSize)
	, AllVertexPos(rhs.AllVertexPos)
	, CubeCount(rhs.CubeCount)
	, eCube(rhs.eCube)
	, iIndex(rhs.iIndex)
{
	/*this->m_pMasterCube = rhs.m_pMasterCube;
	this->m_pCube = rhs.m_pCube;*/
}


HRESULT CWeapon::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CWeapon::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	

	return S_OK;
}

void CWeapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);




}

void CWeapon::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);


}

HRESULT CWeapon::Render()
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

HRESULT CWeapon::Set_MasterCube(CMasterCube* MasterCube)
{
	if (nullptr == MasterCube)
		return E_FAIL;

	m_pMasterCube = MasterCube;
	m_pMasterCube->Set_BindTransform(m_pTransformCom);

	return S_OK;
}

HRESULT CWeapon::Set_Turn(D3DXVECTOR3 vAxis, float fTimeDelta)
{
	m_pTransformCom->Turn(vAxis, fTimeDelta);

	return S_OK;
}

HRESULT CWeapon::Set_TurnAngle(D3DXVECTOR3 vAxis, float fAngle)
{
	m_pTransformCom->TurnAngle(vAxis, fAngle);

	return S_OK;
}

HRESULT CWeapon::Set_WeaponPos(D3DXVECTOR3 vPos,D3DXVECTOR3 vRight, D3DXVECTOR3 vUp, D3DXVECTOR3 vLook)
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

HRESULT CWeapon::Set_WeaponInfo()
{
	//switch (m_eWeapon)
	//{
	//case SWORD:

	//	break;
	//}
	m_eCollisionInfo.fAttack = 3.f;
	m_eCollisionInfo.vSize = D3DXVECTOR3(1.f, 1.f, 1.f);
	return S_OK;
}

ATTACK_COLLISION* CWeapon::Set_StateAttack()
{
	if (m_eState != WEAPON_IDLE)
		return nullptr;

	m_eState = WEAPON_SWING;

	m_eCollisionInfo.vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + (m_vLook * 0.5f);
	
	if (WEAPONSTATS::WEAPON_GUN == m_tagWeaponStats.eWeaponType)
	{

	}

	return &m_eCollisionInfo;
}

void CWeapon::State_Motion()
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

void CWeapon::Change_Weapon(const _tchar* _FilePath)
{

	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);

	//모습이 될 큐브 생성

	if (nullptr != m_pMasterCube)
	{
		m_pMasterCube->Set_Dead();
	}

	HANDLE		hFile = CreateFile(_FilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	ReadFile(hFile, &MasterCubeSize, sizeof(D3DXVECTOR3), &dwByte, nullptr);
	ReadFile(hFile, &AllCubeSize, sizeof(D3DXVECTOR3), &dwByte, nullptr);
	ReadFile(hFile, &AllVertexPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
	ReadFile(hFile, &CubeCount, sizeof(int), &dwByte, nullptr);

	cout << "큐브 숫자 : " << CubeCount << endl;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_MasterCube", L"Prototype_GameObject_MasterCube", MasterCubeSize)))
		return;


	m_pMasterCube = dynamic_cast<CMasterCube*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_MasterCube"));
	
	m_pMasterCube->Set_BindTransform(m_pTransformCom);

	for (int ti = 0; ti < CubeCount; ++ti)
	{
		ReadFile(hFile, &eCube, sizeof(CUBE), &dwByte, nullptr);
		ReadFile(hFile, &iIndex, sizeof(int), &dwByte, nullptr);

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Cube", L"Prototype_GameObject_Cube", &eCube)))
			return;

		m_pCube = dynamic_cast<CCube*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Cube"));
		m_pMasterCube->Add_Cube(iIndex, eCube.vSize, m_pCube);
		m_pCube->Set_Position(m_pMasterCube->Get_Position());
	}
	m_pMasterCube->Set_CubeSize(AllCubeSize);
	m_pMasterCube->Set_VertexPos(AllVertexPos);

	if(m_tagWeaponStats.eWeaponType == WEAPONSTATS::WEAPON_SPEAR)
		m_pMasterCube->Set_CubeSize(D3DXVECTOR3(0.015f, 0.015f, 0.015f));
	else
	{
		m_pMasterCube->Set_CubeSize(AllCubeSize);
		m_pMasterCube->Set_CubeSize(D3DXVECTOR3(0.4f, 0.4f, 0.07f));
	}

	CloseHandle(hFile);

	Safe_Release(pGameInstance);
	return;
}

HRESULT CWeapon::Render_Cube()
{
	return S_OK;
}


HRESULT CWeapon::SetUp_Components()
{


	/* For.Com_Transform */
	//CTransform::TRANSFORMDESC		TransformDesc;
	//ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	//TransformDesc.fSpeedPerSec = 5.f;
	//TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	//if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	//	return E_FAIL;
	//
	///* For.Com_VIBuffer */
	//if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	//	return E_FAIL;	

	///* For.Com_Renderer `*/
	//if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	//	return E_FAIL;



	return S_OK;
}

HRESULT CWeapon::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/


	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	

	return S_OK;
}

HRESULT CWeapon::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/

	return S_OK;
}

CWeapon * CWeapon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeapon*	pInstance = new CWeapon(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWeapon::Clone(void * pArg)
{
	CWeapon*	pInstance = new CWeapon(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CWeapon::Free()
{
	__super::Free();

	//Safe_Release(m_pMasterCube);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
