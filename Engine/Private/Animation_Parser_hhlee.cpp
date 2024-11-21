#include "../Public/Animation_Parser_hhlee.h"

CAnimation_Parser::CAnimation_Parser(LPDIRECT3DDEVICE9 pGraphic_Device) :CComponent(pGraphic_Device)
{
}

CAnimation_Parser::CAnimation_Parser(const CAnimation_Parser& rhs) : CComponent(rhs)
{
	m_iAniCubeIndex = sizeof(ANIMATIONDESC) * ANIMATION_STATE_END;
	m_iBoneIndex = sizeof(STSHAPE) * ANI_RECT_END;
	m_iAniInfoIndex = sizeof(TEST_STRUCT) * TEST_STATE_END;

	m_iToTalIndex = m_iAniCubeIndex + m_iBoneIndex + m_iAniInfoIndex + TOTAL_LEN_SIZE + 1;
}


CComponent* CAnimation_Parser::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAnimation_Parser* pIns = new CAnimation_Parser(pGraphic_Device);

	if (FAILED(pIns->NativeConstruct_Prototype()))
		return NULL;

	return pIns;
}

CComponent* CAnimation_Parser::Clone(void* pArg)
{
	CAnimation_Parser* pIns = new CAnimation_Parser(*this);

	if (FAILED(pIns->NativeConstruct(pArg)))
		return NULL;

	return pIns;
}

HRESULT CAnimation_Parser::NativeConstruct_Prototype()
{
	return S_OK;
}
HRESULT CAnimation_Parser::NativeConstruct(void* pArg)
{
	return S_OK;
}

void CAnimation_Parser::Free()
{

}


HRESULT CAnimation_Parser::Load_Process_Animation(CString FileName, vector<vector<ANIMATIONDESC>>& pVecTransform, vector<STSHAPE>& pVecSphere, vector<TEST_STRUCT>& pVeCAngryBirdStruct)
{
	ifstream infile(FileName, ios::in | ios::binary);

	_ubyte szLen[2];
	ZeroMemory(&szLen, 0);
	infile.read((char*)szLen, 2);

	unsigned int ReadDataSize = (szLen[0] << 8) & 0xFF00;
	ReadDataSize += (szLen[1] & 0xFF);

	cout << "[Read File] " << ReadDataSize << endl;

	_ubyte* ReadBuffer = new _ubyte[ReadDataSize + 1];

	ZeroMemory(ReadBuffer, sizeof(_ubyte) * ReadDataSize + 1);

	unsigned int iIndex = 0;

	//Ready to FileParse
	for (int count = 0; count < TEST_STATE_END; ++count)
	{
		pVecTransform.push_back(vector<ANIMATIONDESC>());
	}
	while (iIndex < ReadDataSize)
	{
		infile.read((char*)ReadBuffer, 1);
		++ReadBuffer;
		++iIndex;
	}

	ReadBuffer -= ReadDataSize;

	if (iIndex == ReadDataSize)
	{
		cout << "Read Index check" << endl;
	}

	int iCase = BONE;
	int iDataCase = LEN_STEP1;
	uint16_t Len = 0;
	int iDataIndex = 0;
	int iAnimationIndex = 0;
	ANIMATIONDESC * pTranformDesc;
	STSHAPE * pShape;
	TEST_STRUCT * pTestStruct;

	byte* pArray = NULL;

	_float arrBoneRadius[4] = { 0.05f, 0.05f, -0.05f, 0.f };

	for (unsigned int index = 0; index < ReadDataSize; ++index)
	{

		switch (iDataCase)
		{
		case LEN_STEP1:
			Len = (ReadBuffer[index] << 8) & 0xFF00;
			iDataCase = LEN_STEP2;
			break;
		case LEN_STEP2:
			Len += (ReadBuffer[index] << 0) & 0xFF;
			iDataCase = DATA;
			break;
		case DATA:
			switch (iCase)
			{
			case BONE:
				for (int i = 0; i < sizeof(ANIMATIONDESC); ++i)
				{
					if (0 == i)
					{
						pTranformDesc = new ANIMATIONDESC;
						pArray = (byte*)pTranformDesc;
					}

					pArray[i] = ReadBuffer[index];

					index++;

					if (sizeof(ANIMATIONDESC) - 1 == i)
					{
						static int k = 0;
						if (k > 0)
						{
							ANIMATIONDESC st = *(ANIMATIONDESC*)pArray;
						}
						else
						{
							k++;
						}
						pVecTransform[iAnimationIndex].push_back(*(ANIMATIONDESC*)pArray);
						index--;
						Safe_Delete(pTranformDesc);
					}
				}
				++iDataIndex;
				if (iDataIndex == ANIMATION_STATE_END)
				{
					iDataIndex = 0;
					iAnimationIndex++;

					if (iAnimationIndex * ANIMATION_STATE_END == Len)
					{
						iDataCase = LEN_STEP1;
						iCase = ANIMATION;
						Len = 0;
					}
				}
				break;
			case ANIMATION:
				for (int i = 0; i < sizeof(STSHAPE); ++i)
				{
					if (0 == i)
					{
						pShape = new STSHAPE;
						pArray = (byte*)pShape;
					}

					pArray[i] = ReadBuffer[index];

					index++;

					if (sizeof(STSHAPE) - 1 == i)
					{
						STSHAPE st;
						pVecSphere.push_back(*pShape);
						index--;
						Safe_Delete(pShape);
					}
				}
				++iDataIndex;

				if (iDataIndex == Len)
				{
					iDataIndex = 0;
					iDataCase = LEN_STEP1;
					iCase = ANIMATION_INFO;
					Len = 0;
				}
				break;
			case ANIMATION_INFO:
				for (int i = 0; i < sizeof(TEST_STRUCT); ++i)
				{
					if (0 == i)
					{
						pTestStruct = new TEST_STRUCT;
						pArray = (byte*)pTestStruct;
					}

					pArray[i] = ReadBuffer[index];
					//TEST_STATE_IDLE
					index++;
					if (sizeof(TEST_STRUCT) - 1 == i)
					{
						TEST_STRUCT st = *(TEST_STRUCT*)pTestStruct;
						cout << "*****Test*****" << endl;
						cout << st.uMotion << endl;

						pVeCAngryBirdStruct.push_back(*(TEST_STRUCT*)pTestStruct);
						index--;
						delete pTestStruct;
					}
				}
				++iDataIndex;
				if (iDataIndex == Len)
				{
					iDataIndex = 0;
					iDataCase = LEN_STEP1;
					return S_OK;
				}
				break;
			}
			break;
		}
	}
	infile.close();

	Safe_Delete_Array(ReadBuffer);

	return E_FAIL;
}
