//=============================================================================
//
// 
// Author : GP11A_341 麦英泳 
//
//=============================================================================
#include "seizaEffect.h"
#include "debugproc.h"


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
#define	TEXTURE_ARI			("data/TEXTURE/ari00.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_TAU			("data/TEXTURE/tau00.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_GEM			("data/TEXTURE/gem000.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_CAN			("data/TEXTURE/can00.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_LEO			("data/TEXTURE/leo00.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_VIR			("data/TEXTURE/vir00.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_LIB			("data/TEXTURE/lib000.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_SCO			("data/TEXTURE/sco00.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_SAG			("data/TEXTURE/sag00.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_CAP			("data/TEXTURE/cap00.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_AQU			("data/TEXTURE/aqu000.png")	// 読み込むテクスチャファイル名
#define	TEXTURE_PIS			("data/TEXTURE/pis00.png")	// 読み込むテクスチャファイル名


#define	SEIZAEFFECT_SIZE_X		(83.0f / 4)						// ビルボードの幅
#define	SEIZAEFFECT_SIZE_Y		(63.0f / 3)						// ビルボードの高さ
#define SEIZAEFFECT_MAX			(12)						//エフェクトの番号


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSeizaEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexSeizaEffect(int Index, float fSizeX, float fSizeY);
void SetDiffuseSeizaEffect(int Index, D3DCOLOR val);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

LPDIRECT3DTEXTURE9			D3DTextureSeizaEffect[SEIZAEFFECT_MAX];	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		D3DVtxBuffSeizaEffect = NULL;	// 頂点バッファインターフェースへのポインタ

SEIZAEFFECT					seizaEffectWk[SEIZAEFFECT_MAX];	// SEIZAEFFECT構造体用変数

D3DXVECTOR3 offsetPos[SEIZAEFFECT_MAX] =
{
	D3DXVECTOR3(5.4f, 2.3f, 0.0f),		// 0 牡羊座
	D3DXVECTOR3(9.0f, -8.0f, 0.0f),		// 1 牡牛座
	D3DXVECTOR3(3.0f, -10.0f, 0.0f),	// 2 双子座
	D3DXVECTOR3(-4.3f, 12.0f, 0.0f),	// 3 蟹座
	D3DXVECTOR3(-11.0f, -7.0f, 0.0f),	// 4 獅子座
	D3DXVECTOR3(9.0f, -1.9f, 0.0f),		// 5 乙女座
	D3DXVECTOR3(-6.8f, 16.0f, 0.0f),	// 6 天秤座
	D3DXVECTOR3(-0.5f, -1.2f, 0.0f),	// 7 蠍座
	D3DXVECTOR3(3.8f, 13.0f, 0.0f),		// 8 射手座
	D3DXVECTOR3(10.0f, -3.0f, 0.0f),	// 9 山羊座
	D3DXVECTOR3(5.0f, -5.0f, 0.0f),		//10 水瓶座
	D3DXVECTOR3(9.0f, 10.8f, 0.0f),		//11 魚座	
};

