#include "stdafx.h"
#include "Test.h"
#include "GameInstance.h"
#include "Player.h"
#include "Bone_hhlee.h"
//#include "VIBuffer_Cube_hhlee.h"
//#include "AniTransform_hhlee."
//#include "Renderer.h"

CTest::CTest(LPDIRECT3DDEVICE9 _pDevice)
	:CMonster(_pDevice)
{
	m_strGameObjecttag = L"Animation";
	m_iHp = 1;
}

CTest::CTest(LPDIRECT3DDEVICE9 _pDevice, ST_GAMEOBJECT_FILE_INFO& tFileInfo)
	: CMonster(_pDevice)
{
	m_stFileInfo = tFileInfo;
	m_strGameObjecttag = L"Animation";
	m_iHp = 1;
}

CTest::CTest(const CTest& _rhs)
	: CMonster(_rhs)
{
	m_strGameObjecttag = _rhs.m_strGameObjecttag;

	CTest* Parent = (CTest*)&_rhs;

	memcpy(m_stAniDescl, _rhs.m_stAniDescl, sizeof(ANIMATIONDESC) * TEST_STATE_END);

	m_vecAniInfo = _rhs.m_vecAniInfo;
	m_vecAniCubeInfo = _rhs.m_vecAniCubeInfo;

	CGameInstance* pGameInstace = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstace);
	
	ANIMATIONDESC stTransDesc;
	ZeroMemory(&stTransDesc, sizeof(ANIMATIONDESC));

	for (int index = 0; index < TEST_STATE_END; ++index)
	{
		m_pBone.push_back((CBone*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY, KEY_COMPONENT_BONE, NULL));
		m_vecBoneTransfrom.push_back(vector<CAniTransform*>());

		CAniTransform* pRootTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_ROOT);
		CAniTransform* pPelvisTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS);
		CAniTransform* pSpineTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_SPINE);
		CAniTransform* pNeckTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_NECK);
		CAniTransform* pLeftLegTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTLEG);
		CAniTransform* pRightLegTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTLEG);
		CAniTransform* pLeftArmTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTARM);
		CAniTransform* pRightArmTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTARM);
		CAniTransform* pLeftElbowTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTELBOW);
		CAniTransform* pRightElbowTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTELBOW);
		CAniTransform* pLeftAnkleTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTANKLE);
		CAniTransform* pRightAnkleTransform = (CAniTransform*)Parent->Get_ParentBone(index)->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTANKLE);

		if (!m_pBone[index])
		{
			MSGBOX(TEXT("Constructre failed in Test Copy"));
		}

		_float arrBoneRadius[4] = { 0.05f, 0.05f, -0.05f, 0.f };

		stTransDesc = pRootTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_ROOT);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_ROOT, stTransDesc, arrBoneRadius);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_ROOT));
		m_vecBoneTransfrom[index][ROOT]->Set_State(CAniTransform::ANI_TRANS_POSITION, _float3(10.f, 0.f, 10.f));

		stTransDesc = pPelvisTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_ROOT, KEY_BONE_PELVIS);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS));

		stTransDesc = pSpineTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_SPINE);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_SPINE, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS, KEY_BONE_SPINE);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_SPINE));

		stTransDesc = pNeckTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_NECK);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_NECK, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_SPINE, KEY_BONE_NECK);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_NECK));

		stTransDesc = pLeftLegTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTLEG);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTLEG, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS, KEY_BONE_LEFTLEG);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTLEG));

		stTransDesc = pRightLegTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTLEG);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTLEG, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS, KEY_BONE_RIGHTLEG);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTLEG));

		stTransDesc = pLeftArmTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTARM);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTARM, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_SPINE, KEY_BONE_LEFTARM);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTARM));

		stTransDesc = pRightArmTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTARM);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTARM, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_SPINE, KEY_BONE_RIGHTARM);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTARM));

		stTransDesc = pLeftElbowTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTELBOW);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTELBOW, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTARM, KEY_BONE_LEFTELBOW);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTELBOW));

		stTransDesc = pRightElbowTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTELBOW);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTELBOW, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTARM, KEY_BONE_RIGHTELBOW);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTELBOW));

		stTransDesc = pLeftAnkleTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTANKLE);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTANKLE, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTLEG, KEY_BONE_LEFTANKLE);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTANKLE));

		stTransDesc = pRightAnkleTransform->Get_TansDesc();
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTANKLE);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTANKLE, stTransDesc, arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTLEG, KEY_BONE_RIGHTANKLE);
		m_vecBoneTransfrom[index].push_back((CAniTransform*)m_pBone[index]->Find_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTANKLE));

	}
	m_vecAniInfo.push_back(Parent->Get_TestStruct(TEST_STATE_IDLE));
	m_vecAniInfo.push_back(Parent->Get_TestStruct(TEST_STATE_ATTACK));
	m_vecAniInfo.push_back(Parent->Get_TestStruct(TEST_STATE_STUN));
	m_vecAniInfo.push_back(Parent->Get_TestStruct(TEST_STATE_DIE));


	if (!FAILED(Set_AniCubeInfo(m_vecAniCubeInfo[ANI_RECT_HEAD], CUBE_HEAD, L"../Bin/Resources/Ani/CKMan.jpg")))
	{
		m_vecAniCube.push_back((CVIBuffer_Ani_Cube*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY,
			TEXT("Prototype_Component_AniBody"), &m_stBodyInfo));
	}

	if (!FAILED(Set_AniCubeInfo(m_vecAniCubeInfo[ANI_RECT_BODY], CUBE_BODY, L"../Bin/Resources/Ani/CKMan.jpg")))
	{
		m_vecAniCube.push_back((CVIBuffer_Ani_Cube*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY,
			TEXT("Prototype_Component_AniBody"), &m_stBodyInfo));
	}

	if (!FAILED(Set_AniCubeInfo(m_vecAniCubeInfo[ANI_RECT_LEFT_LEG], CUBE_LEFTLEG, L"../Bin/Resources/Ani/CKMan.jpg")))
	{
		m_vecAniCube.push_back((CVIBuffer_Ani_Cube*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY,
			TEXT("Prototype_Component_AniBody"), &m_stBodyInfo));
	}

	if (!FAILED(Set_AniCubeInfo(m_vecAniCubeInfo[ANI_RECT_RIGHT_LEG], CUBE_RIGHTLEG, L"../Bin/Resources/Ani/CKMan.jpg")))
	{
		m_vecAniCube.push_back((CVIBuffer_Ani_Cube*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY,
			TEXT("Prototype_Component_AniBody"), &m_stBodyInfo));
	}

	if (!FAILED(Set_AniCubeInfo(m_vecAniCubeInfo[ANI_RECT_LEFT_ARM], CUBE_LEFTARM, L"../Bin/Resources/Ani/CKMan.jpg")))
	{
		m_vecAniCube.push_back((CVIBuffer_Ani_Cube*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY,
			TEXT("Prototype_Component_AniBody"), &m_stBodyInfo));
	}

	if (!FAILED(Set_AniCubeInfo(m_vecAniCubeInfo[ANI_RECT_RIGHT_ARM], CUBE_RIGHTARM, L"../Bin/Resources/Ani/CKMan.jpg")))
	{
		m_vecAniCube.push_back((CVIBuffer_Ani_Cube*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY,
			TEXT("Prototype_Component_AniBody"), &m_stBodyInfo));
	}

	if (!FAILED(Set_AniCubeInfo(m_vecAniCubeInfo[ANI_RECT_LEFT_ELBOW], CUBE_LEFTELBOW, L"../Bin/Resources/Ani/CKMan.jpg")))
	{
		m_vecAniCube.push_back((CVIBuffer_Ani_Cube*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY,
			TEXT("Prototype_Component_AniBody"), &m_stBodyInfo));
	}

	if (!FAILED(Set_AniCubeInfo(m_vecAniCubeInfo[ANI_RECT_RIGHT_ELBOW], CUBE_RIGHTELBOW, L"../Bin/Resources/Ani/CKMan.jpg")))
	{
		m_vecAniCube.push_back((CVIBuffer_Ani_Cube*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY,
			TEXT("Prototype_Component_AniBody"), &m_stBodyInfo));
	}

	if (!FAILED(Set_AniCubeInfo(m_vecAniCubeInfo[ANI_RECT_LEFT_ANKLE], CUBE_LEFTANKLE, L"../Bin/Resources/Ani/CKMan.jpg")))
	{
		m_vecAniCube.push_back((CVIBuffer_Ani_Cube*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY,
			TEXT("Prototype_Component_AniBody"), &m_stBodyInfo));
	}

	if (!FAILED(Set_AniCubeInfo(m_vecAniCubeInfo[ANI_RECT_RIGHT_ANKLE], CUBE_RIGHTANKLE, L"../Bin/Resources/Ani/CKMan.jpg")))
	{
		m_vecAniCube.push_back((CVIBuffer_Ani_Cube*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY,
			TEXT("Prototype_Component_AniBody"), &m_stBodyInfo));
	}

	Safe_Release(pGameInstace);
}

