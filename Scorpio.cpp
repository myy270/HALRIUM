#include "Scorpio.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FILENAME			"data/TEXTURE/star.png"		// 星のテクスチャ
#define TEX_FILENAME_LINE		"data/TEXTURE/line.png"		// 線分のテクスチャ
#define NUM_STAR				(12)						// 蠍座の頂点数
#define NUM_EDGE				(11)						// 蠍座の辺数
#define SIZE_ARRAY				(NUM_STAR*(NUM_STAR+1)/2)	// 配列のサイズ
#define SCORE					(150)						// 配列のサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVtxSco(LPDIRECT3DDEVICE9 pDevice);			// 星の頂点をつくる
HRESULT MakeVtxScoLine(LPDIRECT3DDEVICE9 pDevice);		// 線分の頂点をつくる
void SetVtxColSco(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffSco;		// 星の頂点バッファ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffScoLine;	// 線分の頂点バッファ
LPDIRECT3DTEXTURE9		g_pD3DTexSco;			// 星のテクスチャ
LPDIRECT3DTEXTURE9		g_pD3DTexScoLine;		// 線分のテクスチャ
SEIZA					g_Sco;					// 蠍座の構造体へのポインター

// 蠍座の頂点情報、一個前の頂点との相対位置を保存、 最初の頂点はランダムで生成
D3DXVECTOR3	g_posSco[NUM_STAR] =
{
	{1.50f,  3.6f, 0.0f},	// このデータは最初の頂点ではない、インデックスを同一するだけ
	{ 3.08f , 1.97f, 0.0f},	// 1番
	{ 3.51f , 0.31f, 0.0f},	// 2
	{ 4.17f , -1.07f, 0.0f},	// 3
	{-0.44f , -0.65f, 0.0f},	// 4
	{-1.09f ,-4.57f, 0.0f},	// 5
	{-1.23f ,-7.37f, 0.0f},	// 6
	{-2.73f , -8.53f, 0.0f},	// 7
	{-5.03f , -8.49f, 0.0f},	// 8
	{-6.99f ,  -6.87f, 0.0f}, // 9
	{ -5.91f , -4.61f, 0.0f}, // 10
	{-8.18f , -5.23f, 0.0f},	// 11
};

static D3DXVECTOR3 posBuff[NUM_STAR];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSco()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Sco.name = SCO;
	g_Sco.belong = DEFAULT;

	InitSeiZa(&g_Sco, NUM_STAR, NUM_EDGE);

	// 星の初期化
	for (int i = 0; i < g_Sco.Nv; i++)
	{
		g_Sco.star[i].pos = g_posSco[i];
		g_Sco.star[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Sco.star[i].color = HALF_WHITE;
		g_Sco.star[i].radius = SIZE_STAR / 4.0f;
		g_Sco.star[i].fade = GetRandomFloat(0.002f, 0.006f);
		g_Sco.star[i].rot = GetRandomFloat(0.0f, 6.28f);
		g_Sco.star[i].isLight = false;
		g_Sco.star[i].effectIdx = -1;

		posBuff[i] = g_Sco.star[i].pos;
	}

	// 線分の初期化
	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		g_Sco.G[i] = 0;
		g_Sco.line[i].isLight = false;
		g_Sco.line[i].rot = 0.0f;
		g_Sco.line[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Sco.line[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Sco.line[i].color = WHITE;
	}

	//// 星の番号により、線分を設定
	//SetLine(&g_Sco, 0, 1);
	//SetLine(&g_Sco, 1, 2);
	//SetLine(&g_Sco, 1, 4);
	//SetLine(&g_Sco, 2, 3);
	//SetLine(&g_Sco, 4, 5);
	//SetLine(&g_Sco, 5, 6);
	//SetLine(&g_Sco, 6, 7);
	//SetLine(&g_Sco, 7, 8);
	//SetLine(&g_Sco, 8, 9);
	//SetLine(&g_Sco, 9, 10);
	//SetLine(&g_Sco, 10,11);

	// 頂点をつくる
	MakeVtxSco(pDevice);
	MakeVtxScoLine(pDevice);

	// load textures
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME, &g_pD3DTexSco);
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME_LINE, &g_pD3DTexScoLine);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSco()
{
	if (g_pD3DVtxBuffSco)
	{
		g_pD3DVtxBuffSco->Release();
		g_pD3DVtxBuffSco = NULL;
	}

	if (g_pD3DTexSco)
	{
		g_pD3DTexSco->Release();
		g_pD3DTexSco = NULL;
	}

	if (g_pD3DTexScoLine)
	{
		g_pD3DTexScoLine->Release();
		g_pD3DTexScoLine = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSco()
{
	//static D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 scl = (GetSeizaEffect() + 7)->scl;
	D3DXVECTOR3 pos = (GetSeizaEffect() + 7)->pos;

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_NUMPAD8))
	{
		scl.y += 0.01f;
	}
	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		scl.y -= 0.01f;
	}
	if (GetKeyboardPress(DIK_NUMPAD4))
	{
		scl.x -= 0.01f;
	}
	if (GetKeyboardPress(DIK_NUMPAD6))
	{
		scl.x += 0.01f;
	}


	if (GetKeyboardPress(DIK_NUMPAD3))
	{
		pos.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_DECIMAL))
	{
		pos.y -= 0.1f;
	}
	if (GetKeyboardPress(DIK_NUMPAD0))
	{
		pos.x -= 0.1f;
	}
	if (GetKeyboardPress(DIK_NUMPADENTER))
	{
		pos.x += 0.1f;
	}

	if (GetKeyboardPress(DIK_NUMPAD7))
	{
		rot.z += 0.01f;
	}
	if (GetKeyboardPress(DIK_NUMPAD9))
	{
		rot.z -= 0.01f;
	}
