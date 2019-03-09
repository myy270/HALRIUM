//=============================================================================
//
// エフェクト処理 [starEffect.cpp]
// Author : GP11A_341_22_田中太陽 
//
//=============================================================================
#include "starEffect.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_STAREFFECT		"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名

#define	STAREFFECT_SIZE_X		(4.0f)						// ビルボードの幅
#define	STAREFFECT_SIZE_Y		(4.0f)						// ビルボードの高さ
#define STAREFFECT_MAX			(240)							//エフェクトの番号


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexStarEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexStarEffect(int Index, float fSizeX, float fSizeY);
void SetDiffuseStarEffect(int Index, D3DCOLOR val);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

LPDIRECT3DTEXTURE9			D3DTextureStarEffect;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		D3DVtxBuffStarEffect = NULL;	// 頂点バッファインターフェースへのポインタ

STAREFFECT					starEffectWk[STAREFFECT_MAX];	// STAREFFECT構造体用変数
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitStarEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	STAREFFECT *starEffect = &starEffectWk[0];

	// 頂点情報の作成
	MakeVertexStarEffect(pDevice);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			TEXTURE_STAREFFECT,					// ファイルの名前
			&D3DTextureStarEffect);			// 読み込むメモリー
	}

	for (int i = 0; i < STAREFFECT_MAX; i++, starEffect++)
	{
		starEffect->use = false;
		starEffect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		starEffect->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		starEffect->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStarEffect(void)
{
	if (D3DTextureStarEffect != NULL)
	{// テクスチャの開放
		D3DTextureStarEffect->Release();
		D3DTextureStarEffect = NULL;
	}


	if (D3DVtxBuffStarEffect != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffStarEffect->Release();
		D3DVtxBuffStarEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStarEffect(void)
{


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStarEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	STAREFFECT *starEffect = &starEffectWk[0];

	
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	for (int i = 0; i < STAREFFECT_MAX; i++, starEffect++)
	{
		if (starEffect->use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&starEffect->mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, starEffect->scl.x,
				starEffect->scl.y,
				starEffect->scl.z);
			D3DXMatrixMultiply(&starEffect->mtxWorld,
				&starEffect->mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, starEffect->pos.x,
				starEffect->pos.y,
				starEffect->pos.z);
			D3DXMatrixMultiply(&starEffect->mtxWorld,
				&starEffect->mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &starEffect->mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, D3DVtxBuffStarEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTextureStarEffect);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
		}
	}

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexStarEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * STAREFFECT_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffStarEffect,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffStarEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < STAREFFECT_MAX; i++, pVtx += NUM_VERTEX)
		{
			// 頂点座標の設定
			SetVertexStarEffect(i, STAREFFECT_SIZE_X, STAREFFECT_SIZE_Y);

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
		D3DVtxBuffStarEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexStarEffect(int Index, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffStarEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffStarEffect->Unlock();
	}
}

//============================================================================
// 煙エフェクトの透過処理
//============================================================================
void SetDiffuseStarEffect(int Index, D3DCOLOR val)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffStarEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// 反射光の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = val;

		// 頂点データをアンロックする
		D3DVtxBuffStarEffect->Unlock();
	}
}

//==========================================================================
// 煙エフェクトの設置
// 引　数：index プレイヤーの番号
// 戻り値：　エフェクトのインデクス番号
//==========================================================================
int SetStarEffect(D3DXVECTOR3 pos, int indexPlayer)
{
	STAREFFECT *starEffect = &starEffectWk[0];

	for (int i = 0; i < STAREFFECT_MAX; i++, starEffect++)
	{
		if (!starEffect->use)
		{
			starEffect->use = true;	// 使用中
			starEffect->pos = pos;

			indexPlayer == 0 ? starEffect->color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f) :
				starEffect->color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.4f);

			SetDiffuseStarEffect(i, starEffect->color);

			return i;
		}
	}

	return -1;
}

void DelStarEffect(int index)
{

	if ((index >= 0) && (index < STAREFFECT_MAX))
	{

		starEffectWk[index].use = false;
	}

}

