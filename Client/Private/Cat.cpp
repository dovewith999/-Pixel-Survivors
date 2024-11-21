#include "stdafx.h"
#include "..\Public\Cat.h"
#include "GameInstance.h"

#include "Player.h"


CCat::CCat(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect(pGraphic_Device)
{
}

CCat::CCat(const CCat & rhs)
	: CEffect(rhs)
{
}

HRESULT CCat::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCat::NativeConstruct(void * pArg)
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

		
	}

	return S_OK;
}

void CCat::Tick(_float fTimeDelta)
{
	m_fFrame += 10 * fTimeDelta;
	m_fAttackDelay -= fTimeDelta;
	m_fDeadTime -= fTimeDelta;
	m_fTargetSearchCool -= fTimeDelta;
	m_fEffectCool -= fTimeDelta;

	if (m_fFrame >= 2)
		m_fFrame = 0.f;

	if (m_fTargetSearchCool <= 0)
	{
		m_fTargetSearchCool = 1.f;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		m_pTarget = pGameInstance->Get_Target(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 5.f, LEVEL_GAMEPLAY, L"Layer_Enemy");

		if (nullptr == m_pTarget)
			m_pTarget = pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player");

		CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Find_Component(L"Com_Transform"));

		m_vTargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);

		if (m_pTransformCom)
			m_vTargetPos.y = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;

		Safe_Release(pGameInstance);
	}
	m_pTransformCom->LookAt(m_vTargetPos);
	
	if( D3DXVec3Length(&(m_vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION))) >= 0.3f)
		m_pTransformCom->Go_Straight(fTimeDelta);

		

	if (m_iEffect == 1 && m_fEffectCool <= 0)
	{
		m_fEffectCool = 1.f;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("Prototype_Effect_Effect0"), m_pTransformCom->Get_State(CTransform::STATE_POSITION))))
			return;

		Safe_Release(pGameInstance);
	}
	
	

	if (m_fAttackDelay <= 0)
	{
		CreateAttackCollision();
		m_fAttackDelay = m_pBattleCom->Get_BattleData().fAttack_Delay;
	}

	if (m_fDeadTime <= 0)
	{
		Set_Dead();
	}
}

void CCat::Late_Tick(_float fTimeDelta)
{
	
	//m_pTransformCom->TurnAngle(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), 90.f);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CCat::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0] * m_pTransformCom->Get_Scale().x);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0] * m_pTransformCom->Get_Scale().z);

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(_uint(m_fFrame))))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	//m_pTransformCom->TurnAngle(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), -90.f);

	return S_OK;
}

void CCat::CreateAttackCollision()
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

HRESULT CCat::SetUp_Components(ACTIVE_RECIEVE_DATA eData)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	TransformDesc.fSpeedPerSec = 6.f;

	if (eData.iLevel >= 2 && eData.iLevel < 5)
		TransformDesc.fSpeedPerSec *= 1.3f;
	if (eData.iLevel >= 5 && eData.iLevel < 8)
		TransformDesc.fSpeedPerSec *= 1.6f;
	if (eData.iLevel >= 8 && eData.iLevel < 9)
		TransformDesc.fSpeedPerSec *= 1.9f;

	TransformDesc.fSpeedPerSec *= eData.fSpeed;

	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX(L"Failed : Add_Components Com_Transform");
		return E_FAIL;
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, eData.vPos);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, eData.vRight);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, eData.vLook);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Renderer");
		return E_FAIL;
	}
	

	

	//범위증가
	if (eData.iLevel >= 2 && eData.iLevel < 6)
		m_vCollisionScale *= 1.5f;
	else if (eData.iLevel >= 6 && eData.iLevel < 9)
		m_vCollisionScale *= 2.0f;
	else if (eData.iLevel >= 9)
	{
		m_vCollisionScale *= 6.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, eData.vPos + D3DXVECTOR3(0.f,1.f,0.f));
	}

	m_vCollisionScale *= eData.fArea;

	CBattle::BATTLEDATA		BattleData;
	ZeroMemory(&BattleData, sizeof(CBattle::BATTLEDATA));

	switch(eData.iLevel)
	{
	case 1:
		BattleData.iAtk = 50;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 2:
		BattleData.iAtk = 50;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 3:
		BattleData.iAtk = 50;
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
		BattleData.iAtk = 30;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	}
	BattleData.iAtk = _uint((float)BattleData.iAtk * eData.fPower);
	BattleData.fAttack_Delay = 0.5f;

	if (eData.iLevel >= 2 && eData.iLevel < 5)
		m_fDeadTime += 1.f;
	if (eData.iLevel >= 5 && eData.iLevel < 8)
		m_fDeadTime += 1.f;
	if (eData.iLevel >= 8 && eData.iLevel < 9)
		m_fDeadTime += 1.f;
	if (eData.iLevel >= 9 )
		m_fDeadTime += 3.f;

	m_fDeadTime *= eData.fDuration;

	/* For.Com_Battle */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"), TEXT("Com_Battle"), (CComponent**)&m_pBattleCom, &BattleData)))
	{
		MSGBOX(L"Failed : Add_Components Com_Battle");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Cat"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

HRESULT CCat::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CCat::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CCat * CCat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCat*	pInstance = new CCat(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCat::Clone(void * pArg)
{
	CCat*	pInstance = new CCat(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCat::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBattleCom);
}
