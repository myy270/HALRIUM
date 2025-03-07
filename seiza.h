//=============================================================================
//
// 星座クラス [seiza.h]
// Author : HEW チームE
// 
//=============================================================================
#ifndef _SEIZA_H_
#define _SEIZA_H_

#include "main.h"
#include "sound.h"
#include "score.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SEIZA		(12)		// 星座数
#define SIZE_STAR		(3.5f)		// 星のサイズ
#define SIZE_LINE		(1.0f)		// 線分のサイズ

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 列挙体定義
enum ZODIAC {
	ARI,			// 牡羊座
	TAU,			// 牡牛座
	GEM,			// 双子座
	CAN,			// 蟹座
	LEO,			// 獅子座
	VIR,			// 乙女座
	LIB,			// 天秤座
	SCO,			// 蠍座
	SAG,			// 射手座
	CAP,			// 山羊座
	AQU,			// 水瓶座
	PIS,			// 魚座	

	PLAYER_ONE,		// P1に所属
	PLAYER_TWO,		// P2に所属
	DEFAULT,		// 何も選択していない、マウス初期化の時使用
};

// 星の構造体定義
typedef struct _star{
	D3DXVECTOR3		pos;
	D3DXVECTOR3		scale;
	D3DXCOLOR		color;
	float			fade;
	float			radius;
	float			rot;
	bool			isLight;
	int				effectIdx;		//starEffectの番号、後で消せるためにこの番号を記録する
}STAR;

typedef STAR LINE;	// 星と線分が同じ構造体

// 星座の構造体定義
typedef struct _seiza{
	ZODIAC	name;		// 星座の名前
	ZODIAC	belong;		// 所属
	int		Nv;			// 星の数
	int		Ne;			// 線分数
	int*	G;			// 星と星の関係を反映する配列
	STAR*	star;		// 星の配列
	LINE*	line;		// 線分の配列
	bool	isConnect;	// 星が全部繋がっているかどうかS
}SEIZA;

// 色の定義
const D3DXCOLOR WHITE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR HALF_WHITE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
const D3DXCOLOR PINK = D3DXCOLOR(1.0f, 0.6f, 1.0f, 1.0f);
const D3DXCOLOR RED = D3DXCOLOR(1.0f, 0.4f, 1.0f, 0.8f);//p1カーソル

const D3DXCOLOR CYAN = D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLUE = D3DXCOLOR(0.3f, 1.0f, 1.0f, 0.8f);//p2カーソル

//*****************************************************************************
// 星座に関する関数
//*****************************************************************************

void InitSeiZa(SEIZA* pSeiza, int numStar, int numEdge); // 星座構造体の初期化
void SetLine(SEIZA* pSeiza, int v1, int v2); // 線分を設定する

void UpdateSeiza(SEIZA* seiza,int Score);
//*****************************************************************************
// 補助関数
//*****************************************************************************
float GetRandomFloat(float lowBound, float highBound);
void  GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max);
float GetDistance(D3DXVECTOR3* pV1, D3DXVECTOR3* pV2);

#include "seizaEffect.h"

#endif