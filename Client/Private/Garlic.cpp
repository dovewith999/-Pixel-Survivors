#include "stdafx.h"
#include "..\Public\Garlic.h"
#include "GameInstance.h"

#include "Player.h"
#include "../Public/MyTerrain.h"


CGarlic::CGarlic(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect(pGraphic_Device)
{
}

CGarlic::CGarlic(const CGarlic & rhs)
	: CEffect(rhs)
{
}

HRESULT CGarlic::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGarlic::NativeConstruct(void * pArg)
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

	Safe_AddRef(pGameInstance);

	m_pTarget = pGameInstance->Get_Target(D3DXVECTOR3(1.f, 1.f, 1.f), 200.f, LEVEL_GAMEPLAY, L"Layer_Player");

	Safe_Release(pGameInstance);

	return S_OK;
}

void CGarlic::Tick(_float fTimeDelta)
{
	//m_fFrame += 4 * fTimeDelta;
	m_fAttackDelay -= fTimeDelta;
	m_fDeadTime -= fTimeDelta;

	/*if (m_iEffect == 0)
	{
		if (m_fFrame >= 2)
			m_fFrame = 0.f;
	}
	else
	{
		if (m_fFrame >= 4)
			m_fFrame = 2.f;
	}*/

	if (m_bGlow)
	{
		m_fGlow += 0.9f * fTimeDelta;
		if (1.f <= m_fGlow)
			m_bGlow = false;
	}
	else
	{
		m_fGlow -= 0.9f * fTimeDelta;
		if (0.5f >= m_fGlow)
			m_bGlow = true;
	}

	CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Find_Component(L"Com_Transform"));

	D3DXVECTOR3 TargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, TargetPos);




	if (m_fAttackDelay <= 0)
	{
		CreateAttackCollision();
		m_fAttackDelay = m_pBattleCom->Get_BattleData().fAttack_Delay;
	}

	/*if (m_fDeadTime <= 0)
	{
		Set_Dead();
	}*/
}

void CGarlic::Late_Tick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CGarlic::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

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

	_float vRGB = 255.f;

	m_pShaderCom->SetUp_RawValue("g_fColorR", &vRGB, sizeof(_float));
	m_pShaderCom->SetUp_RawValue("g_fColorG", &vRGB, sizeof(_float));
	m_pShaderCom->SetUp_RawValue("g_fColorB", &vRGB, sizeof(_float));
	m_pShaderCom->SetUp_RawValue("g_fAlpha", &m_fGlow, sizeof(_float));

	m_pShaderCom->Begin_Shader(2);

	m_pVIBufferCom->Render();

	m_pShaderCom->End_Shader();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CGarlic::CreateAttackCollision()
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

HRESULT CGarlic::SetUp_Components(ACTIVE_RECIEVE_DATA eData)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	//스피드 증가
	TransformDesc.fSpeedPerSec = 2.f;
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
	if (eData.iLevel >= 2 && eData.iLevel < 4)
		m_vCollisionScale *= 1.4f;
	if (eData.iLevel >= 4 && eData.iLevel < 6)
		m_vCollisionScale *= 1.6f;
	if (eData.iLevel >= 6 && eData.iLevel < 8)
		m_vCollisionScale *= 1.8f;
	if (eData.iLevel >= 8 && eData.iLevel < 9)
		m_vCollisionScale *= 2.f;
	else if (eData.iLevel >= 9)
		m_vCollisionScale *= 2.f;
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
		BattleData.iAtk = 5;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 2:
		BattleData.iAtk = 7;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 3:
		BattleData.iAtk = 8;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 4:
		BattleData.iAtk = 9;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 5:
		BattleData.iAtk = 11;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 6:
		BattleData.iAtk = 12;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 7:
		BattleData.iAtk = 13;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 8:
		BattleData.iAtk = 15;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	case 9:
		BattleData.iAtk = 18;
		m_pTransformCom->Set_Scale(m_vCollisionScale);
		break;
	}
	//공격력 증가수치 적용
	BattleData.iAtk = _uint((float)BattleData.iAtk * eData.fPower);


	//공격 간격 설정
	BattleData.fAttack_Delay = 0.5f;
	if (eData.iLevel >= 3)
		BattleData.fAttack_Delay = 0.4f;
	if (eData.iLevel >= 5)
		BattleData.fAttack_Delay = 0.3f;
	if (eData.iLevel >= 7)
		BattleData.fAttack_Delay = 0.2f;

	m_vCollisionScale.y = 1.f;

	/* For.Com_Battle */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"), TEXT("Com_Battle"), (CComponent**)&m_pBattleCom, &BattleData)))
	{
		MSGBOX(L"Failed : Add_Components Com_Battle");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Garlic"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGarlic::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CGarlic::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CGarlic * CGarlic::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGarlic*	pInstance = new CGarlic(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGarlic::Clone(void * pArg)
{
	CGarlic*	pInstance = new CGarlic(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGarlic::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pBattleCom);
	Safe_Release(m_pShaderCom);
}
