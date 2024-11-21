#pragma once

#include <process.h>
#include "Engine_Defines.h"

namespace Client 
{

	const unsigned int g_iWinCX = 1280;
	const unsigned int g_iWinCY = 720;

	/* LEVEL_STATIC : 할당되기위한 레벨은 아니다. 각종 인게임내에서 사용되는 오브젝트들의 매니져들에서 사용. */ 
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };
	enum CAMERA { CAMERA_PLAYER, CAMERA_FREE };
	enum ATKCLASS { ATK_PLAYER, ATK_ENEMY };
	enum BONE_LAYER { BONE_LAYER_PLAYER, BONE_LAYER_MONSTER, BONE_LAYER_ELBOW, BONE_LAYER_END };
	//액티브무기 관련 열거체
	enum WEAPONLIST { PLASMA, BIBLE, WHIP, FIRE_BALL, GARLIC, METEO, LIGHTNING, CAT, SONG_OF_MANA };
	enum PASSIVELIST {};
	//0316 텍스쳐 몬스터용 열거체
	enum TEXMONSTER { MONSTER_BAT, MONSTER_SKELETON, MONSTER_LIZARDMAN, MONSTER_WRAITH, MONSTER_DRAGO, MONSTER_UNDEAD, MONSTER_END };

	typedef struct tagAttackCol
	{
		ATKCLASS eAtkClass;
		float fAttack;
		D3DXVECTOR3 vSize;
		D3DXVECTOR3 vPosition;
		D3DXVECTOR3 vAttackDir;		//날라가는 방향
		float	fAttacktedPower;	//날라가는 힘

	}ATTACK_COLLISION;

	//액티브 아이템 공격 생성에 필요한 데이터
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

	//0316 텍스쳐 몬스터가 받아야 하는 정보
	typedef struct tagTexMonData
	{
		D3DXVECTOR3 vPos; //몬스터 생산되는 위치
		TEXMONSTER eTex; //몬스터의 텍스쳐종류

	}TEXMONDATA;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
using namespace Client;
