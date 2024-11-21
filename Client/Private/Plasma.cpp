#include "stdafx.h"
#include "..\Public\Plasma.h"
#include "GameInstance.h"

#include "Player.h"
#include "../Public/MyTerrain.h"


CPlasma::CPlasma(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect(pGraphic_Device)
{
}

CPlasma::CPlasma(const CPlasma & rhs)
	: CEffect(rhs)
{
}

HRESULT CPlasma::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlasma::NativeConstruct(void * pArg)
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

	return S_OK;
}

void CPlasma::Tick(_float fTimeDelta)
{
	m_fFrame += 4 * fTimeDelta;
	m_fAttackDelay -= fTimeDelta;
	m_fDeadTime -= fTimeDelta;

	if (m_iEffect == 0)
	{
		if (m_fFrame >= 2)
			m_fFrame = 0.f;
	}
	else
	{
		if (m_fFrame >= 4)
			m_fFrame = 2.f;
	}

	m_pTransformCom->Go_Straight(fTimeDelta);

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

void CPlasma::Late_Tick(_float fTimeDelta)
{

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CPlasma::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

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

void CPlasma::CreateAttackCollision()
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

HRESULT CPlasma::SetUp_Components(ACTIVE_RECIEVE_DATA eData)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 8.f * eData.fSpeed;
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
	if (eData.iLevel >= 2 && eData.iLevel < 5)
		m_vCollisionScale *= 1.5f;
	else if (eData.iLevel >= 5 && eData.iLevel < 7)
		m_vCollisionScale *= 2.f;
	else if (eData.iLevel >= 7 && eData.iLevel < 9)
		m_vCollisionScale *= 3.f;
	else if (eData.iLevel >= 9)
		m_vCollisionScale *= 4.f;
	m_vCollisionScale *= eData.fArea;

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
		BattleData.iAtk = 20;
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
		BattleData.iAtk = 30;
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
	BattleData.iAtk = _uint((float)BattleData.iAtk * eData.fPower);
	BattleData.fAttack_Delay = 0.08f;

	m_fDeadTime *= eData.fDuration;

	/* For.Com_Battle */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"), TEXT("Com_Battle"), (CComponent**)&m_pBattleCom, &BattleData)))
	{
		MSGBOX(L"Failed : Add_Components Com_Battle");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Plasma"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

HRESULT CPlasma::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPlasma::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CPlasma * CPlasma::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlasma*	pInstance = new CPlasma(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlasma::Clone(void * pArg)
{
	CPlasma*	pInstance = new CPlasma(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlasma::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBattleCom);
}
