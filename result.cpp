//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : 麦英泳
//
//=============================================================================
#include "title.h"

#include "result.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "mouse.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT			"data/TEXTURE/bg001.jpg"		// 読み込むテクスチャファイル名

#define	TEXTURE_RESULT_LOGO		"data/TEXTURE/player01win.png"		// 勝つ テクスチャファイル名
#define	TEXTURE_RESULT_LOGO_2	"data/TEXTURE/player02win.png"	// 負ける テクスチャファイル名



#define	RESULT_LOGO_WIDTH	(286 * 1.4f * (SCREEN_WIDTH / 1280.0f))		// リザルトロゴの幅
#define	RESULT_LOGO_HEIGHT	(160 * 1.4f * (SCREEN_HEIGHT / 720.0f))		// リザルトロゴの高さ
#define	RESULT_LOGO_POS_X	((SCREEN_WIDTH - RESULT_LOGO_WIDTH) / 2)		// リザルトロゴの位置(X座標) 240
#define	RESULT_LOGO_POS_Y	(SCREEN_HEIGHT * 0.6f)		// リザルトロゴの位置(Y座標) 620

#define	COUNT_APPERA_RESULT	(60)		// リザルトロゴ出現までの待ち時間	
#define	LIMIT_COUNT_WAIT	(60 * 5)	// 待ち時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);
void SetColorResultLogo(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResult = NULL;		// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTextureResultLogo = NULL;		// 勝つ テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultLogo2 = NULL;	// 負ける テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultLogo = NULL;	// 頂点バッファインターフェースへのポインタ
int						g_nCountAppearResult = 0;		// 出現までの待ち時間
float					g_fAlphaResult = 0.0f;			// リザルトロゴのα値
int						g_nCountWaitResult = 0;			// 待ち時間

LPDIRECT3DTEXTURE9 g_textureBuff = NULL;//作業用

bool textureDecide;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	for (int i = 0; i < MAX_MOUSE; i++)
	{
		(GetMouse() + i)->use = false;
	}

	g_textureBuff = NULL;

	textureDecide = false;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearResult = 0;
	g_fAlphaResult = 0.0f;
	g_nCountWaitResult = 0;

	// 頂点情報の作成
	MakeVertexResult(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_RESULT,				// ファイルの名前
								&g_pD3DTextureResult);		// 読み込むメモリー

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_RESULT_LOGO,		// ファイルの名前
								&g_pD3DTextureResultLogo);	// 読み込むメモリー

		// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_RESULT_LOGO_2,		// ファイルの名前
								&g_pD3DTextureResultLogo2);	// 読み込むメモリー



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{


	if(g_pD3DTextureResult != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if(g_pD3DVtxBuffResult != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffResult->Release();
		g_pD3DVtxBuffResult = NULL;
	}

	if(g_pD3DTextureResultLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultLogo->Release();
		g_pD3DTextureResultLogo = NULL;
	}

	if(g_pD3DVtxBuffResultLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffResultLogo->Release();
		g_pD3DVtxBuffResultLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	

	g_nCountAppearResult++;
	if(g_nCountAppearResult >= COUNT_APPERA_RESULT)//60フレーム後に、浮き上がり始める
	{
		if(g_fAlphaResult < 1.0f)
		{
			g_fAlphaResult += 0.05f;//20フレーム完成
			if(g_fAlphaResult >= 0.9f)
			{
				g_fAlphaResult = 0.9f;
			}

			SetColorResultLogo();
		}
	}

	if(GetKeyboardTrigger(DIK_RETURN) || IsButtonTrigger(0, BUTTON_OPTIONS) || IsButtonTrigger(1, BUTTON_OPTIONS))
	{// Enter押したら、フェードアウトしてモードを切り替えいく
		SetFade(FADE_OUT);
	}

	g_nCountWaitResult++;
	if(g_nCountWaitResult > LIMIT_COUNT_WAIT)
	{
		//SetFade(FADE_OUT);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (!textureDecide)
	{

		if (GetWinner() == OBJECT_PLAYER)
		{
			// テクスチャの設定
			g_textureBuff = g_pD3DTextureResultLogo;
		}
		else
		{
			// テクスチャの設定
			g_textureBuff = g_pD3DTextureResultLogo2;
		}

		textureDecide = true;
	}

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResultLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_textureBuff);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffResult,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResult->Unlock();
	}

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffResultLogo,	// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_WIDTH, RESULT_LOGO_POS_Y + RESULT_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaResult);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffResultLogo->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorResultLogo(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		if (GetWinner() == OBJECT_PLAYER)
		{
			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.4f, 1.0f, g_fAlphaResult);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.4f, 1.0f, g_fAlphaResult);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.4f, 1.0f, g_fAlphaResult);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.4f, 1.0f, g_fAlphaResult);

		}
		else
		{
			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.3f, 1.0f, 1.0f, g_fAlphaResult);
			pVtx[1].diffuse = D3DXCOLOR(0.3f, 1.0f, 1.0f, g_fAlphaResult);
			pVtx[2].diffuse = D3DXCOLOR(0.3f, 1.0f, 1.0f, g_fAlphaResult);
			pVtx[3].diffuse = D3DXCOLOR(0.3f, 1.0f, 1.0f, g_fAlphaResult);

		}



		// 頂点データをアンロックする
		g_pD3DVtxBuffResultLogo->Unlock();
	}
}