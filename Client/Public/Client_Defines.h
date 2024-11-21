#pragma once

#include <process.h>
#include "Engine_Defines.h"

namespace Client 
{

	const unsigned int g_iWinCX = 1280;
	const unsigned int g_iWinCY = 720;

	/* LEVEL_STATIC : �Ҵ�Ǳ����� ������ �ƴϴ�. ���� �ΰ��ӳ����� ���Ǵ� ������Ʈ���� �Ŵ����鿡�� ���. */ 
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };
	enum CAMERA { CAMERA_PLAYER, CAMERA_FREE };
	enum ATKCLASS { ATK_PLAYER, ATK_ENEMY };
	enum BONE_LAYER { BONE_LAYER_PLAYER, BONE_LAYER_MONSTER, BONE_LAYER_ELBOW, BONE_LAYER_END };
	//��Ƽ�깫�� ���� ����ü
	enum WEAPONLIST { PLASMA, BIBLE, WHIP, FIRE_BALL, GARLIC, METEO, LIGHTNING, CAT, SONG_OF_MANA };
	enum PASSIVELIST {};
	//0316 �ؽ��� ���Ϳ� ����ü
	enum TEXMONSTER { MONSTER_BAT, MONSTER_SKELETON, MONSTER_LIZARDMAN, MONSTER_WRAITH, MONSTER_DRAGO, MONSTER_UNDEAD, MONSTER_END };

	typedef struct tagAttackCol
	{
		ATKCLASS eAtkClass;
		float fAttack;
		D3DXVECTOR3 vSize;
		D3DXVECTOR3 vPosition;
		D3DXVECTOR3 vAttackDir;		//���󰡴� ����
		float	fAttacktedPower;	//���󰡴� ��

	}ATTACK_COLLISION;

	//��Ƽ�� ������ ���� ������ �ʿ��� ������
	typedef struct tagActiveData
	{
		D3DXVECTOR3 vRight;
		D3DXVECTOR3 vUp;
		D3DXVECTOR3 vLook;
		D3DXVECTOR3 vPos;
		int			iLevel;
		float		fPower;
		float		fArea;
		float		fSpeed;
		float		fDuration;
		int			iEffect;

	}ACTIVE_RECIEVE_DATA;

	typedef struct tagITEM
	{
		_float3			vPosition;
		_uint			iType;
	}ITEM;

	//0316 �ؽ��� ���Ͱ� �޾ƾ� �ϴ� ����
	typedef struct tagTexMonData
	{
		D3DXVECTOR3 vPos; //���� ����Ǵ� ��ġ
		TEXMONSTER eTex; //������ �ؽ�������

	}TEXMONDATA;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
using namespace Client;
