#pragma once

#include "hhlee_Struct.h"
#include <string>

namespace Engine
{
	// D3DFVF_XYZ
	// D3DDECLUSAGE

	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;				
	}VTXTEX;

	typedef struct tagVertex_Cube_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexUV;
	}VTXCUBETEX;

	typedef struct tagFaceIndices16
	{
		unsigned short		_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_0, _1, _2;
	}FACEINDICES32;

	typedef struct Graphic_Device_Desc
	{
		enum MODE { MODE_FULLMODE, MODE_WINMODE, MODE_END };

		MODE			eWinMode = MODE_END;
		HWND			hWnd = 0;
		unsigned int	iWinCX = 0;
		unsigned int	iWinCY = 0;

	}GRAPHIC_DEVICE_DESC;

	typedef struct tagVertex_Color
	{
		D3DXVECTOR3		vPosition;
		D3DCOLOR		dwColor;
	}VERTEXCOLOR;

	typedef struct Ray
	{
		D3DXVECTOR3 vOrigin;
		D3DXVECTOR3 vDirection;
	}RAY;


	typedef struct tagVertex_Texture_Terrain
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
	}VERTEXTXT;

	typedef struct tagCube
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vSize;
		D3DCOLOR		dwColor;
	}CUBE;

	typedef struct tagRect_Range
	{
		D3DXVECTOR3		vLeftUpPos;
		D3DXVECTOR3		vRightUpPos;
		D3DXVECTOR3		vRightDownPos;
		D3DXVECTOR3		vLeftDownPos;
	}RECTRANGE;

	typedef struct tagCollider
	{
		D3DXVECTOR3		vPosition;
	}COLLIDER;

	typedef struct tagWeaponStats
	{
		unsigned int iAttackDmg;
		const wchar_t* szName = nullptr;
		enum WEAPONTYPE { WEAPON_NONE = 0, WEAPON_SWORD, WEAPON_SPEAR, WEAPON_GUN, WEAPON_RIFLE, WEAPON_BULLET, WEAPON_END };
		WEAPONTYPE eWeaponType;
	}WEAPONSTATS;

	typedef struct tagTile
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vSize;
		unsigned int	iDrawID;
		unsigned int	iOption;

		int				iIndex;
		int				iParentIndex;
	}TILE;

	typedef struct tagTerrain
	{
		//enum ROLE { ROLE_BACK, ROLE_FRONT, ROLE_LEFT, ROLE_RIGHT, ROLE_TERRAIN, ROLE_END };

		//ROLE			eRole;
		D3DXVECTOR3		vSize;
		D3DXVECTOR3		vBasePos;
		unsigned int	iDrawID;
	}TERRAIN;

	typedef struct tagTerrainCube
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vSize;
		unsigned int	iDrawID;
	}TERRAINCUBE;

	typedef struct tagTerrainWall
	{
		D3DXVECTOR3		vSize;
		D3DXVECTOR3		vBasePos;
		unsigned int	iDrawID;
		D3DXVECTOR3		vAxis;
		float			fAngle;
	}TERRAINWALL;

	typedef struct tagWall
	{
		enum ROLE { ROLE_BACK, ROLE_FRONT, ROLE_LEFT, ROLE_RIGHT, ROLE_TERRAIN, ROLE_END };

		ROLE			eRole;
		D3DXVECTOR3		vSize;
		D3DXVECTOR3		vBasePos;
		unsigned int	iDrawID;
	}WALL;


	enum CHANNELID { SOUND_BGM, SOUND_PLAYER, SOUND_NPC, SOUND_BOSS, SOUND_MONSTER, SOUND_EFFECT, SOUND_UI, MAXCHANNEL }; // 05/07 ¿”»Òº∑ √ﬂøÏ∞°
}