D3DXVECTOR3 offsetScl[SEIZAEFFECT_MAX] =
{
	D3DXVECTOR3(14.5f * ARI_DISTANCE * 3.2f, 14.5f * ARI_DISTANCE * 3.2f, 1.0f),	// 0 牡羊座
	D3DXVECTOR3(14.5f * TAU_DISTANCE * 3.2f, 14.5f * TAU_DISTANCE * 5.5f, 1.0f),	// 1 牡牛座
	D3DXVECTOR3(14.5f * GEM_DISTANCE * 3.2f, 14.5f * GEM_DISTANCE * 3.2f, 1.0f),	// 2 双子座
	D3DXVECTOR3(14.5f * CAN_DISTANCE * 5.7f, 14.5f * CAN_DISTANCE * 8.5f, 1.0f),	// 3 蟹座
	D3DXVECTOR3(14.5f * LEO_DISTANCE * 2.1f, 14.5f * LEO_DISTANCE * 3.15f, 1.0f),	// 4 獅子座
	D3DXVECTOR3(14.5f * VIR_DISTANCE * 2.5f, 14.5f * VIR_DISTANCE * 5.0f, 1.0f),	// 5 乙女座
	D3DXVECTOR3(14.5f * LIB_DISTANCE * 1.7f, 14.5f * LIB_DISTANCE * 2.5f, 1.0f),	// 6 天秤座
	D3DXVECTOR3(14.5f * SCO_DISTANCE * 2.3f, 14.5f * SCO_DISTANCE * 3.5f, 1.0f),	// 7 蠍座
	D3DXVECTOR3(14.5f * SAG_DISTANCE * 5.3f, 14.5f * SAG_DISTANCE * 6.5f, 1.0f),	// 8 射手座
	D3DXVECTOR3(14.5f * CAP_DISTANCE * 2.3f, 14.5f * CAP_DISTANCE * 3.5f, 1.0f),	// 9 山羊座
	D3DXVECTOR3(14.5f * AQU_DISTANCE * 3.3f, 14.5f * AQU_DISTANCE * 4.5f, 1.0f),	//10 水瓶座
	D3DXVECTOR3(14.5f * PIS_DISTANCE * 5.0f, 14.5f * PIS_DISTANCE * 8.5f, 1.0f),	//11 魚座	
}; 

static D3DXVECTOR3 pos[12];
static D3DXVECTOR3 scl[12];

static D3DXVECTOR3 posCom[12];
static D3DXVECTOR3 sclCom[12];