void CTest::Tick(_float _fDeltaTime)
{
	__super::Tick(_fDeltaTime);

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (!m_vecBoneTransfrom.size())
		return;

	_float  fDir = 0.f;

	if (m_vecAniInfo[m_eAniState].uMotion >> INV_ROOT & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[ROOT] >> INV_ANI_END & 0x01))
	{
		_float fResult = m_vecBoneTransfrom[m_eAniState][ROOT]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[ROOT], _fDeltaTime);

		switch (m_eAniState)
		{
		case TEST_STATE_DIE:
			if (fResult >= m_vecAniInfo[m_eAniState].fAngle[ROOT])
			{
				m_vecAniInfo[m_eAniState].uFlag[ROOT] |= (0x01 << INV_ANI_END);
			}
			break;
		}
	}


	if (m_vecAniInfo[m_eAniState].uMotion >> INV_SPINE & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[SPINE] >> INV_ANI_END & 0x01))
		{
			_float fResult = m_vecBoneTransfrom[m_eAniState][SPINE]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[SPINE], _fDeltaTime);

			switch (m_eAniState)
			{
			case TEST_STATE_DIE:
				if (fResult >= m_vecAniInfo[m_eAniState].fAngle[SPINE])
				{
					m_vecAniInfo[m_eAniState].uFlag[SPINE] |= (0x01 << INV_ANI_END);
				}
				break;
			}
		}

	if (m_vecAniInfo[m_eAniState].uMotion >> INV_PELVIS & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[PELVIS] >> INV_ANI_END & 0x01))
		{
			_float fResult = m_vecBoneTransfrom[m_eAniState][PELVIS]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[PELVIS], _fDeltaTime);

			switch (m_eAniState)
			{
			case TEST_STATE_DIE:
				if (fResult >= m_vecAniInfo[m_eAniState].fAngle[PELVIS])
				{
					m_vecAniInfo[m_eAniState].uFlag[PELVIS] |= (0x01 << INV_ANI_END);
				}
				break;
			}
		}

	if (m_vecAniInfo[m_eAniState].uMotion >> INV_NECK & 0x01)
			if (!(m_vecAniInfo[m_eAniState].uFlag[NECK] >> INV_ANI_END & 0x01))
			{
				_float fResult = m_vecBoneTransfrom[m_eAniState][NECK]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[NECK], _fDeltaTime);

				switch (m_eAniState)
				{
				case TEST_STATE_DIE:
					if (fResult >= m_vecAniInfo[m_eAniState].fAngle[NECK])
					{
						m_vecAniInfo[m_eAniState].uFlag[NECK] |= (0x01 << INV_ANI_END);
					}
					break;
				}

			}

	if (m_vecAniInfo[m_eAniState].uMotion >> INV_LEFTLEG & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[LEFTLEG] >> INV_ANI_END & 0x01))
		{
			_float fResult = m_vecBoneTransfrom[m_eAniState][LEFTLEG]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[LEFTLEG], _fDeltaTime);

			switch (m_eAniState)
			{
			case TEST_STATE_DIE:
				if (fResult >= m_vecAniInfo[m_eAniState].fAngle[LEFTLEG])
				{
					m_vecAniInfo[m_eAniState].uFlag[LEFTLEG] |= (0x01 << INV_ANI_END);
				}
				break;
			}
		}

	if (m_vecAniInfo[m_eAniState].uMotion >> INV_RIGHTLEG & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[RIGHTLEG] >> INV_ANI_END & 0x01))
		{
			_float fResult = m_vecBoneTransfrom[m_eAniState][RIGHTLEG]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[RIGHTLEG], _fDeltaTime);

			switch (m_eAniState)
			{
			case TEST_STATE_DIE:
				if (fResult >= m_vecAniInfo[m_eAniState].fAngle[RIGHTLEG])
				{
					m_vecAniInfo[m_eAniState].uFlag[RIGHTLEG] |= (0x01 << INV_ANI_END);
				}
				break;
			}
		}

	if (m_vecAniInfo[m_eAniState].uMotion >> INV_LEFTARM & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[LEFTARM] >> INV_ANI_END & 0x01))
		{
			_float fResult = m_vecBoneTransfrom[m_eAniState][LEFTARM]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[LEFTARM], _fDeltaTime);

			switch (m_eAniState)
			{
			case TEST_STATE_DIE:
				if (fResult >= m_vecAniInfo[m_eAniState].fAngle[LEFTARM])
				{
					m_vecAniInfo[m_eAniState].uFlag[LEFTARM] |= (0x01 << INV_ANI_END);
				}
				break;
			}
		}

	if (m_vecAniInfo[m_eAniState].uMotion >> INV_RIGHTARM & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[RIGHTARM] >> INV_ANI_END & 0x01))
		{
			_float fResult = m_vecBoneTransfrom[m_eAniState][RIGHTARM]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[RIGHTARM], _fDeltaTime);

			switch (m_eAniState)
			{
			case TEST_STATE_DIE:
				if (fResult >= m_vecAniInfo[m_eAniState].fAngle[RIGHTARM])
				{
					m_vecAniInfo[m_eAniState].uFlag[RIGHTARM] |= (0x01 << INV_ANI_END);
				}
				break;
			}
		}
	
	if (m_vecAniInfo[m_eAniState].uMotion >> INV_LEFTELBOW & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[LEFTELBOW] >> INV_ANI_END & 0x01))
		{
			_float fResult = m_vecBoneTransfrom[m_eAniState][LEFTELBOW]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[LEFTELBOW], _fDeltaTime);

			switch (m_eAniState)
			{
			case TEST_STATE_DIE:
				if (fResult >= m_vecAniInfo[m_eAniState].fAngle[LEFTELBOW])
				{
					m_vecAniInfo[m_eAniState].uFlag[LEFTELBOW] |= (0x01 << INV_ANI_END);
				}
				break;
			}
		}

	if (m_vecAniInfo[m_eAniState].uMotion >> INV_RIGHTELBOW & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[RIGHTELBOW] >> INV_ANI_END & 0x01))
		{
			_float fResult = m_vecBoneTransfrom[m_eAniState][RIGHTELBOW]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[RIGHTELBOW], _fDeltaTime);

			switch (m_eAniState)
			{
			case TEST_STATE_DIE:
				if (fResult >= m_vecAniInfo[m_eAniState].fAngle[RIGHTELBOW])
				{
					m_vecAniInfo[m_eAniState].uFlag[RIGHTELBOW] |= (0x01 << INV_ANI_END);
				}
				break;
			}
		}

	if (m_vecAniInfo[m_eAniState].uMotion >> INV_LEFTANKLE & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[LEFTANKLE] >> INV_ANI_END & 0x01))
		{
			_float fResult = m_vecBoneTransfrom[m_eAniState][LEFTANKLE]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[LEFTANKLE], _fDeltaTime);

			switch (m_eAniState)
			{
			case TEST_STATE_DIE:
				if (fResult >= m_vecAniInfo[m_eAniState].fAngle[LEFTANKLE])
				{
					m_vecAniInfo[m_eAniState].uFlag[LEFTANKLE] |= (0x01 << INV_ANI_END);
				}
				break;
			}
		}

	if (m_vecAniInfo[m_eAniState].uMotion >> INV_RIGHTANKLE & 0x01)
		if (!(m_vecAniInfo[m_eAniState].uFlag[RIGHTANKLE] >> INV_ANI_END & 0x01))
		{
			_float fResult = m_vecBoneTransfrom[m_eAniState][RIGHTANKLE]->Turn_Local(m_vecAniInfo[m_eAniState].m_varr[RIGHTANKLE], _fDeltaTime);

			switch (m_eAniState)
			{
			case TEST_STATE_DIE:
				if (fResult >= m_vecAniInfo[m_eAniState].fAngle[RIGHTANKLE])
				{
					m_vecAniInfo[m_eAniState].uFlag[RIGHTANKLE] |= (0x01 << INV_ANI_END);
				}
				break;
			}
		}
	Safe_Release(pGameInstance);
}

