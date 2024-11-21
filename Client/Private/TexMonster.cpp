#include "stdafx.h"
#include "..\Public\TexMonster.h"
#include "GameInstance.h"

#include "Player.h"
#include "../Public/MyTerrain.h"

CTexMonster::CTexMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster(pGraphic_Device)
{

}

CTexMonster::CTexMonster(const CTexMonster & rhs)
	: CMonster(rhs)
{

}

HRESULT CTexMonster::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CTexMonster::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		TEXMONDATA eData = *(TEXMONDATA*)pArg;
		m_eTexKind = eData.eTex;

		if (FAILED(SetUp_Components()))
		{
			MSGBOX(L"Failed : SetUp_Components");
			return E_FAIL;
		}
		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, eData.vPos);
		//m_pTransformCom->Set_Scale(1.5f);
	}

	
	//플레이어 HPBar 관련 내용 추가!!

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	_float2 HP_Pos(0.f, 0.6f);
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_UI_EnemyHPBar", TEXT("Prototype_UIObject_EnemyHPBar"), &HP_Pos.y)))
		return E_FAIL;

	m_pHPBar = dynamic_cast<CUI_Enemy_HPBar*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_UI_EnemyHPBar"));
	Set_HPBar(m_pHPBar);
	Set_BindingTransform_HPBar();

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_UI_EnemyHPBar", TEXT("Prototype_UIObject_EnemyHPElement"))))
		return E_FAIL;

	m_pHPBar->Add_Element(dynamic_cast<CUI_Enemy_HPElement*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_UI_EnemyHPBar")));

	Safe_Release(pGameInstance);

	return S_OK;
}

void CTexMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_eState != STATE_DIE)
	{
		m_fFrame += 2 * fTimeDelta;

		if (m_fFrame >= 2)
			m_fFrame = 0.f;
	}
	if (m_eState == STATE_DIE)
	{
		m_fFrame += 20 * fTimeDelta;

		if (m_fFrame >= 10)
		{
			m_fFrame = 9.f;
		m_pBattleCom->Set_Attackted();

		if (m_pHPBar != nullptr)
			m_pHPBar->Set_HPBar_UI((float)m_pBattleCom->Get_HP() / (float)m_pBattleCom->Get_MaxHP());
		if (m_pBattleCom->Get_HP() <= 0)
			Set_Dead();
		}
	}

	
	if (m_eState == STATE_DIE)
		return;

	if (m_eAtkMotion == ATKMOTION_1 && m_iAttackDelay <= 0 )
	{
		m_iAttackDelay = m_pBattleCom->Get_FinalAttackDelay();

		ATTACK_COLLISION Attack_Collision;
		ZeroMemory(&Attack_Collision, sizeof(ATTACK_COLLISION));

		Attack_Collision.fAttack = (float)m_pBattleCom->Get_FinalAtk();

		Attack_Collision.vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		Attack_Collision.vSize = D3DXVECTOR3(1.f, 1.f, 1.f);

		Attack_Collision.vAttackDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		Attack_Collision.fAttacktedPower = 3.f;
		Attack_Collision.eAtkClass = ATK_ENEMY;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Attack"), TEXT("Prototype_GameObject_AttackCollision"), &Attack_Collision)))
			return;

		Safe_Release(pGameInstance);
	}

}

void CTexMonster::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);


	m_iTerrainIndex = m_pTerrain->Get_TerrainRange(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_iTerrainIndexX, m_iTerrainIndexY);
	__super::SetUp_OnTerrain(m_pTransformCom, LEVEL_GAMEPLAY, TEXT("Layer_Terrain"));

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);

	if (m_eState != STATE_DIE)
	{
		m_pColliderCom->Set_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		m_pCollisionCom->Add_Collisions(m_iTerrainIndex, L"Monster", this);
	}
	/*if (m_iTerrainIndexX != -1)
		m_pCollisionCom->Add_Collisions(m_iTerrainIndexX, L"Monster", this);
	if (m_iTerrainIndexY != -1)
		m_pCollisionCom->Add_Collisions(m_iTerrainIndexY, L"Monster", this);*/


	if (m_eState == STATE_DIE)
		return;

	__super::Affected_By_Gravity(m_pTransformCom, fTimeDelta);
}

HRESULT CTexMonster::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	//if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	//	return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(_uint(m_fFrame))))
	//	return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	_float4x4	WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix);

	_float4x4	ViewMat;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMat);

	_float4x4	ProjMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom->SetUp_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	m_pShaderCom->SetUp_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&ViewMat, &ViewMat), sizeof(_float4x4));
	m_pShaderCom->SetUp_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_float4x4));

	m_pShaderCom->SetUp_Texture("g_Texture", m_pTextureCom->Get_Texture(m_fFrame));

	_uint iPassIndex = 0;

	if (m_bAttacked)
		iPassIndex = 1;
	else if (m_bFreeze)
		iPassIndex = 3;

	m_pShaderCom->Begin_Shader(iPassIndex);

	m_pVIBufferCom->Render();

	m_pShaderCom->End_Shader();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTexMonster::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Collision */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}

	/* For.Com_Collider */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, _float3(0.7f, 0.7f, 0.7f))))
	{
		MSGBOX(L"Failed : Add_Components Prototype_Component_Collider");
		return E_FAIL;
	}

	CBattle::BATTLEDATA		BattleData;
	ZeroMemory(&BattleData, sizeof(CBattle::BATTLEDATA));

	switch (m_eTexKind)
	{
	case MONSTER_BAT:
		m_eLevel = LEVEL_1;
		BattleData.iAtk = 1;
		BattleData.iDeffence = 0;
		BattleData.iMaxHP = 2;
		BattleData.iHP = 2;
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bat"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case MONSTER_SKELETON:
		m_eLevel = LEVEL_1;
		BattleData.iAtk = 2;
		BattleData.iDeffence = 0;
		BattleData.iMaxHP = 50;
		BattleData.iHP = 50;
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skeleton"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case MONSTER_LIZARDMAN:
		m_eLevel = LEVEL_2;
		BattleData.iAtk = 3;
		BattleData.iDeffence = 0;
		BattleData.iMaxHP = 90;
		BattleData.iHP = 90;
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_LizardMan"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case MONSTER_WRAITH:
		m_eLevel = LEVEL_2;
		BattleData.iAtk = 4;
		BattleData.iDeffence = 0;
		BattleData.iMaxHP = 150;
		BattleData.iHP = 150;
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Wraith"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case MONSTER_DRAGO:
		m_eLevel = LEVEL_3;
		BattleData.iAtk = 5;
		BattleData.iDeffence = 0;
		BattleData.iMaxHP = 300;
		BattleData.iHP = 300;
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Drago"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case MONSTER_UNDEAD:
		m_eLevel = LEVEL_3;
		BattleData.iAtk = 6;
		BattleData.iDeffence = 0;
		BattleData.iMaxHP = 500;
		BattleData.iHP = 500;
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Undead"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	}
	
	BattleData.fAttack_Delay = 30;

	/* For.Com_Battle */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"), TEXT("Com_Battle"), (CComponent**)&m_pBattleCom, &BattleData)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTexMonster::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CTexMonster::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CTexMonster * CTexMonster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTexMonster*	pInstance = new CTexMonster(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTexMonster::Clone(void * pArg)
{
	CTexMonster*	pInstance = new CTexMonster(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexMonster::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
