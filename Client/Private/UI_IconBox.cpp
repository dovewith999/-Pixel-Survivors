#include "stdafx.h"
#include "../Public/UI_IconBox.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_WeaponIcon_Garlic.h"
#include "UI_WeaponIcon_Bible.h"
#include "UI_WeaponIcon_LightningRing.h"
#include "UI_WeaponIcon_Plasma.h"
#include <ctime>
#include <random>
#include <functional>

CUI_IconBox::CUI_IconBox(LPDIRECT3DDEVICE9 _GraphicDevice)
	: CUIObject(_GraphicDevice)
{
}

CUI_IconBox::CUI_IconBox(const CUI_IconBox & rhs)
	: CUIObject(rhs)
{
}

HRESULT CUI_IconBox::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_IconBox::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	/*srand((unsigned)time(nullptr));
	m_iRand = rand() % 4;*/

	// 시드값을 얻기 위한 random_device 생성.
	std::random_device rd;

	// random_device 를 통해 난수 생성 엔진을 초기화 한다.
	std::mt19937 gen(rd());

	//mt19937 engine((unsigned int)time(NULL));                    // MT19937 난수 엔진
	uniform_int_distribution<int> distribution(0, 3);       // 생성 범위
	auto generator = bind(distribution, gen);
	m_iTextureNum = generator();

	
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 50.0f;
	m_fSizeY = 50.0f;

	if (nullptr != pArg)
	{
		m_vPos = *(_float2*)pArg;

		m_vPos.x += (m_fSizeX / 2);
		m_vPos.y += (m_fSizeY / 2);
	}

	
	D3DXMatrixOrthoLH(&m_ProjMatrix, (float)g_iWinCX, (float)g_iWinCY, 0.f, 100.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player"));

	Safe_Release(pGameInstance);
	
	if (250.f > m_vPos.y)
	{
		m_iPosNum = 0;
	}

	else if (250.f < m_vPos.y && 380.f > m_vPos.y)
	{
		m_iPosNum = 1;
	}

	else if (380.f < m_vPos.y && 510.f > m_vPos.y)
	{
		m_iPosNum = 2;
	}

	m_pPlayer->Set_IconBox(m_iPosNum, m_iTextureNum);


	if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
	{
		MSGBOX(L"m_pSprite Failed");
		return E_FAIL;
	}

	D3DXFONT_DESCW	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"ANY");

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pFont)))
	{
		MSGBOX(L"m_pFont Failed");
		return E_FAIL;
	}



	return S_OK;
}

void CUI_IconBox::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CUI_IconBox::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_pPlayer->Get_LevelUp())
	{
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

		if (!m_bRand)
		{
			std::random_device rd;

			// random_device 를 통해 난수 생성 엔진을 초기화 한다.
			std::mt19937 gen(rd());

			//mt19937 engine((unsigned int)time(NULL));                    // MT19937 난수 엔진
			uniform_int_distribution<int> distribution(0, 3);       // 생성 범위
			auto generator = bind(distribution, gen);
			m_iTextureNum = generator();

			if (PLASMA == m_iTextureNum)
			{
				m_szWeaponName = L"플라즈마";
				m_iWeaponLevel = m_pPlayer->Get_ItemLevel(PLASMA);
			}

			else if (BIBLE == m_iTextureNum)
			{
				m_szWeaponName = L"성서";
				m_iWeaponLevel = m_pPlayer->Get_ItemLevel(BIBLE);
			}

			else if (LIGHTNING == m_iTextureNum)
			{
				m_szWeaponName = L"번개반지";
				m_iWeaponLevel = m_pPlayer->Get_ItemLevel(LIGHTNING);
			}

			else if (GARLIC == m_iTextureNum)
			{
				m_szWeaponName = L"마늘";
				m_iWeaponLevel = m_pPlayer->Get_ItemLevel(GARLIC);
			}

			m_pPlayer->Set_IconBox(m_iPosNum, m_iTextureNum);

			if (!m_pPlayer->Get_SameItem())
			{
				m_bRand = true;
			}
		}
		
	}

	else
	{
		m_bRand = false;

		//if (!m_bRand)
		//{
		//	std::random_device rd;

		//	// random_device 를 통해 난수 생성 엔진을 초기화 한다.
		//	std::mt19937 gen(rd());

		//	//mt19937 engine((unsigned int)time(NULL));                    // MT19937 난수 엔진
		//	uniform_int_distribution<int> distribution(0, 3);       // 생성 범위
		//	auto generator = bind(distribution, gen);
		//	m_iTextureNum = generator();

		//	m_pPlayer->Set_IconBox(m_iPosNum, m_iTextureNum);

		//	if (!m_pPlayer->Get_SameItem())
		//	{
		//		m_bRand = true;
		//	}
		//}
	}
}

HRESULT CUI_IconBox::Render()
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

	TCHAR	szBuf[256]{};
	if (0 == m_iWeaponLevel)
	{
		swprintf_s(szBuf, L"\n\n\n%s\n\n신규", m_szWeaponName);
	}
	else
	{
		swprintf_s(szBuf, L"\n\n\n%s\n\n레벨 : %d", m_szWeaponName, m_iWeaponLevel);
	}

	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, m_vPos.x - (m_fSizeX / 2.f), m_vPos.y - (m_fSizeY / 2.f), 0);

	m_pSprite->SetTransform(&world);
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);//2D

	m_pFont->DrawTextW(m_pSprite,
		szBuf,
		lstrlen(szBuf),
		NULL,
		0,
		D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pSprite->End();

	return S_OK;
}

HRESULT CUI_IconBox::SetUp_Components()
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

	/* For.Com_Renderer `*/
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */

	
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_IconBox"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;;
}

HRESULT CUI_IconBox::SetUp_RenderState()
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

HRESULT CUI_IconBox::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CUI_IconBox * CUI_IconBox::Create(LPDIRECT3DDEVICE9 _GraphicDevice)
{
	CUI_IconBox* pInstance = new CUI_IconBox(_GraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Error : Failed Created UI_IconBox"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_IconBox::Clone(void * pArg)
{
	CUI_IconBox* pInstance = new CUI_IconBox(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Error : Failed Cloned UI_IconBox"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_IconBox::Free()
{
	__super::Free();
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