D3DXMATRIX CTest::Set_Matrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vAxis, float fAngle)
{
	D3DXMATRIX Matrix;
	ZeroMemory(&Matrix, sizeof(D3DXMATRIX));
	D3DXMatrixRotationAxis(&Matrix, &vAxis, fAngle);
	Matrix.m[3][0] = vPos.x;
	Matrix.m[3][1] = vPos.y;
	Matrix.m[3][2] = vPos.z;
	Matrix.m[3][3] = 1;
	return Matrix;
}

void CTest::Late_Tick(_float _fDeltaTime)
{
	__super::Late_Tick(_fDeltaTime);
	
	m_iTerrainIndex = m_pTerrain->Get_TerrainRange(m_vecBoneTransfrom[m_eAniState][ROOT]->Get_State(CAniTransform::ANI_TRANS_POSITION), m_iTerrainIndexX, m_iTerrainIndexY);
	__super::SetUp_OnTerrain(m_vecBoneTransfrom[m_eAniState][ROOT], LEVEL_GAMEPLAY, TEXT("Layer_Terrain"));

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);

	m_pColliderCom->Set_Position(m_pAniTransformCom->Get_State(CAniTransform::ANI_TRANS_POSITION));

	m_pCollisionCom->Add_Collisions(m_iTerrainIndex, L"Monster", this);
	if (m_iTerrainIndexX != -1)
		m_pCollisionCom->Add_Collisions(m_iTerrainIndexX, L"Monster", this);
	if (m_iTerrainIndexY != -1)
		m_pCollisionCom->Add_Collisions(m_iTerrainIndexY, L"Monster", this);

	__super::Affected_By_Gravity(m_pAniTransformCom, _fDeltaTime);

	if (m_eAniMemoState != m_eAniState)
	{
		Clear_Flag(m_eAniState, _fDeltaTime);
		/* For.Com_Collider */
		m_pAniTransformCom = m_vecBoneTransfrom[m_eAniState][ROOT];
	}

	switch (m_eAniState)
	{
	case TEST_STATE_IDLE:
		if (!FAILED(Search_Target(L"Layer_Player", &m_pTarget, 100.f)))
		{
			CTransform* PlayerTransform = ((CPlayer*)m_pTarget)->Get_PlayerTransform();
			float fCos, fDot, fScale;
			_float3 PlayerPos = PlayerTransform->Get_State(CTransform::STATE_POSITION);
			_float3 MonsterPos = m_vecBoneTransfrom[TEST_STATE_IDLE][ROOT]->Get_State(CAniTransform::ANI_TRANS_POSITION);
			fDot = D3DXVec3Dot(&PlayerPos, &MonsterPos);
			fScale = D3DXVec3Length(&PlayerPos) * D3DXVec3Length(&MonsterPos);
			fCos = fDot / fScale;
			m_vecBoneTransfrom[TEST_STATE_IDLE][NECK]->TurnAngle(_float3{ 0.f, -1.0f, 0.f }, fCos);
		}
		break;
	case TEST_STATE_ATTACK:
		if (!FAILED(Search_Target(L"Layer_Player", &m_pTarget, 5.f)))
		{
			CTransform* PlayerTransform = ((CPlayer*)m_pTarget)->Get_PlayerTransform();
			float fCos, fDot, fScale;
			_float3 PlayerPos = PlayerTransform->Get_State(CTransform::STATE_POSITION);
			_float3 MonsterPos = m_vecBoneTransfrom[TEST_STATE_ATTACK][ROOT]->Get_State(CAniTransform::ANI_TRANS_POSITION);
			fDot = D3DXVec3Dot(&PlayerPos, &MonsterPos);
			fScale = D3DXVec3Length(&PlayerPos) * D3DXVec3Length(&MonsterPos);
			fCos = fDot / fScale;
			m_vecBoneTransfrom[TEST_STATE_ATTACK][ROOT]->LookAt(PlayerTransform->Get_State(CTransform::STATE_POSITION));
		}
		else
		{
			m_eAniState = TEST_STATE_IDLE;
		}
		break;
	case TEST_STATE_STUN:
		if (0 >= m_iHp)
			Set_Dead();
		else
			--m_iHp;
		break;
	}

}

