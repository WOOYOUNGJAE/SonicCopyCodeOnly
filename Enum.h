#pragma once

enum STAGE_ID
{
	STAGE_TEST,
	STAGE_END,
};

enum OBJ_ID
{
	ID_PLAYER,
	ID_MECHASONIC,
	ID_WALL,
	ID_SPRING,
	ID_DIAGONALSPRING,
	ID_SPIKE,
	ID_RING,
	ID_ITEM,
	ID_OTHER,
	OBJ_ID_END,
};

enum MY_DIR
{
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
	DIR_END,
};

enum DIAGONAL_DIR
{
	DIR_RU,
	DIR_RD,
	DIR_LD,
	DIR_LU,
	DIAGONAL_DIR_END,
};

enum CHANNELID { SOUND_EFFECT, SOUND_EFFECT1, SOUND_BGM, SOUND_PLAYER, SOUND_MONSTER, MAXCHANNEL };

enum RENDER_GROUP
{
	RENDER_BACKGROUND,
	RENDER_OBJECT0,
	RENDER_OBJECT1,
	RENDER_UI,
	RENDER_GROUP_END,
};