#include "particleStar.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FILENAME		"data/TEXTURE/star.dds"		// 星のテクスチャ
#define NUM_PSTAR			(256)
#define SIZE_PSTAR			(1.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	D3DXVECTOR3 pos;	// T行列
	D3DXCOLOR   col;
	float		fade;
}PSTAR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVtxPStar(LPDIRECT3DDEVICE9 pDevice);			// 頂点をつくる
void SetVtxPStar(LPDIRECT3DDEVICE9 pDevice);
void ResetPStar(PSTAR* pPStar);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffPStar;		// 頂点バッファ
LPDIRECT3DTEXTURE9		g_pD3DTexPStar;			// 星のテクスチャ
PSTAR					g_PStar[NUM_PSTAR];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticleStar()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 min = D3DXVECTOR3(-100.0f / 2, -100.0f / 2, 1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3( 100.0f / 2,  100.0f / 2,  50.0f);
	float alpha;

	for (int i = 0; i < NUM_PSTAR; i++)
	{
		GetRandomVector(&g_PStar[i].pos, &min, &max);//出現位置
		alpha = GetRandomFloat(0.2f, 0.8f);//アルファ初期値
		g_PStar[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
		g_PStar[i].fade = GetRandomFloat(-0.01f, 0.01f);//アルファ変化値
	}

	// 頂点をつくる
	MakeVtxPStar(pDevice);

	// load textures
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME, &g_pD3DTexPStar);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticleStar()
{
	if (g_pD3DVtxBuffPStar)
	{
		g_pD3DVtxBuffPStar->Release();
		g_pD3DVtxBuffPStar = NULL;
	}

	if (g_pD3DTexPStar)
	{
		g_pD3DTexPStar->Release();
		g_pD3DTexPStar = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticleStar()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < NUM_PSTAR; i++)
	{
		g_PStar[i].col.a += g_PStar[i].fade;	// 色を更新
		
		if (g_PStar[i].col.a > 1.0f)			
		{// アルファ値が1.0fより大きいなら、fadeをマイナスにする
			g_PStar[i].fade = -g_PStar[i].fade;
		}
		
		if (g_PStar[i].col.a <= 0.0f)
		{// アルファ値が0.0fより小さいなら、リセットする
			ResetPStar(&g_PStar[i]);
		}		
	}

	SetVtxPStar(pDevice);	// 頂点情報を更新
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticleStar()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// αテストを有効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int i = 0; i < NUM_PSTAR; i++)
	{
		D3DXMATRIX mtxWorld, mtxTranslate;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 移動を反映		
		D3DXMatrixTranslation(&mtxTranslate, g_PStar[i].pos.x, g_PStar[i].pos.y, g_PStar[i].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 例の設定
		pDevice->SetStreamSource(0, g_pD3DVtxBuffPStar, 0, sizeof(VERTEX_3D));
		pDevice->SetFVF(FVF_VERTEX_3D);
		pDevice->SetTexture(0, g_pD3DTexPStar);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, NUM_POLYGON);
	}

	// αテストを無効
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVtxPStar(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_PSTAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffPStar,
		NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffPStar->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < NUM_PSTAR; i++, pVtx += NUM_VERTEX)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_PSTAR / 2,  SIZE_PSTAR / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3( SIZE_PSTAR / 2,  SIZE_PSTAR / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_PSTAR / 2, -SIZE_PSTAR / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3( SIZE_PSTAR / 2, -SIZE_PSTAR / 2, 0.0f);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		g_pD3DVtxBuffPStar->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 色情報を頂点に反映する
//=============================================================================
void SetVtxPStar(LPDIRECT3DDEVICE9 pDevice)
{
	// 色の更新		
	VERTEX_3D *pVtx;
	g_pD3DVtxBuffPStar->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_PSTAR; i++, pVtx += 4)
	{
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = g_PStar[i].col;
		
	}

	g_pD3DVtxBuffPStar->Unlock();
}

//=============================================================================
// 
//=============================================================================
void ResetPStar(PSTAR* pPStar)
{
	// 位置を設定
	D3DXVECTOR3 min = D3DXVECTOR3(-100.0f / 2, -100.0f / 2, 1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(100.0f / 2, 100.0f / 2, 50.0f);

	GetRandomVector(&pPStar->pos, &min, &max);
	
	// アルファを設定
	pPStar->col.a = 0.0f;

	pPStar->fade = GetRandomFloat(0.01f, 0.02f);
}


