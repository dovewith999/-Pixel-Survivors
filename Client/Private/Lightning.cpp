#include "stdafx.h"
#include "..\Public\Lightning.h"
#include "GameInstance.h"

#include "Player.h"
#include "../Public/MyTerrain.h"


CLightning::CLightning(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect(pGraphic_Device)
{
}

CLightning::CLightning(const CLightning & rhs)
	: CEffect(rhs)
{
}

HRESULT CLightning::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLightning::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		ACTIVE_RECIEVE_DATA eData = *(ACTIVE_RECIEVE_DATA*)pArg;

		if (FAILED(SetUp_Components(eData)))
		{
			MSGBOX(L"Failed : SetUp_Components");
			return E_FAIL;
		}

		m_iEffect = eData.iEffect;

		if (m_iEffect == 1)
			m_fFrame = 2.f;
	}

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;


	return S_OK;
}

void CLightning::Tick(_float fTimeDelta)
{
	m_fFrame += fTimeDelta * m_fSpeed;
	m_fAttackDelay -= fTimeDelta;
	m_fDeadTime -= fTimeDelta;

	
	if (m_fFrame >= 2 && !m_bLightning)
	{
		m_bLightning = true; 
		m_pTransformCom->TurnAngle(D3DXVECTOR3(1.f, 0.f, 0.f), -90.f);

		ATTACK_COLLISION Attack_Collision;
		ZeroMemory(&Attack_Collision, sizeof(ATTACK_COLLISION));

		Attack_Collision.fAttack = (float)m_pBattleCom->Get_FinalAtk();
		Attack_Collision.vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		Attack_Collision.vSize = m_vCollisionScale;
		Attack_Collision.vAttackDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		Attack_Collision.eAtkClass = ATK_PLAYER;

		m_vCollisionScale.y += 14.f;
		m_pTransformCom->Set_Scale(D3DXVECTOR3(m_vCollisionScale.x, m_vCollisionScale.y, m_vCollisionScale.z));

		D3DXVECTOR3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, D3DXVECTOR3(vPos.x, vPos.y + 5.f, vPos.z));

		m_vCollisionScale.x *= 1.5f;
		m_vCollisionScale.z *= 1.5f;
	}


	if (m_fAttackDelay <= 0 && m_bLightning)
	{
		CreateAttackCollision();
		m_fAttackDelay = m_pBattleCom->Get_BattleData().fAttack_Delay;
	}
	

	if (m_fFrame >= 3 && m_bLightning)
	{
		Set_Dead();
	}
}

void CLightning::Late_Tick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CLightning::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (m_bLightning)
	{
		_float4x4		ViewMatrix;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
		D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);
	}

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

void CLightning::CreateAttackCollision()
{
	ATTACK_COLLISION Attack_Collision;
	ZeroMemory(&Attack_Collision, sizeof(ATTACK_COLLISION));

	Attack_Collision.fAttack = (float)m_pBattleCom->Get_FinalAtk();
	Attack_Collision.vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Attack_Collision.vSize = m_vCollisionScale;
	Attack_Collision.vAttackDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	Attack_Collision.eAtkClass = ATK_PLAYER;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Attack"), TEXT("Prototype_GameObject_AttackCollision"), &Attack_Collision)))
		return;

	Safe_Release(pGameInstance);
}

HRESULT CLightning::SetUp_Components(ACTIVE_RECIEVE_DATA eData)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	//스피드 증가
	TransformDesc.fSpeedPerSec = 2.f;
	if (eData.iLevel >= 2)
		m_fSpeed += 0.3f;
	if (eData.iLevel >= 6)
		m_fSpeed += 0.3f;

	TransformDesc.fSpeedPerSec *= eData.fSpeed;

	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX(L"Failed : Add_Components Com_Transform");
		return E_FAIL;
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, eData.vPos);

	m_pTransformCom->TurnAngle(D3DXVECTOR3(1.f, 0.f, 0.f), 90.f);


	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Renderer");
		return E_FAIL;
	}
	
	//범위증가
	if (eData.iLevel >= 2 && eData.iLevel < 6)
		m_vCollisionScale *= 1.5f;
	else if (eData.iLevel >= 6)
		m_vCollisionScale *= 2.f;
	m_vCollisionScale *= eData.fArea;

	////지속시간 증가
	//if (eData.iLevel >= 4)
	//	m_fDeadTime += 0.5f;
	//if (eData.iLevel >= 7)
	//	m_fDeadTime += 0.5f;
	//m_fDeadTime *= eData.fDuration;

	CBattle::BATTLEDATA		BattleData;
	ZeroMemory(&BattleData, sizeof(CBattle::BATTLEDATA));

	switch(eData.iLevel)
	{
	case 1:
		BattleData.iAtk = 5;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 2:
		BattleData.iAtk = 5;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 3:
		BattleData.iAtk = 5;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 4:
		BattleData.iAtk = 10;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 5:
		BattleData.iAtk = 10;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 6:
		BattleData.iAtk = 10;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 7:
		BattleData.iAtk = 15;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 8:
		BattleData.iAtk = 15;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 9:
		BattleData.iAtk = 15;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	}
	//공격력 증가수치 적용
	BattleData.iAtk = _uint((float)BattleData.iAtk * eData.fPower);


	//공격 간격 설정
	BattleData.fAttack_Delay = 0.1f;
	/*if (eData.iLevel >= 3)
		BattleData.fAttack_Delay = 0.06f;
	if (eData.iLevel >= 6)
		BattleData.fAttack_Delay = 0.04f;*/

	

	/* For.Com_Battle */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"), TEXT("Com_Battle"), (CComponent**)&m_pBattleCom, &BattleData)))
	{
		MSGBOX(L"Failed : Add_Components Com_Battle");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Lightning"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

	return S_OK;
}

HRESULT CLightning::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CLightning::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CLightning * CLightning::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLightning*	pInstance = new CLightning(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLightning::Clone(void * pArg)
{
	CLightning*	pInstance = new CLightning(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLightning::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBattleCom);
}
