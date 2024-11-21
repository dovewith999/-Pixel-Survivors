#pragma once

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CAnimation_Parser final : public CComponent
{
private:
	const int   LEN_POS = 2;
	const int	LEN_BONE = 2;
	const int	LEN_ANI_CUBE = 2;
	const int	LEN_ANI_INFO = 2;
	const int TOTAL_LEN_SIZE = LEN_POS + LEN_BONE + LEN_ANI_CUBE + LEN_ANI_INFO;
public:
	enum PARSER_STATE { BONE, ANIMATION, ANIMATION_INFO };
	enum DATA_STATE { LEN_STEP1, LEN_STEP2, DATA };
private:
	int			 m_iToTalIndex = 0;
	int			 m_iAniCubeIndex = 0;
	int          m_iBoneIndex = 0;
	int          m_iAniInfoIndex = 0;
public:

	HRESULT Load_Process_Animation(CString FileName, vector<vector<ANIMATIONDESC>>& pVecTransform, vector<STSHAPE>& pVecSphere, vector<TEST_STRUCT>& pVeCAngryBirdStruct);
private:
	explicit CAnimation_Parser(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CAnimation_Parser(const CAnimation_Parser& rhs);
	virtual ~CAnimation_Parser() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);


public:
	static CComponent* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};
END
