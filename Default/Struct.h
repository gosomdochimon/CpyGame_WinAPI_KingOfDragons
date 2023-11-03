#pragma once

typedef struct tagInfo
{
	float	fX;
	float	fY;
	float	fCX;	// 가로 사이즈
	float	fCY;	// 세로 사이즈

}INFO;

typedef struct tagStat
{
	float fCurHp;
	float fMaxHp;
	float fCurMp;
	float fMaxMp;
	//tagStat(void) { ZeroMemory(this, sizeof(tagStat)); }
	//tagStat(float _Hp) : fCurHp(_Hp), fMaxHp(_Hp) {} 
}STAT;



typedef struct tagPoint
{
	float		fX;
	float		fY;

	tagPoint(void) { ZeroMemory(this, sizeof(tagPoint)); }
	tagPoint(float _fX, float _fY) : fX(_fX), fY(_fY){}

}LINEPOINT ;

typedef struct tagLine
{
	LINEPOINT		tLPoint;
	LINEPOINT		tRPoint;

	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(LINEPOINT& tLeft, LINEPOINT& tRight) : tLPoint(tLeft), tRPoint(tRight) {}

}LINE;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iMotion;

	DWORD	dwFrameSpeed;
	DWORD	dwFrameTime;

}FRAME;

typedef struct tagPhaseX {
	float fLeft;
	float fRight;
}PHASEX;

typedef struct tagIcon {
	INFO	tInfo;
	RECT	tRect;
	//SKILLID eSkill;

}ICON;