HRESULT CTest::Render()
{
	if (!m_pGraphic_Device) //|| !m_pBoneRootCube || !m_pBonePelvisCube || !m_pBoneSpineCube)
		return E_FAIL;

	if (__super::Render())
		return E_FAIL;

	m_pBone[m_eAniState]->Render();

	m_vecAniCube[ANI_RECT_HEAD]->Render(m_vecBoneTransfrom[m_eAniState][NECK]->Get_BindMatrix());

	m_vecAniCube[ANI_RECT_BODY]->Render(m_vecBoneTransfrom[m_eAniState][SPINE]->Get_BindMatrix());

	m_vecAniCube[ANI_RECT_LEFT_LEG]->Render(m_vecBoneTransfrom[m_eAniState][LEFTLEG]->Get_BindMatrix());

	m_vecAniCube[ANI_RECT_RIGHT_LEG]->Render(m_vecBoneTransfrom[m_eAniState][RIGHTLEG]->Get_BindMatrix());

	m_vecAniCube[ANI_RECT_LEFT_ARM]->Render(m_vecBoneTransfrom[m_eAniState][LEFTARM]->Get_BindMatrix());

	m_vecAniCube[ANI_RECT_RIGHT_ARM]->Render(m_vecBoneTransfrom[m_eAniState][RIGHTARM]->Get_BindMatrix());

	m_vecAniCube[ANI_RECT_LEFT_ELBOW]->Render(m_vecBoneTransfrom[m_eAniState][LEFTELBOW]->Get_BindMatrix());

	m_vecAniCube[ANI_RECT_RIGHT_ELBOW]->Render(m_vecBoneTransfrom[m_eAniState][RIGHTELBOW]->Get_BindMatrix());

	m_vecAniCube[ANI_RECT_LEFT_ANKLE]->Render(m_vecBoneTransfrom[m_eAniState][LEFTANKLE]->Get_BindMatrix());

	m_vecAniCube[ANI_RECT_RIGHT_ANKLE]->Render(m_vecBoneTransfrom[m_eAniState][RIGHTANKLE]->Get_BindMatrix());

	return S_OK;
}


