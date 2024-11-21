#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "Monster.h"

BEGIN(Client)
class CTest : public CMonster
{
public :
	enum ANI_CUBE {
		CUBE_HEAD, CUBE_BODY, CUBE_LEFTLEG, CUBE_RIGHTLEG, CUBE_LEFTARM, CUBE_RIGHTARM, CUBE_LEFTELBOW,
		CUBE_RIGHTELBOW, CUBE_LEFTANKLE, CUBE_RIGHTANKLE, ANI_CUBE_END
	};
	typedef struct tagCubeBodyInfo
	{
		float fWidth;
		float fTopHeight;
		float fBottomHeight;
		float fPos;
		ANI_CUBE eCube;
		CString strFilePath;
	}CUBE_BODY_INFO;
private:
	ANIMATIONDESC m_stAniDescl[TEST_STATE_END];
public:
	void Clear_Flag(TEST_STATE eState, _float _fDeltaTime);
public:
	explicit CTest::CTest(LPDIRECT3DDEVICE9 _pDevice);

	explicit CTest::CTest(LPDIRECT3DDEVICE9 _pDevice, ST_GAMEOBJECT_FILE_INFO& tFileInfo);

	explicit CTest::CTest(const CTest& _rhs);

	virtual ~CTest() = default;
public:
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
protected:
	virtual HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct_Prototype(void* pArg);
	virtual HRESULT NativeConstruct(void* pArg) override;
private:
	HRESULT Search_Target(const _tchar* pTargetTag, CGameObject** pTarget, _float fRange);
	HRESULT SetUp_Components();
	CGameObject* m_pTarget = nullptr;
	//root
	CAniTransform* m_pBoneRootTransForm = NULL;
	//pelvis
	CAniTransform* m_pBonePelvisTransForm = NULL;
	//spine
	CAniTransform* m_pBoneSpineTransForm = NULL;
	//neck
	CAniTransform* m_pBoneNeckTransForm = NULL;
	//left leg
	CAniTransform* m_pBoneLeftLegTransForm = NULL;
	//right leg
	CAniTransform* m_pBoneRightLegTransForm = NULL;
	//left arm
	CAniTransform* m_pBoneLeftArmTransForm = NULL;
	//Right arm
	CAniTransform* m_pBoneRightArmTransForm = NULL;
	//Left Elbow
	CAniTransform* m_pBoneLeftElbowTransForm = NULL;
	//Right Elbow
	CAniTransform* m_pBoneRightElbowTransForm = NULL;
	//Left Ankle
	CAniTransform* m_pBoneLeftAnkleTransForm = NULL;
	//Right Ankle
	CAniTransform* m_pBoneRightAnkleTransForm = NULL;

	//Ani VIBuffer Member Pointer
	//head
	CVIBuffer_Ani_Cube* m_pAniHeadCube = NULL;
	//body
	CVIBuffer_Ani_Cube* m_pAniBodyCube = NULL;
	//left leg
	CVIBuffer_Ani_Cube* m_pAniLeftLegCube = NULL;
	//right leg
	CVIBuffer_Ani_Cube* m_pAniRightLegCube = NULL;
	//left arm
	CVIBuffer_Ani_Cube* m_pAniLeftArmCube = NULL;
	//Right arm
	CVIBuffer_Ani_Cube* m_pAniRightArmCube = NULL;
	//Left Elbow
	CVIBuffer_Ani_Cube* m_pAniLeftElbowCube = NULL;
	//Right Elbow
	CVIBuffer_Ani_Cube* m_pAniRightElbowCube = NULL;
	//Left Ankle
	CVIBuffer_Ani_Cube* m_pAniLeftAnkleCube = NULL;
	//Right Ankle
	CVIBuffer_Ani_Cube* m_pAniRightAnkleCube = NULL;
private :
		CCollider*				m_pColliderCom = nullptr;
		CCollision*				m_pCollisionCom = nullptr;
public:
	class CBone* Get_ParentBone(int index);
private:
	vector<class CBone*> m_pBone;
protected:
	vector<vector<CAniTransform*>> m_vecBoneTransfrom;
private:
	vector<CVIBuffer_Ani_Cube*> m_vecAniCube;
protected:
	vector<STSHAPE> m_vecAniCubeInfo;
public:
	TEST_STRUCT Get_TestStruct(TEST_STATE eTestState);
private:
	vector<TEST_STRUCT> m_vecAniInfo;
private :
private:
	tagCubeBodyInfo			m_stBodyInfo;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	ST_GAMEOBJECT_FILE_INFO m_stFileInfo;
	HRESULT Set_AniCubeInfo(STSHAPE CubeInfo, ANI_CUBE eCubeState, CString strFilePath);
private:
	D3DXMATRIX Set_Matrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vAxis, float fAngle);
public:
	static CGameObject* CTest::Create(LPDIRECT3DDEVICE9 _pDevice, ST_GAMEOBJECT_FILE_INFO& stFileInfo);

	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

