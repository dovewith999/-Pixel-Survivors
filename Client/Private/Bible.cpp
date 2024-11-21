#include "stdafx.h"
#include "..\Public\Bible.h"
#include "GameInstance.h"

#include "Player.h"
#include "../Public/MyTerrain.h"


CBible::CBible(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect(pGraphic_Device)
{
}

CBible::CBible(const CBible & rhs)
	: CEffect(rhs)
{
}

HRESULT CBible::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBible::NativeConstruct(void * pArg)
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

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	m_pTarget = pGameInstance->Get_Target(D3DXVECTOR3(1.f, 1.f, 1.f), 200.f, LEVEL_GAMEPLAY, L"Layer_Player");

	Safe_Release(pGameInstance);

	return S_OK;
}

void CBible::Tick(_float fTimeDelta)
{
	m_fAttackDelay -= fTimeDelta;
	m_fDeadTime -= fTimeDelta;



	CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Find_Component(L"Com_Transform"));

	D3DXVECTOR3 TargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);

	Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), m_vLook , 3.f * m_fSpeed);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, TargetPos + (m_vLook*5.f));




	if (m_fAttackDelay <= 0)
	{
		CreateAttackCollision();
		m_fAttackDelay = m_pBattleCom->Get_BattleData().fAttack_Delay;
	}

	if (m_fDeadTime <= 0 && m_iEffect == 0)
	{
		Set_Dead();
	}
}

void CBible::Late_Tick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CBible::Render()
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

void CBible::CreateAttackCollision()
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

void CBible::Get_TurnVector(D3DXVECTOR3 vAxis, _float3 &vLook, float fAngle)
{
	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, D3DXToRadian(fAngle));

	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);
}

HRESULT CBible::SetUp_Components(ACTIVE_RECIEVE_DATA eData)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	//스피드 증가
	TransformDesc.fSpeedPerSec = 2.f;
	if (eData.iLevel >= 3)
		m_fSpeed += 0.5f;
	if (eData.iLevel >= 6)
		m_fSpeed += 0.5f;

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
	m_vLook = eData.vLook;


	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Renderer");
		return E_FAIL;
	}
	
	//범위증가
	if (eData.iLevel >= 3 && eData.iLevel < 6)
		m_vCollisionScale *= 1.25f;
	else if (eData.iLevel >= 6 && eData.iLevel < 9)
		m_vCollisionScale *= 1.5f;
	else if (eData.iLevel >= 9)
		m_vCollisionScale *= 1.2f;
	m_vCollisionScale *= eData.fArea;

	//지속시간 증가
	if (eData.iLevel >= 4)
		m_fDeadTime += 0.5f;
	if (eData.iLevel >= 7)
		m_fDeadTime += 0.5f;
	m_fDeadTime *= eData.fDuration;

	CBattle::BATTLEDATA		BattleData;
	ZeroMemory(&BattleData, sizeof(CBattle::BATTLEDATA));

	switch(eData.iLevel)
	{
	case 1:
		BattleData.iAtk = 10;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 2:
		BattleData.iAtk = 10;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 3:
		BattleData.iAtk = 10;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 4:
		BattleData.iAtk = 20;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 5:
		BattleData.iAtk = 20;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 6:
		BattleData.iAtk = 20;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 7:
		BattleData.iAtk = 30;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 8:
		BattleData.iAtk = 30;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 9:
		BattleData.iAtk = 40;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	}
	//공격력 증가수치 적용
	BattleData.iAtk = _uint((float)BattleData.iAtk * eData.fPower);


	//공격 간격 설정
	BattleData.fAttack_Delay = 0.08f;
	if (eData.iLevel >= 3)
		BattleData.fAttack_Delay = 0.06f;
	if (eData.iLevel >= 6)
		BattleData.fAttack_Delay = 0.04f;

	

	/* For.Com_Battle */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"), TEXT("Com_Battle"), (CComponent**)&m_pBattleCom, &BattleData)))
	{
		MSGBOX(L"Failed : Add_Components Com_Battle");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bible"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

HRESULT CBible::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CBible::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CBible * CBible::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBible*	pInstance = new CBible(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBible::Clone(void * pArg)
{
	CBible*	pInstance = new CBible(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBible::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBattleCom);
}
