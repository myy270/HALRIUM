//=============================================================================
//
// スコアエフェクト処理 [score_effect.h]
// Author : 増田　光汰
//
//=============================================================================
#ifndef _BEZIER_PARTICLEH_
#define _BEZIER_PARTICLEH_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR	col;		// 色
	float		fSizeX;		// 幅
	float		fSizeY;		// 高さ
	int			nIdxShadow;	// 影ID
	int			nLife;		// 寿命
	bool		bUse;		// 使用しているかどうか
	int			frame;		// 到達フレーム
	int			cntFrame;	// カウントフレーム
	D3DXVECTOR3 start;		//				スタート位置
	D3DXVECTOR3 control1;	// ベジェ曲線用 軌道1
	D3DXVECTOR3 control2;	//				軌道2
	int			cntclick;	// カウントフレーム

} BEZIER_PARTICLEH;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBezier_Particle(void);				// 初期化処理
void UninitBezier_Particle(void);				// 終了処理
void UpdateBezier_Particle(void);				// 更新処理
void DrawBezier_Particle(void);					// 描画処理
void SetBezier_Particle(D3DXVECTOR3 pos);		// セット関数(出したい場所に設定)
BEZIER_PARTICLEH *GetBezier_Particle(void);		// 関数取得
#endif

#pragma once
