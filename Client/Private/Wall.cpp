#include "stdafx.h"
#include "..\Public\Wall.h"
#include "GameInstance.h"

CWall::CWall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CWall::CWall(const CWall & rhs)
	: CGameObject(rhs)
{

}

HRESULT CWall::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CWall::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		if (nullptr == pGameInstance)
			return E_FAIL;

		Safe_AddRef(pGameInstance);

		WALL tTerrain = *(WALL*)pArg;

		m_tTerrain = tTerrain;

		m_iTileCountX = (_uint)tTerrain.vSize.x;
		m_iTileCountY = (_uint)tTerrain.vSize.y;
		m_iTileCountZ = (_uint)tTerrain.vSize.z;

		Safe_Release(pGameInstance);
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_Position(m_tTerrain.vBasePos);

	// 충돌 처리를 위한 Collider Position Set
	_float3 vLocation;

	if (ROLE_BACK == m_tTerrain.eRole)
		vLocation = { (m_tTerrain.vSize.x / 2.f), (m_tTerrain.vSize.z / 2.f), m_tTerrain.vBasePos.z };
	else if(ROLE_FRONT == m_tTerrain.eRole)
		vLocation = { (m_tTerrain.vSize.x / 2.f), (m_tTerrain.vSize.z / 2.f), 0.f };
	else if (ROLE_LEFT == m_tTerrain.eRole)
		vLocation = { 0.f, (m_tTerrain.vSize.x / 2.f), (m_tTerrain.vSize.z / 2.f) };
	else
		vLocation = { m_tTerrain.vBasePos.x, (m_tTerrain.vSize.x / 2.f), (m_tTerrain.vSize.z / 2.f) };

	m_pColliderCom->Set_Location(vLocation);
	m_pCollisionCom->Add_Collisions(0, L"Wall", this);

	return S_OK;
}

void CWall::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CWall::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_TERRAIN, this);

}

HRESULT CWall::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_tTerrain.iDrawID)))
		return E_FAIL;

	//if (FAILED(SetUp_RenderState()))
	//	return E_FAIL;
	m_pColliderCom->Render();
	m_pVIBufferCom->Render();

	//if (FAILED(Release_RenderState()))
	//	return E_FAIL;

	return S_OK;
}

void CWall::Set_Role(ROLE eRole)
{
	//m_eRole = eRole;
}

void CWall::Set_Position(const _float3& vPosition)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
}

void CWall::Rotation(const _float3& vAxis, _float fRadian)
{
	m_pTransformCom->Rotation(vAxis, fRadian);
}

HRESULT CWall::SetUp_Components()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, &m_tTerrain.vSize)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Tile"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Collider */
	_float3 vCollierSize;
	if (ROLE_BACK == m_tTerrain.eRole || ROLE_FRONT == m_tTerrain.eRole)
		m_vColliderSize = { m_tTerrain.vSize.x, m_tTerrain.vSize.z, 0.5f };
	else
		m_vColliderSize = { 0.5f, m_tTerrain.vSize.x, m_tTerrain.vSize.z };

	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &m_vColliderSize)))
	{
		MSGBOX(L"Failed : Add_Components Prototype_Component_Collider");
		return E_FAIL;
	}

	/* For.Com_Collision */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CWall::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return S_OK;
}

HRESULT CWall::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return S_OK;
}

CWall * CWall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWall*	pInstance = new CWall(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWall::Clone(void * pArg)
{
	CWall*	pInstance = new CWall(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CWall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWall::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
