#include "stdafx.h"
#include "..\Public\Item_Object.h"
#include "GameInstance.h"
#include "Player.h"

#include "Item_Timer.h"
#include "Item_HP.h"
#include "Item_Vacuum.h"
#include "Item_Treasure.h"

#include <ctime>
#include <random>
#include <functional>

CItem_Object::CItem_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CItem(pGraphic_Device)
{
}

CItem_Object::CItem_Object(const CItem_Object & rhs)
	: CItem(rhs)
{
}

HRESULT CItem_Object::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_Object::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_tItemInit = *(ITEM*)pArg;

		vPosition = m_tItemInit.vPosition;

		m_eType = (TYPE)m_tItemInit.iType;

		switch (m_eType)
		{
		case TYPE_BLUE_BRAZIER:
			m_pTextureTag = L"Prototype_Component_Texture_Item_Blue_Brazier";
			m_iFrameCnt = 3;
			break;
		case TYPE_RED_BRAZIER:
			m_pTextureTag = L"Prototype_Component_Texture_Item_Red_Brazier";
			m_iFrameCnt = 3;
			break;
		case TYPE_CANDLESTICK:
			m_pTextureTag = L"Prototype_Component_Texture_Item_CandleStick";
			m_iFrameCnt = 1;
			break;
		case TYPE_STREETLAMP:
			m_pTextureTag = L"Prototype_Component_Texture_Item_StreetLamp";
			m_iFrameCnt = 1;
			break;
		}

	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX(L"Failed : SetUp_Components");
		return E_FAIL;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	if (TYPE_STREETLAMP == m_eType)
		m_pTransformCom->Set_ScalePlus(_float3(1.3f, 1.f, 0.f));

	//wsprintf(m_szRenderTag, m_szRenderComTag, m_iDrawID);

	return S_OK;
}

void CItem_Object::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fFrame += m_iFrameCnt * fTimeDelta;

	if (m_fFrame >= m_iFrameCnt)
		m_fFrame = 0.f;

	if (m_bActive)
	{
		Create_Itme();
		Set_Dead();
	}
}

void CItem_Object::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (Get_Dead())
		return;

	__super::Late_Tick(fTimeDelta);

	m_iTerrainIndex = m_pTerrain->Get_TerrainRange(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_iTerrainIndexX, m_iTerrainIndexY);
	__super::SetUp_OnTerrain(m_pTransformCom, LEVEL_GAMEPLAY, TEXT("Layer_Terrain"));

	m_pColliderCom->Set_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pCollisionCom->Add_Collisions(m_iTerrainIndex, L"Item_Object", this);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CItem_Object::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	Set_Billboard();

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(_uint(m_fFrame))))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CItem_Object::Create_Itme()
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);

	//srand((_uint)time(NULL));

	//// rand() % (마지막 값 - 시작 값 + 1) + 시작 값
	//// 1 ~ 100 사이 난수
	//_uint iItemNum = _uint(rand() % 100 + 1);

	std::random_device rd;

	std::mt19937 gen(rd());

	uniform_int_distribution<int> distribution(1, 100);   // 생성 범위
	auto generator = bind(distribution, gen);
	_int iItemNum = generator();

	_tchar* pPrototypeTag = L"";
	_tchar* pLayereTag = L"";

	ITEM tItem = { m_tItemInit.vPosition, 0 };

	if (1 <= iItemNum && iItemNum < 25 || 75 <= iItemNum && iItemNum <= 100)
	{
		pPrototypeTag = L"Prototype_GameObject_Item_HP";
		pLayereTag = L"Layer_Item_HP";

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayereTag, pPrototypeTag, &tItem)))
			return;

		dynamic_cast<CItem_HP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, pLayereTag))->Set_Terrain(m_pTerrain);
		dynamic_cast<CItem_HP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, pLayereTag))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	}
	else if (25 <= iItemNum && iItemNum < 50)
	{
		pPrototypeTag = L"Prototype_GameObject_Item_Vacuum";
		pLayereTag = L"Layer_Item_Vacuum";

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayereTag, pPrototypeTag, &tItem)))
			return;

		dynamic_cast<CItem_Vacuum*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, pLayereTag))->Set_Terrain(m_pTerrain);
		dynamic_cast<CItem_Vacuum*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, pLayereTag))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	}
	else if (50 <= iItemNum && iItemNum < 60)
	{
		pPrototypeTag = L"Prototype_GameObject_Item_Timer";
		pLayereTag = L"Layer_Item_Timer";

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayereTag, pPrototypeTag, &tItem)))
			return;

		dynamic_cast<CItem_Timer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, pLayereTag))->Set_Terrain(m_pTerrain);
		dynamic_cast<CItem_Timer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, pLayereTag))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	}
	else
	{
		pPrototypeTag = L"Prototype_GameObject_Item_Treasure";
		pLayereTag = L"Layer_Item_Treasure";

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayereTag, pPrototypeTag, &tItem)))
			return;

		dynamic_cast<CItem_Treasure*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, pLayereTag))->Set_Terrain(m_pTerrain);
		dynamic_cast<CItem_Treasure*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, pLayereTag))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	}


	Safe_Release(pGameInstance);
}

void CItem_Object::Set_Billboard()
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

}

HRESULT CItem_Object::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX(L"Failed : Add_Components Com_Transform");
		return E_FAIL;
	}

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Renderer");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, m_pTextureTag, TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Texture");
		return E_FAIL;
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_VIBuffer");
		return E_FAIL;
	}

	/* For.Com_Collision */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}

	/* For.Com_Collider */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, _float3(1.f, 1.f, 0.5f))))
	{
		MSGBOX(L"Failed : Add_Components Prototype_Component_Collider");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CItem_Object::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CItem_Object::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CItem_Object * CItem_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Object*	pInstance = new CItem_Object(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Object::Clone(void * pArg)
{
	CItem_Object*	pInstance = new CItem_Object(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Object::Free()
{
	__super::Free();
}
