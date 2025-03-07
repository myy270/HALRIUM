//=============================================================================
//
// 背景処理 [basicScene.cpp]
// Author : katei
//
//=============================================================================
#include "basicScene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FILENAME	"data/TEXTURE/sky_01.jpg"	// テクスチャファイルネーム
#define MOVE_BS			(0.0001f)					// テクスチャ座標の移動量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBS(LPDIRECT3DDEVICE9 pDevice);
void SetTextureBS(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBS;	// 頂点バッファ
LPDIRECT3DTEXTURE9		g_pD3DTexBS;		// テクスチャ
float					g_fMoveBS;			// テクスチャの移動量
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBasicScene(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_fMoveBS = 0.0f;

	// 頂点をつくる
	MakeVertexBS(pDevice);

	// テクスチャ
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME, &g_pD3DTexBS);

	// ビューマトリックス
	D3DXVECTOR3 pos(0.0f, 0.0f, -100.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	pDevice->SetTransform(D3DTS_VIEW, &V);

	// プロジェクションマトリックス
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI / 4,
		SCREEN_ASPECT,
		1.0f,
		1000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBasicScene(void)
{
	if (g_pD3DVtxBuffBS)
	{
		g_pD3DVtxBuffBS->Release();
		g_pD3DVtxBuffBS = NULL;
	}
	if (g_pD3DTexBS)
	{
		g_pD3DTexBS->Release();
		g_pD3DTexBS = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBasicScene(void)
{
	g_fMoveBS += MOVE_BS;	//　テクスチャを移動

	SetTextureBS();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBasicScene(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX I;

	D3DXMatrixIdentity(&I);

	pDevice->SetTransform(D3DTS_WORLD, &I);

	pDevice->SetTexture(0, g_pD3DTexBS);

	pDevice->SetStreamSource(0, g_pD3DVtxBuffBS, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBS(LPDIRECT3DDEVICE9 pDevice)
{
	HRESULT hr;

	hr = pDevice->CreateVertexBuffer(
		4 * sizeof(VERTEX_3D),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffBS,
		NULL);

	if (FAILED(hr))
		return E_FAIL;

	VERTEX_3D* pV;
	g_pD3DVtxBuffBS->Lock(0, 0, (void**)&pV, 0);

	pV[0].vtx = D3DXVECTOR3(-SIZE_BASICSCENE / 2,  SIZE_BASICSCENE / 2, POS_BASICSCENE_Z);
	pV[1].vtx = D3DXVECTOR3( SIZE_BASICSCENE / 2,  SIZE_BASICSCENE / 2, POS_BASICSCENE_Z);
	pV[2].vtx = D3DXVECTOR3(-SIZE_BASICSCENE / 2, -SIZE_BASICSCENE / 2, POS_BASICSCENE_Z);
	pV[3].vtx = D3DXVECTOR3( SIZE_BASICSCENE / 2, -SIZE_BASICSCENE / 2, POS_BASICSCENE_Z);

	pV[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pV[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pV[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pV[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	pV[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pV[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pV[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pV[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pV[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pV[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pV[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pV[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pD3DVtxBuffBS->Unlock();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureBS(void)
{
	VERTEX_3D* pV;
	g_pD3DVtxBuffBS->Lock(0, 0, (void**)&pV, 0);

	pV[0].tex = D3DXVECTOR2(0.0f + g_fMoveBS, 0.0f);
	pV[1].tex = D3DXVECTOR2(1.0f + g_fMoveBS, 0.0f);
	pV[2].tex = D3DXVECTOR2(0.0f + g_fMoveBS, 1.0f);
	pV[3].tex = D3DXVECTOR2(1.0f + g_fMoveBS, 1.0f);

	g_pD3DVtxBuffBS->Unlock();
}


	



