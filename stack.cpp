//=============================================================================
//
// 
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "stack.h"
#include "mouse.h"

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
#define	TEXTURE_FRAME		("data/TEXTURE/stack_frame.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_BG01		("data/TEXTURE/stack_bg01.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_BG02		("data/TEXTURE/stack_bg02.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_ARI			("data/TEXTURE/ari.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_TAU			("data/TEXTURE/tau.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_GEM			("data/TEXTURE/gem.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_CAN			("data/TEXTURE/can.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_LEO			("data/TEXTURE/leo.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_VIR			("data/TEXTURE/vir.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_LIB			("data/TEXTURE/lib.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_SCO			("data/TEXTURE/sco.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_SAG			("data/TEXTURE/sag.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_CAP			("data/TEXTURE/cap.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_AQU			("data/TEXTURE/aqu.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_PIS			("data/TEXTURE/pis.png")	// 読み込むテクスチャファイル名

#define STACK_MAX			(12)
#define BG_TEXTURE_MAX		(2)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexStack(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9				D3DTextureFrame = NULL;							// テクスチャへのポインタ
LPDIRECT3DTEXTURE9				D3DTextureBg[BG_TEXTURE_MAX] = { NULL,NULL };	// テクスチャへのポインタ
LPDIRECT3DTEXTURE9				D3DTextureZodiac[STACK_MAX];					// テクスチャへのポインタ

VERTEX_2D						VertexWkFrame[STACK_MAX][NUM_VERTEX];	// 頂点情報格納ワーク
VERTEX_2D						VertexWkBg[STACK_MAX][NUM_VERTEX];		// 頂点情報格納ワーク
VERTEX_2D						VertexWkZodiac[STACK_MAX][NUM_VERTEX];	// 頂点情報格納ワーク


