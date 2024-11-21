#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBattle final : public CComponent
{
public:
	enum BUFFENUM {
		FREEZE, FIRE, WEAKNESS, STRONG
	};

	//���� �Ǵ� ���ݹ޾����� �ʿ��� ���� 1.���ݷ� 2. ü�� 3. ���� 4. ������ ������(�̰��� ��¦ �ָ��ϴ� ������ �����̴� �÷��̾ ���⿡ �־�ߵ� �����ϼ��� ����)
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

	//������ �ʿ��Ѱ� 1.������ ���ӽð� 2. ������ ȿ���� (���������� �������̳��� Ű������ �з�)
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
	//����� ���� ����
	HRESULT Get_Buff(BUFFENUM Buff, void* pArg = nullptr);			//����/������� �������̳ʿ� �����ϴ� �Լ� pArg�� BuffData�� �����Ͱ���
	_uint Attackted(_uint Attackted_Atk, void* pArg = nullptr);		//���� ���ݷ��� ����,�����,������ ��� �� �� �׸�ŭ �ڽ��� ü���� ��� �Լ�
	_uint Get_FinalAtk(void);			//��� ����,������� ������ ���ݷ��� �޴� �Լ�
	_uint Get_FinalAttackDelay(void);	//��� ����,������� ������ ���� �� �����̸� �޴� �Լ�;

public:
	//�� ���
	_uint Get_MaxHP(void) { return m_BattleData.iMaxHP; }
	_uint Get_HP(void) { return m_BattleData.iHP; }
	bool Get_Attackted(void) { return m_BattleData.bAttackted; }
	BATTLEDATA Get_BattleData(void) { return m_BattleData; }

public:
	//�� ����
	void Set_Atk(_uint Atk) { m_BattleData.iAtk = Atk; }
	void Set_Attackted() { m_BattleData.bAttackted = false; }
	void Set_HP(_uint iHP) { m_BattleData.iHP = iHP; } // 0511
	void Process_HP(_uint iHP) { m_BattleData.iHP -= iHP; } // 0511
private:
	BATTLEDATA m_BattleData;

	//������ ���� ������ �����ι��� ������ �������ְ� �ش� ������ ������ġ�� �ֱ� �����̴�.
	//�ߺ��� ������ �Ұ��ϴٴ� ����. ���� �����Ѱ��� �ش� ������ ���ӽð��̳� ������ġ�� ���ϴ� ���°� �ɰ��̴�.
	map<int, BUFFDATA*> m_BuffList;

public:
	static CBattle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

};

END