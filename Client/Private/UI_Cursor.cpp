#include "stdafx.h"
#include "..\Public\UI_Cursor.h"
#include "GameInstance.h"
#include "Player.h"

CUI_Cursor::CUI_Cursor(LPDIRECT3DDEVICE9 _GraphicDevice)
	: CUIObject(_GraphicDevice)
{
}

CUI_Cursor::CUI_Cursor(const CUI_Cursor & rhs)
	: CUIObject(rhs)
{
}

HRESULT CUI_Cursor::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Cursor::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (nullptr != pArg)
	{
		m_vPos = *(_float2*)pArg;
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 40.0f;
	m_fSizeY = 40.0f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, (float)g_iWinCX, (float)g_iWinCY, 0.f, 100.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player"));

	Safe_Release(pGameInstance);

	return S_OK;
}

void CUI_Cursor::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (m_pPlayer->Get_LevelUp())
	{
		m_fAccTimeDelta += fTimeDelta;

		if (0.2f <= m_fAccTimeDelta)
		{
			if (4 <= m_iTextureNum)
			{
				m_iTextureNum = 0;
			}
			else
			{
				++m_iTextureNum;
			}

			CGameInstance* pGameInstance = CGameInstance::Get_Instance();

			Safe_AddRef(pGameInstance);

			if (pGameInstance->Get_DIKeyState(DIK_UP) & 0x80)
			{
				if (250.f != m_vPos.y)
				{
					m_vPos.y -= 130.f;
				}
			}

			else if (pGameInstance->Get_DIKeyState(DIK_DOWN) & 0x80)
			{

				if (510.f != m_vPos.y)
				{
					m_vPos.y += 130.f;
				}
			}

			Safe_Release(pGameInstance);

			m_fAccTimeDelta = 0.f;

			if (250.f == m_vPos.y)
			{
				m_pPlayer->Set_SelectItem(0);
			}

			else if (380.f == m_vPos.y)
			{
				m_pPlayer->Set_SelectItem(1);
			}

			else if (510.f == m_vPos.y)
			{
				m_pPlayer->Set_SelectItem(2);
			}
		}

	}
}

void CUI_Cursor::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_pPlayer->Get_LevelUp())
	{
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Cursor::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_vPos.x - g_iWinCX * 0.5f, -m_vPos.y + g_iWinCY * 0.5f, 1.f));

	_float4x4		ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);

	m_pTransformCom->Bind_OnGraphicDevice();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_iTextureNum)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Cursor::SetUp_Components()
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

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (420.f == m_vPos.x)
	{

		if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Cursor"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	else
	{
		if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Cursor_Reverse"), TEXT("Com_Texture_1"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}

	return S_OK;;
}

HRESULT CUI_Cursor::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CUI_Cursor::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CUI_Cursor * CUI_Cursor::Create(LPDIRECT3DDEVICE9 _GraphicDevice)
{
	CUI_Cursor* pInstance = new CUI_Cursor(_GraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Error : Failed Created UI_Cursor"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Cursor::Clone(void * pArg)
{
	CUI_Cursor* pInstance = new CUI_Cursor(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Error : Failed Cloned UI_Cursor"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Cursor::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