STACK			StackWk[STACK_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitStack(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	STACK *stack = &StackWk[0];


	for (int i = 0; i < STACK_MAX; i++, stack++)
	{
		stack->use = false;		// スタックのBGの使用状態
		stack->texNum = 0;	// スタックBGのテクスチャ番号
	}

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_FRAME,					// ファイルの名前
			&D3DTextureFrame);				// 読み込むメモリー
				// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_BG01,					// ファイルの名前
			&D3DTextureBg[0]);				// 読み込むメモリー
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_BG02,					// ファイルの名前
			&D3DTextureBg[1]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_ARI,					// ファイルの名前
			&D3DTextureZodiac[ARI]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_TAU,					// ファイルの名前
			&D3DTextureZodiac[TAU]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_GEM,					// ファイルの名前
			&D3DTextureZodiac[GEM]);				// 読み込むメモリー
		
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_CAN,					// ファイルの名前
			&D3DTextureZodiac[CAN]);				// 読み込むメモリー
		
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_LEO,					// ファイルの名前
			&D3DTextureZodiac[LEO]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_VIR,					// ファイルの名前
			&D3DTextureZodiac[VIR]);				// 読み込むメモリー
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_LIB,					// ファイルの名前
			&D3DTextureZodiac[LIB]);				// 読み込むメモリー
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_SCO,					// ファイルの名前
			&D3DTextureZodiac[SCO]);				// 読み込むメモリー
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_SAG,					// ファイルの名前
			&D3DTextureZodiac[SAG]);				// 読み込むメモリー
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_CAP,					// ファイルの名前
			&D3DTextureZodiac[CAP]);				// 読み込むメモリー
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_AQU,					// ファイルの名前
			&D3DTextureZodiac[AQU]);				// 読み込むメモリー
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_PIS,					// ファイルの名前
			&D3DTextureZodiac[PIS]);				// 読み込むメモリー

	}
	
	// 頂点情報の作成
	MakeVertexStack();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStack(void)
{
	if(D3DTextureFrame != NULL)
	{// テクスチャの開放
		D3DTextureFrame->Release();
		D3DTextureFrame = NULL;
	}

	for (int i = 0; i < BG_TEXTURE_MAX; i++)
	{
		if (D3DTextureBg[i] != NULL)
		{// テクスチャの開放
			D3DTextureBg[i]->Release();
			D3DTextureBg[i] = NULL;
		}
	}

	for (int j = 0; j < STACK_MAX; j++)
	{
		if (D3DTextureBg[j] != NULL)
		{// テクスチャの開放
			D3DTextureBg[j]->Release();
			D3DTextureBg[j] = NULL;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStack(void)
{
	SEIZA** seiza = GetSeiza();
	
	for (int i = 0; i < MAX_SEIZA; i++, seiza++)
	{
		if ((*seiza)->isConnect)
		{
			StackWk[i].use = true;
			if ((*seiza)->belong == PLAYER_ONE)
			{
				StackWk[i].texNum = 0;
			}
			else if ((*seiza)->belong == PLAYER_TWO)
			{
				StackWk[i].texNum = 1;
			}
		}
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	STACK *stack = &StackWk[0];

	for (int i = 0; i < STACK_MAX; i++, stack++)
	{
		//枠の背景
		if (stack->use)
		{
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTextureBg[stack->texNum]);//色

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkBg[i], sizeof(VERTEX_2D));
		}
	
		//枠の星座図案
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureZodiac[i]);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkZodiac[i], sizeof(VERTEX_2D));


		//枠
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, D3DTextureFrame);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkFrame[i], sizeof(VERTEX_2D));
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexStack(void)
{
	for(int i = 0; i < STACK_MAX; i++)
	{
		// 枠
		{
			// 頂点座標の設定
			VertexWkFrame[i][0].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkFrame[i][1].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkFrame[i][2].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);
			VertexWkFrame[i][3].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);

			// テクスチャのパースペクティブコレクト用
			VertexWkFrame[i][0].rhw =
				VertexWkFrame[i][1].rhw =
				VertexWkFrame[i][2].rhw =
				VertexWkFrame[i][3].rhw = 1.0f;

			// 反射光の設定
			VertexWkFrame[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 155);
			VertexWkFrame[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 155);
			VertexWkFrame[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 155);
			VertexWkFrame[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 155);

			// テクスチャ座標の設定
			VertexWkFrame[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
			VertexWkFrame[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
			VertexWkFrame[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
			VertexWkFrame[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 星座
		{
			// 頂点座標の設定
			VertexWkZodiac[i][0].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkZodiac[i][1].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkZodiac[i][2].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);
			VertexWkZodiac[i][3].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);

			// テクスチャのパースペクティブコレクト用
			VertexWkZodiac[i][0].rhw =
				VertexWkZodiac[i][1].rhw =
				VertexWkZodiac[i][2].rhw =
				VertexWkZodiac[i][3].rhw = 1.0f;

			// 反射光の設定
			VertexWkZodiac[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 205);
			VertexWkZodiac[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 205);
			VertexWkZodiac[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 205);
			VertexWkZodiac[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 205);

			// テクスチャ座標の設定
			VertexWkZodiac[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
			VertexWkZodiac[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
			VertexWkZodiac[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
			VertexWkZodiac[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// BG
		{
			// 頂点座標の設定
			VertexWkBg[i][0].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkBg[i][1].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkBg[i][2].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);
			VertexWkBg[i][3].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);

			// テクスチャのパースペクティブコレクト用
			VertexWkBg[i][0].rhw =
				VertexWkBg[i][1].rhw =
				VertexWkBg[i][2].rhw =
				VertexWkBg[i][3].rhw = 1.0f;

			// 反射光の設定
			VertexWkBg[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 95);
			VertexWkBg[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 95);
			VertexWkBg[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 95);
			VertexWkBg[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 95);

			// テクスチャ座標の設定
			VertexWkBg[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
			VertexWkBg[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
			VertexWkBg[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
			VertexWkBg[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}


	return S_OK;
}
