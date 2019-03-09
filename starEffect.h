//=============================================================================
//
// バレットエフェクト処理 [smoke.h]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#ifndef _STAREFFECT_H_
#define _STAREFFECT_H_

#include "main.h"

typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 行列のT
	D3DXVECTOR3 scl;		// 行列のS
	bool		use;		// 使用しているかどうか
	D3DXCOLOR	color;

} STAREFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStarEffect(int type);
void UninitStarEffect(void);
void UpdateStarEffect(void);
void DrawStarEffect(void);
int SetStarEffect(D3DXVECTOR3 pos, int indexPlayer);

void DelStarEffect(int index);
#endif