#endif

	D3DXMATRIX mtxScale, mtxTran, mtxRot;
	D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
	D3DXMatrixTranslation(&mtxTran, pos.x, pos.y, pos.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

	for (int i = 0; i < g_Sco.Nv; i++)
	{
		D3DXVec3TransformCoord(&g_Sco.star[i].pos, &posBuff[i], &mtxScale);//S
		D3DXVec3TransformCoord(&g_Sco.star[i].pos, &g_Sco.star[i].pos, &mtxRot);//R
		D3DXVec3TransformCoord(&g_Sco.star[i].pos, &g_Sco.star[i].pos, &mtxTran);//T

	}

	// 星の番号により、線分を設定
	SetLine(&g_Sco, 0, 1);
	SetLine(&g_Sco, 1, 2);
	SetLine(&g_Sco, 1, 4);
	SetLine(&g_Sco, 2, 3);
	SetLine(&g_Sco, 4, 5);
	SetLine(&g_Sco, 5, 6);
	SetLine(&g_Sco, 6, 7);
	SetLine(&g_Sco, 7, 8);
	SetLine(&g_Sco, 8, 9);
	SetLine(&g_Sco, 9, 10);
	SetLine(&g_Sco, 10,11);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	UpdateSeiza(&g_Sco, SCORE);

	SetVtxColSco(pDevice);


	//PrintDebugProc("Sco: scl(%f,%f) pos(%f,%f)\n\n", scl.x, scl.y, pos.x, pos.y);

	//for (int i = 0; i < g_Sco.Nv; i++)
	//{
	//	PrintDebugProc("Sco[%d]: pos(%f,%f)\n", i, g_Sco.star[i].pos.x, g_Sco.star[i].pos.y);
	//}




}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSco()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// 線分を描画------------------------------------------------------------------------------------
	// render state
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		D3DXMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;
		if (g_Sco.line[i].isLight)
		{
			D3DXMatrixIdentity(&mtxWorld);

			// 拡大縮小
			D3DXMatrixScaling(&mtxScale, g_Sco.line[i].scale.x, g_Sco.line[i].scale.y, g_Sco.line[i].scale.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Sco.line[i].rot);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Sco.line[i].pos.x, g_Sco.line[i].pos.y, g_Sco.line[i].pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 例の設定
			pDevice->SetStreamSource(0, g_pD3DVtxBuffScoLine, 0, sizeof(VERTEX_3D));
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pD3DTexScoLine);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
		}
	}
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ほしを描画------------------------------------------------------------------------------------
	for (int i = 0; i < NUM_STAR; i++)
	{	
		D3DXMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 拡大縮小
		D3DXMatrixScaling(&mtxScale, g_Sco.star[i].scale.x, g_Sco.star[i].scale.y, g_Sco.star[i].scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Sco.star[i].rot);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 移動を反映		
		D3DXMatrixTranslation(&mtxTranslate, g_Sco.star[i].pos.x, g_Sco.star[i].pos.y, g_Sco.star[i].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 例の設定
		pDevice->SetStreamSource(0, g_pD3DVtxBuffSco, 0, sizeof(VERTEX_3D));
		pDevice->SetFVF(FVF_VERTEX_3D);
		pDevice->SetTexture(0, g_pD3DTexSco);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVtxSco(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_STAR,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffSco,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffSco->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < NUM_STAR; i++, pVtx += NUM_VERTEX)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_STAR / 2,  SIZE_STAR / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3( SIZE_STAR / 2,  SIZE_STAR / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_STAR / 2, -SIZE_STAR / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3( SIZE_STAR / 2, -SIZE_STAR / 2, 0.0f);

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

		g_pD3DVtxBuffSco->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
HRESULT MakeVtxScoLine(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SIZE_ARRAY,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffScoLine,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffScoLine->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < SIZE_ARRAY; i++, pVtx += NUM_VERTEX)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_LINE / 2,  SIZE_LINE / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3( SIZE_LINE / 2,  SIZE_LINE / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_LINE / 2, -SIZE_LINE / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3( SIZE_LINE / 2, -SIZE_LINE / 2, 0.0f);

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

		g_pD3DVtxBuffScoLine->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
void SetVtxColSco(LPDIRECT3DDEVICE9 pDevice)
{
	// 色の更新		
	VERTEX_3D *pVtx;
	g_pD3DVtxBuffSco->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_STAR; i++, pVtx += NUM_VERTEX)
	{
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = g_Sco.star[i].color;

		g_pD3DVtxBuffSco->Unlock();
	}
}

//=============================================================================
// 
//=============================================================================
SEIZA* GetSco()
{
	return &g_Sco;
}

