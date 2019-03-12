//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 麦英泳
//
//=============================================================================
#include "game.h"
#include "light.h"
#include "basicScene.h"
#include "particleStar.h"
#include "mouse.h"
#include "timer.h"
#include "sound.h"

#include "starEffect.h"
#include "stack.h"
#include "seizaEffect.h"
#include "score.h"
#include "score_effect.h"

#include "Aries.h"
#include "Taurus.h"
#include "Gemini.h"
#include "Cancer.h"
#include "Leo.h"
#include "Virgo.h"
#include "Libra.h"
#include "Scorpio.h"
#include "Sagittarius.h"
#include "Capricorn.h"
#include "Aquarius.h"
#include "Pisces.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	// タイマーの初期化
	InitTimer();
	ResetTimer(999);

	{
		InitAri();
		InitTau();
		InitGem();
		InitCan();
		InitLeo();
		InitVir();
		InitLib();
		InitSco();
		InitSag();
		InitCap();
		InitAqu();
		InitPis();
	}

	InitMouse();	// マウスの初期化は必ず星座の後

	InitStarEffect(0);

	InitSeizaEffect(0);//星座の後

	InitStack(0);

	InitScore();

	InitBezier_Particle();


	// BGM再生 ちゃんとloopできるにする!
	PlaySound(SOUND_LABEL_BGM001, XAUDIO2_LOOP_INFINITE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// タイマーの終了処理
	UninitTimer();

	UninitBasicScene();

	UninitParticleStar();

	UninitMouse2();

	{
		UninitAri();
		UninitTau();
		UninitGem();
		UninitCan();
		UninitLeo();
		UninitVir();
		UninitLib();
		UninitSco();
		UninitSag();
		UninitCap();
		UninitAqu();
		UninitPis();
	}

	UninitStarEffect();
	UninitSeizaEffect();
	UninitStack();
	UninitScore();
	UninitBezier_Particle();

	// BGM停止
	StopSound(SOUND_LABEL_BGM001);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	UpdateBasicScene();
	UpdateParticleStar();

	UpdateMouse();

	{
		UpdateAri();
		UpdateTau();
		UpdateGem();
		UpdateCan();
		UpdateLeo();
		UpdateVir();
		UpdateLib();
		UpdateSco();
		UpdateSag();
		UpdateCap();
		UpdateAqu();
		UpdatePis();
	}

	UpdateStarEffect();
	UpdateSeizaEffect();//星座の後ろに置く

	UpdateStack();//星座の後ろに置く

	UpdateScore();

	UpdateBezier_Particle();

	// タイマー処理の更新
	UpdateTimer();

}

//=============================================================================
// 描画処理 描画順注意
//=============================================================================
void DrawGame(void)
{
	DrawBasicScene();//z 80
	DrawParticleStar();//z 1~50

	DrawSeizaEffect();//z 0

	{
		DrawAri();
		DrawTau();
		DrawGem();
		DrawCan();
		DrawLeo();
		DrawVir();
		DrawLib();
		DrawSco();
		DrawSag();
		DrawCap();
		DrawAqu();
		DrawPis();
	}

	DrawStarEffect();//星座の後に置く

	DrawMouse();

	DrawScore();//2D描画
	DrawTimer();//2D描画
	DrawStack();//2D描画


	DrawBezier_Particle();
}

