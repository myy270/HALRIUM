#ifndef _CAN_H_
#define _CAN_H_

#include "main.h"
#include "seiza.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAN_DISTANCE		(0.37f)						// 星距離の単位	

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCan();
void UninitCan();
void UpdateCan();
void DrawCan();

SEIZA* GetCan();	// 星座のアドレスを取得

void UpdateSeiza2(SEIZA seiza, int Score);
#endif
