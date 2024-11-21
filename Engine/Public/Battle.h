#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBattle final : public CComponent
{
public:
	enum BUFFENUM {
		FREEZE, FIRE, WEAKNESS, STRONG
	};

	//공격 또는 공격받았을때 필요한 정보 1.공격력 2. 체력 3. 방어력 4. 공격의 딜레이(이것은 살짝 애매하다 공격의 딜레이는 플레이어나 무기에 있어야될 정보일수도 있음)
	struct BATTLEDATA {
		_uint iAtk;
		_uint iHP;
		_uint iMaxHP;
		_uint iDeffence;
		_float3 vAttackDir;
		_float3 vAttacktedDir;
		float fAttacktedPower;
		float fAttack_Delay;
		bool bAttackted = false;
	};

	//버프에 필요한것 1.버프의 지속시간 2. 버프의 효과량 (버프종류는 맵컨테이너의 키값으롷 분류)
	struct BUFFDATA {
		int		BuffPower;
		float	BuffDuration;
	};

protected:
	explicit CBattle(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBattle(const CBattle& rhs);
	virtual ~CBattle() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	//값들로 얻은 정보
	HRESULT Get_Buff(BUFFENUM Buff, void* pArg = nullptr);			//버프/디버프를 맵컨테이너에 저장하는 함수 pArg는 BuffData를 받을것같음
	_uint Attackted(_uint Attackted_Atk, void* pArg = nullptr);		//받은 공격력을 버프,디버프,방어력을 계산 한 후 그만큼 자신의 체력을 깍는 함수
	_uint Get_FinalAtk(void);			//모든 버프,디버프를 적용한 공격력을 받는 함수
	_uint Get_FinalAttackDelay(void);	//모든 버프,디버프를 적용한 공격 후 딜레이를 받는 함수;

public:
	//값 얻기
	_uint Get_MaxHP(void) { return m_BattleData.iMaxHP; }
	_uint Get_HP(void) { return m_BattleData.iHP; }
	bool Get_Attackted(void) { return m_BattleData.bAttackted; }
	BATTLEDATA Get_BattleData(void) { return m_BattleData; }

public:
	//값 수정
	void Set_Atk(_uint Atk) { m_BattleData.iAtk = Atk; }
	void Set_Attackted() { m_BattleData.bAttackted = false; }
	void Set_HP(_uint iHP) { m_BattleData.iHP = iHP; } // 0511
	void Process_HP(_uint iHP) { m_BattleData.iHP -= iHP; } // 0511
private:
	BATTLEDATA m_BattleData;

	//맵으로 만든 이유는 버프로받은 버프의 종류가있고 해당 버프의 버프수치가 있기 떄문이다.
	//중복된 버프는 불가하다는 가정. 만약 가능한경우는 해당 버프의 지속시간이나 증감수치를 더하는 형태가 될것이다.
	map<int, BUFFDATA*> m_BuffList;

public:
	static CBattle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

};

END