HRESULT CTest::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTest::NativeConstruct_Prototype(void* pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	CGameInstance* pGameInstace = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstace);

	ST_GAMEOBJECT_FILE_INFO* pFileInfo = (ST_GAMEOBJECT_FILE_INFO*)pArg;

	//Ready to Get File

	for (int index = 0; index < TEST_STATE_END; ++index)
	{

		m_pBone.push_back((CBone*)pGameInstace->Clone_Component(LEVEL_GAMEPLAY, KEY_COMPONENT_BONE, NULL));
		_float arrBoneRadius[4] = { 0.05f, 0.05f, -0.05f, 0.f };

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_ROOT);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_ROOT, pFileInfo->vecBoneTransform[index][ROOT], arrBoneRadius);
		m_stAniDescl[index] = pFileInfo->vecBoneTransform[index][ROOT];
		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS, pFileInfo->vecBoneTransform[index][PELVIS], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_ROOT, KEY_BONE_PELVIS);

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_SPINE);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_SPINE, pFileInfo->vecBoneTransform[index][SPINE], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS, KEY_BONE_SPINE);

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, L"Bone_Neck");
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, L"Bone_Neck", pFileInfo->vecBoneTransform[index][NECK], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_SPINE, L"Bone_Neck");

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTLEG);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTLEG, pFileInfo->vecBoneTransform[index][LEFTLEG], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS, KEY_BONE_LEFTLEG);

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTLEG);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTLEG, pFileInfo->vecBoneTransform[index][RIGHTLEG], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_PELVIS, KEY_BONE_RIGHTLEG);

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTARM);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTARM, pFileInfo->vecBoneTransform[index][LEFTARM], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_SPINE, KEY_BONE_LEFTARM);

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTARM);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTARM, pFileInfo->vecBoneTransform[index][RIGHTARM], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_SPINE, KEY_BONE_RIGHTARM);

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTELBOW);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTELBOW, pFileInfo->vecBoneTransform[index][LEFTELBOW], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTARM, KEY_BONE_LEFTELBOW);

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTELBOW);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTELBOW, pFileInfo->vecBoneTransform[index][RIGHTELBOW], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTARM, KEY_BONE_RIGHTELBOW);

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTANKLE);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_LEFTANKLE, pFileInfo->vecBoneTransform[index][LEFTANKLE], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_LEFTLEG, KEY_BONE_LEFTANKLE);

		m_pBone[index]->EraseBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTANKLE);
		m_pBone[index]->Add_Bone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTANKLE, pFileInfo->vecBoneTransform[index][RIGHTANKLE], arrBoneRadius);
		m_pBone[index]->Setting_ParentBone(BONE_LAYER_PLAYER, KEY_BONE_RIGHTLEG, KEY_BONE_RIGHTANKLE);
	}

	m_vecAniInfo.push_back(pFileInfo->vecAniInfo[TEST_STATE_IDLE]);
	m_vecAniInfo.push_back(pFileInfo->vecAniInfo[TEST_STATE_ATTACK]);
	m_vecAniInfo.push_back(pFileInfo->vecAniInfo[TEST_STATE_STUN]);
	m_vecAniInfo.push_back(pFileInfo->vecAniInfo[TEST_STATE_DIE]);

	m_vecAniCubeInfo.push_back(pFileInfo->vecAniCube[ANI_RECT_HEAD]);
	m_vecAniCubeInfo.push_back(pFileInfo->vecAniCube[ANI_RECT_BODY]);
	m_vecAniCubeInfo.push_back(pFileInfo->vecAniCube[ANI_RECT_LEFT_LEG]);
	m_vecAniCubeInfo.push_back(pFileInfo->vecAniCube[ANI_RECT_RIGHT_LEG]);
	m_vecAniCubeInfo.push_back(pFileInfo->vecAniCube[ANI_RECT_LEFT_ARM]);
	m_vecAniCubeInfo.push_back(pFileInfo->vecAniCube[ANI_RECT_RIGHT_ARM]);
	m_vecAniCubeInfo.push_back(pFileInfo->vecAniCube[ANI_RECT_LEFT_ELBOW]);
	m_vecAniCubeInfo.push_back(pFileInfo->vecAniCube[ANI_RECT_RIGHT_ELBOW]);
	m_vecAniCubeInfo.push_back(pFileInfo->vecAniCube[ANI_RECT_LEFT_ANKLE]);
	m_vecAniCubeInfo.push_back(pFileInfo->vecAniCube[ANI_RECT_RIGHT_ANKLE]);

	Safe_Release(pGameInstace);
	return S_OK;
}