int randam[12];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSeizaEffect(int type)
{
	
	
	posCom[0] = D3DXVECTOR3(-29.1f, 23.7f, 0.0f);
	posCom[1] = D3DXVECTOR3(-7.5f, 23.7f, 0.0f);
	posCom[2] = D3DXVECTOR3( 12.5f, 23.7f, 0.0f);
	posCom[3] = D3DXVECTOR3( 28.6f, 23.7f, 0.0f);

	posCom[4] = D3DXVECTOR3(-29.1f, 2.5f, 0.0f);
	posCom[5] =	D3DXVECTOR3(-7.5f, 2.5f, 0.0f);
	posCom[6] =	D3DXVECTOR3(12.5f, 2.5f, 0.0f);
	posCom[7] =	D3DXVECTOR3(28.6f, 2.5f, 0.0f);

	posCom[8] =	D3DXVECTOR3(-29.1f, -16.5f, 0.0f);
	posCom[9] =	D3DXVECTOR3(-7.5f, -16.5f, 0.0f);
	posCom[10] =	D3DXVECTOR3(12.5f, -16.5f, 0.0f);
	posCom[11] =	D3DXVECTOR3(28.6f, -16.5f, 0.0f);
	
	


	for (int i = 0; i < 12; i++)
	{
		pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		scl[i] = D3DXVECTOR3(1.3f, 1.4f, 1.0f);
		randam[i] = -1;
	}


	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SEIZAEFFECT *seizaEffect = &seizaEffectWk[0];

	// 頂点情報の作成
	MakeVertexSeizaEffect(pDevice);

	if (type == 0)
	{
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_ARI,					// ファイルの名前
			&D3DTextureSeizaEffect[ARI]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_TAU,					// ファイルの名前
			&D3DTextureSeizaEffect[TAU]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_GEM,					// ファイルの名前
			&D3DTextureSeizaEffect[GEM]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_CAN,					// ファイルの名前
			&D3DTextureSeizaEffect[CAN]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_LEO,					// ファイルの名前
			&D3DTextureSeizaEffect[LEO]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_VIR,					// ファイルの名前
			&D3DTextureSeizaEffect[VIR]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_LIB,					// ファイルの名前
			&D3DTextureSeizaEffect[LIB]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_SCO,					// ファイルの名前
			&D3DTextureSeizaEffect[SCO]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_SAG,					// ファイルの名前
			&D3DTextureSeizaEffect[SAG]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_CAP,					// ファイルの名前
			&D3DTextureSeizaEffect[CAP]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_AQU,					// ファイルの名前
			&D3DTextureSeizaEffect[AQU]);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_PIS,					// ファイルの名前
			&D3DTextureSeizaEffect[PIS]);				// 読み込むメモリー

	}

	for (int i = 0; i < SEIZAEFFECT_MAX; i++, seizaEffect++)
	{
		int val;
		for (int j = 0; j < 12; j++)
		{
			if (randam[j] == -1)
			{
				while (1)
				{
					val = rand() % 12;
					if (randam[val] == -1)
					{
						seizaEffectWk[i].pos = posCom[val];
						randam[val] = i;
						break;
					}
				}
				break;
			}
		}

		seizaEffect->use = false;
		//seizaEffect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		seizaEffect->scl = D3DXVECTOR3(1.3f, 1.4f, 1.0f);
		seizaEffect->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		seizaEffect->delta = 0.003f;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSeizaEffect(void)
{
	for (int j = 0; j < SEIZAEFFECT_MAX; j++)
	{
		if (D3DTextureSeizaEffect[j] != NULL)
		{// テクスチャの開放
			D3DTextureSeizaEffect[j]->Release();
			D3DTextureSeizaEffect[j] = NULL;
		}
	}


	if (D3DVtxBuffSeizaEffect != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffSeizaEffect->Release();
		D3DVtxBuffSeizaEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSeizaEffect(void)
{
	SEIZA** seiza = GetSeiza();

	static int index = 0;

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_0))
	{
		index = 0;
	}
	if (GetKeyboardPress(DIK_1))
	{
		index = 1;
	}
	if (GetKeyboardPress(DIK_2))
	{
		index = 2;
	}
	if (GetKeyboardPress(DIK_3))
	{
		index = 3;
	}
	if (GetKeyboardPress(DIK_4))
	{
		index = 4;
	}
	if (GetKeyboardPress(DIK_5))
	{
		index = 5;
	}
	if (GetKeyboardPress(DIK_6))
	{
		index = 6;
	}
	if (GetKeyboardPress(DIK_7))
	{
		index = 7;
	}
	if (GetKeyboardPress(DIK_8))
	{
		index = 8;
	}
	if (GetKeyboardPress(DIK_9))
	{
		index = 9;
	}
	if (GetKeyboardPress(DIK_MINUS))
	{
		index = 10;
	}
	if (GetKeyboardPress(DIK_YEN))
	{
		index = 11;
	}



	if (GetKeyboardPress(DIK_T))
	{
		seizaEffectWk[index].pos.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_G))
	{
		seizaEffectWk[index].pos.y -= 0.1f;
	}
	if (GetKeyboardPress(DIK_F))
	{
		seizaEffectWk[index].pos.x -= 0.1f;
	}
	if (GetKeyboardPress(DIK_H))
	{
		seizaEffectWk[index].pos.x += 0.1f;
	}

	for (int i = 0;i < 12;i++)
	{
		if (GetKeyboardPress(DIK_I))
		{
			scl[i].y += 0.01f;
		}
		if (GetKeyboardPress(DIK_K))
		{
			scl[i].y -= 0.01f;
		}
		if (GetKeyboardPress(DIK_J))
		{
			scl[i].x -= 0.01f;
		}
		if (GetKeyboardPress(DIK_L))
		{
			scl[i].x += 0.01f;
		}
	}
#endif

	for (int i = 0; i < SEIZAEFFECT_MAX; i++, seiza++)
	{
		//if (i == 1)
		//{
			//(*seiza)->isConnect = true;
		//}

		//int val;

		if ((*seiza)->isConnect)
		{
			//if (!seizaEffectWk[i].use)
			//{
			//	for (int j = 0; j < 12; j++)
			//	{
			//		if (randam[j] == -1)
			//		{
			//			while (1)
			//			{
			//				val = rand() % 12;
			//				if (randam[val] == -1)
			//				{
			//					seizaEffectWk[i].pos = posCom[val];
			//					randam[val] = i;
			//					break;
			//				}
			//			}
			//			break;
			//		}
			//	}
			//}
			SetSeizaEffect((*seiza)->name, seizaEffectWk[i].pos, scl[i]);

			//SetSeizaEffect((*seiza)->name, pos[i], scl[i]);
		
			//SetSeizaEffect((*seiza)->name, (*seiza)->star->pos + offsetPos[i], offsetScl[i]);
			
		}
	}

	PrintDebugProc("\n %d Effect: pos(%f,%f)  scl(%f,%f)\n", index, seizaEffectWk[index].pos.x, seizaEffectWk[index].pos.y, scl[index].x, scl[index].y);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSeizaEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;
	SEIZAEFFECT *seizaEffect = &seizaEffectWk[0];

	for (int i = 0; i < SEIZAEFFECT_MAX; i++, seizaEffect++)
	{
		if (seizaEffect->use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&seizaEffect->mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, seizaEffect->scl.x,
				seizaEffect->scl.y,
				seizaEffect->scl.z);

			D3DXMatrixMultiply(&seizaEffect->mtxWorld,&seizaEffect->mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, seizaEffect->pos.x,
				seizaEffect->pos.y,
				seizaEffect->pos.z);

			D3DXMatrixMultiply(&seizaEffect->mtxWorld,&seizaEffect->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &seizaEffect->mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, D3DVtxBuffSeizaEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTextureSeizaEffect[i]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
		}
	}

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexSeizaEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SEIZAEFFECT_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffSeizaEffect,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffSeizaEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < SEIZAEFFECT_MAX; i++, pVtx += NUM_VERTEX)
		{
			// 頂点座標の設定
			SetVertexSeizaEffect(i, SEIZAEFFECT_SIZE_X, SEIZAEFFECT_SIZE_Y);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}
		// 頂点データをアンロックする
		D3DVtxBuffSeizaEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexSeizaEffect(int Index, float fSizeX, float fSizeY)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffSeizaEffect->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (Index * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

	// 頂点データをアンロックする
	D3DVtxBuffSeizaEffect->Unlock();
	
}

