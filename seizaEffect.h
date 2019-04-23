//=============================================================================
//
// 
// Author : GP11A_341 麦英泳 
//
//=============================================================================
#ifndef _SEIZAEFFECT_H_
#define _SEIZAEFFECT_H_

#include "main.h"
#include "seiza.h"
#include "mouse.h"

typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 行列のT
	D3DXVECTOR3 scl;		// 行列のS
	bool		use;		// 使用しているかどうか
	D3DXCOLOR	color;
	float delta;			//アルファの変化値
} SEIZAEFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSeizaEffect(int type);
void UninitSeizaEffect(void);
void UpdateSeizaEffect(void);
void DrawSeizaEffect(void);

void SetSeizaEffect(ZODIAC seiza, D3DXVECTOR3 pos, D3DXVECTOR3 scl);

void DelSeizaEffect(int index);

SEIZAEFFECT* GetSeizaEffect(void);
#endif
