#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "AniTransform_hhlee.h"
#include "ZFrustum.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pZFrustum_Manager(ZFrustum_Manager::Get_Instance())
{
	Safe_AddRef(m_pZFrustum_Manager);
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderList(RENDERGROUP eRenderGroup, CGameObject * pGameObject)
{
	if (eRenderGroup >= CRenderer::RENDER_END)
		return E_FAIL;

	m_RenderList[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_Renderer()
{

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_Terrain()))
		return E_FAIL;

	if (FAILED(Render_NonAlphaBlend()))
		return E_FAIL;

	if (FAILED(Render_AlphaBlend()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (ZFrustum_Manager::COMPLETE_GAPHIC_DEVICE == m_pZFrustum_Manager->Get_ZFrustumSate())
		m_pZFrustum_Manager->Render();

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	CTransform* pTransform = nullptr;
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		pGameObject->Render();
		Safe_Release(pGameObject);
	}

	m_RenderList[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Terrain()
{
	CTransform* pTransform = nullptr;

	for (auto& pGameObject : m_RenderList[RENDER_TERRAIN])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
		}

		Safe_Release(pGameObject);
	}

	m_RenderList[RENDER_TERRAIN].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonAlphaBlend()
{
	CTransform* pTransform = nullptr;
	CAniTransform* pAniTransform = nullptr;
	for (auto& pGameObject : m_RenderList[RENDER_NONALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();

			//if (ZFrustum_Manager::WAIT_INIT == m_pZFrustum_Manager->Get_ZFrustumSate())
			//	pGameObject->Render();
			//else
			//{
			//	if (nullptr != (pTransform = (CTransform*)pGameObject->Find_Component(TEXT("Com_Transform"))))
			//	{
			//		if (m_pZFrustum_Manager->IsIn(&pTransform->Get_State(CTransform::STATE_POSITION)))
			//			pGameObject->Render();
			//		else
			//		{
			//		}
			//	}
			//	else if (nullptr != (pAniTransform = (CAniTransform*)pGameObject->Find_Component(TEXT("Com_AniTransform"))))
			//	{
			//		if (m_pZFrustum_Manager->IsIn(&pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION)))
			//			pGameObject->Render();
			//		else
			//		{
			//		}
			//	}
			//}
		}

		Safe_Release(pGameObject);
	}

	m_RenderList[RENDER_NONALPHABLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_AlphaBlend()
{
	CTransform* pTransform = nullptr;
	CAniTransform* pAniTransform = nullptr;
	for (auto& pGameObject : m_RenderList[RENDER_ALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			//if (ZFrustum_Manager::WAIT_INIT == m_pZFrustum_Manager->Get_ZFrustumSate())
			//	pGameObject->Render();
			//else
			//{
			//	if (nullptr != (pTransform = (CTransform*)pGameObject->Find_Component(TEXT("Com_Transform"))))
			//	{
			//		if (m_pZFrustum_Manager->IsIn(&pTransform->Get_State(CTransform::STATE_POSITION)))
			//			pGameObject->Render();
			//	}
			//	else if (nullptr != (pAniTransform = (CAniTransform*)pGameObject->Find_Component(TEXT("Com_AniTransform"))))
			//	{
			//		if (m_pZFrustum_Manager->IsIn(&pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION)))
			//			pGameObject->Render();
			//	}
	
			//}
		}

		Safe_Release(pGameObject);
	}

	m_RenderList[RENDER_ALPHABLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	CTransform* pTransform = nullptr;

	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			//if (ZFrustum_Manager::WAIT_INIT == m_pZFrustum_Manager->Get_ZFrustumSate())
			//	pGameObject->Render();
			//else
			//{
			//	if (nullptr != (pTransform = (CTransform*)pGameObject->Find_Component(TEXT("Com_Transform"))))
			//		if (m_pZFrustum_Manager->IsIn(&pTransform->Get_State(CTransform::STATE_POSITION)))
			//			pGameObject->Render();
			//}
		}

		Safe_Release(pGameObject);
	}

	m_RenderList[RENDER_UI].clear();

	return S_OK;
}


CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*	pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CLayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pGameObject : m_RenderList[i])
			Safe_Release(pGameObject);
		m_RenderList[i].clear();
	}

	Safe_Release(m_pZFrustum_Manager);
}
