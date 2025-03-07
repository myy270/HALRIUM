#ifndef _LIBRA_H_
#define _LIBRA_H_

#include "main.h"
#include "seiza.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LIB_DISTANCE		(1.4f)						// 星距離の単位

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLib();
void UninitLib();
void UpdateLib();
void DrawLib();

SEIZA* GetLib();	// 星座のアドレスを取得

#endif
