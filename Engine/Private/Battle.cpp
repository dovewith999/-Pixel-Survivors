#include "Battle.h"

CBattle::CBattle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{
}

CBattle::CBattle(const CBattle & rhs)
	:CComponent(rhs)
{
}

HRESULT CBattle::NativeConstruct_Prototype()
{
	ZeroMemory(&m_BattleData, sizeof(BATTLEDATA));

	return S_OK;
}

HRESULT CBattle::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_BattleData, pArg, sizeof(BATTLEDATA));
	}

	return S_OK;
}

HRESULT CBattle::Get_Buff(BUFFENUM Buff, void * pArg)
{
	return S_OK;
}

_uint CBattle::Attackted(_uint Attackted_Atk, void * pArg)
{
	if (pArg != nullptr)
	{
		BATTLEDATA BattleData = *(BATTLEDATA*)pArg;
		m_BattleData.fAttacktedPower = BattleData.fAttacktedPower;
		m_BattleData.vAttacktedDir = m_BattleData.vAttackDir;
	}

	_uint FinalAtk = Attackted_Atk - m_BattleData.iDeffence;

	if (m_BattleData.iHP < FinalAtk)
		m_BattleData.iHP = 0;
	else
		m_BattleData.iHP -= FinalAtk;
	m_BattleData.bAttackted = true;

	return m_BattleData.iHP;
}

_uint CBattle::Get_FinalAtk(void)
{
	//후에는 각종 상태이상에 따른 공격력 변화 처리 후 해당 값을 리턴해준다.
	return m_BattleData.iAtk;
}

_uint CBattle::Get_FinalAttackDelay(void)
{
	//각종 상태이상 처리 후의 공격딜레이를 받을것임
	return (_uint)m_BattleData.fAttack_Delay;
}

CBattle * CBattle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBattle*	pInstance = new CBattle(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CBattle::Clone(void * pArg)
{
	CBattle*	pInstance = new CBattle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattle::Free()
{
	__super::Free();

	for (auto& iter : m_BuffList)
	{
		iter.second = nullptr;
	}
	m_BuffList.clear();

}