//============================================================================
// 
//============================================================================
void SetDiffuseSeizaEffect(int Index, D3DCOLOR val)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffSeizaEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// 反射光の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = val;

		// 頂点データをアンロックする
		D3DVtxBuffSeizaEffect->Unlock();
	}
}

//==========================================================================

//==========================================================================
void SetSeizaEffect(ZODIAC seiza,D3DXVECTOR3 pos, D3DXVECTOR3 scl)
{
	seizaEffectWk[seiza].use = true;
	seizaEffectWk[seiza].pos = pos;
	seizaEffectWk[seiza].scl = scl;

	seizaEffectWk[seiza].color.a += seizaEffectWk[seiza].delta;

	if (seizaEffectWk[seiza].color.a > 0.8f)
	{
		seizaEffectWk[seiza].delta = -0.001f;

		//seizaEffectWk[seiza].color.a = 0.8f;
	}

	if (seizaEffectWk[seiza].color.a < 0.2f)
	{
		seizaEffectWk[seiza].color.a = 0.2f;//0.2
	}

	//頂点に反映する
	SetDiffuseSeizaEffect(seiza, seizaEffectWk[seiza].color);

}

void DelSeizaEffect(int index)
{

	if ((index >= 0) && (index < SEIZAEFFECT_MAX))
	{

		seizaEffectWk[index].use = false;
	}

}

SEIZAEFFECT* GetSeizaEffect(void)
{
	return seizaEffectWk;
}