HRESULT CTest::NativeConstruct(void* pArg)
{

	if (!m_pGraphic_Device)
		return E_FAIL;

	typedef struct tagCSanta
	{
		CTest* pObject;
		TEXMONDATA eData;
	}St_CSanta;
	St_CSanta stTest;
	stTest = *(St_CSanta*)pArg;

	if (FAILED(__super::NativeConstruct(&stTest.eData.vPos)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_vecBoneTransfrom[m_eAniState][ROOT]->Set_State(CAniTransform::ANI_TRANS_POSITION, stTest.eData.vPos);
	m_pAniTransformCom = m_vecBoneTransfrom[m_eAniState][ROOT];

	return S_OK;
}

HRESULT CTest::SetUp_Components()
{
	CGameInstance* pGameInstace = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstace);
	m_pRendererCom = ((CRenderer*)pGameInstace->Clone_Component(0, TEXT("Prototype_Component_Renderer")));
	if (NULL == m_pRendererCom)
	{
		Safe_Release(m_pRendererCom);
		Safe_Release(pGameInstace);
		MSGBOX(TEXT("Failed to Init CTest"));
		return E_FAIL;
	}

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Collision */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}

	/* For.Com_Collider */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, _float3(2.f, 0.f, 2.f))))
	{
		MSGBOX(L"Failed : Add_Components Prototype_Component_Collider");
		return E_FAIL;
	}

	/* For.Com_Collider */
	if (FAILED(__super::Bind(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AniTransform"), TEXT("Com_AniTransform"), (CComponent**)&m_vecBoneTransfrom[m_eAniState][ROOT])))
	{
		MSGBOX(L"Failed : Add_Components Prototype_Component_AniTransform");
		return E_FAIL;
	}

	m_vecBoneTransfrom[m_eAniState][ROOT]->Set_TransformDesc(m_stAniDescl[m_eAniState]);

	CBattle::BATTLEDATA		BattleData;
	ZeroMemory(&BattleData, sizeof(CBattle::BATTLEDATA));

	BattleData.iAtk = 2;
	BattleData.iDeffence = 0;
	BattleData.iMaxHP = 1;
	BattleData.iHP = 1;
	BattleData.fAttack_Delay = 180;

	/* For.Com_Battle */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"), TEXT("Com_Battle"), (CComponent**)&m_pBattleCom, &BattleData)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}

	m_pTarget = pGameInstace->Get_Target(D3DXVECTOR3(1.f, 1.f, 1.f), 200.f, LEVEL_GAMEPLAY, L"Layer_Player");

	Safe_AddRef(m_pRendererCom);
	Safe_Release(pGameInstace);
	return S_OK;
}


CGameObject* CTest::Clone(void* pArg)
{
	CTest * pInstance = new CTest(*this);

	if (!pInstance)
		return NULL;
	typedef struct tagCTest
	{
		CTest* pObject;
		TEXMONDATA eData;
	}St_CTest;
	St_CTest stTest;
	ZeroMemory(&stTest, sizeof(St_CTest));
	stTest.pObject = this;
	stTest.eData = *(TEXMONDATA*)pArg;
	if (FAILED(pInstance->NativeConstruct(&stTest)))
		return NULL;

	return pInstance;
}

void CTest::Free()
{
	__super::Free();
	Safe_Release(m_pRendererCom);

	for (int index = 0; index < TEST_STATE_END; ++index)
	{
		Safe_Release(m_pBone[index]);
	}

	for (int index = 0; index < ANI_RECT_END; ++index)
	{
		Safe_Release(m_vecAniCube[index]);
	}
}

void CTest::Clear_Flag(TEST_STATE eState, _float _fDeltaTime)
{
	for (int i = 0; i < ANIMATION_STATE_END; ++i)
	{
		m_vecAniInfo[eState].uFlag[i] = 0x00;
	}
	if(m_eAniMemoState != TEST_STATE_END)
		m_vecBoneTransfrom[m_eAniState][ROOT]->Set_State(CAniTransform::ANI_TRANS_POSITION, m_vecBoneTransfrom[m_eAniMemoState][ROOT]->Get_State(CAniTransform::ANI_TRANS_POSITION));
	
	m_eAniMemoState = m_eAniState;
}

CGameObject* CTest::Create(LPDIRECT3DDEVICE9 _pDevice, ST_GAMEOBJECT_FILE_INFO& stFileInfo)
{
	CTest * pInstance = new CTest(_pDevice);
	if (!pInstance)
		return NULL;

	if (FAILED(pInstance->NativeConstruct_Prototype(&stFileInfo)))
		return NULL;

	return pInstance;
}

CBone* CTest::Get_ParentBone(int index)
{
	return m_pBone[index];
}

TEST_STRUCT CTest::Get_TestStruct(TEST_STATE eTestState)
{
	return m_vecAniInfo[eTestState];
}

HRESULT CTest::Set_AniCubeInfo(STSHAPE CubeInfo, ANI_CUBE eCubeState, CString strFilePath)
{
	memcpy(&m_stBodyInfo, &CubeInfo, sizeof(float) * 4);
	m_stBodyInfo.eCube = eCubeState;
	m_stBodyInfo.strFilePath = strFilePath;
	return S_OK;
}

HRESULT CTest::Search_Target(const _tchar* pTargetTag, CGameObject** pTarget, _float fRange)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	// Target : Player, Tower, Monster
	*pTarget = pGameInstance->Get_Target(m_vecBoneTransfrom[m_eAniState][ROOT]->Get_State(CAniTransform::ANI_TRANS_POSITION), fRange, LEVEL_GAMEPLAY, pTargetTag);
	if (nullptr == *pTarget)
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}