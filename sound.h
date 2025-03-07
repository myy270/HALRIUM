//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 麦英泳
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// サウンドファイル
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_SE_SELECT,		// 
	SOUND_LABEL_SE_COMPLETE,	// 
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label, int LoopCount = 0);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
