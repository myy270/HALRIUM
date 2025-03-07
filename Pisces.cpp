#include "Pisces.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FILENAME		"data/TEXTURE/star.png"		// 星のテクスチャ
#define TEX_FILENAME_LINE	"data/TEXTURE/line.png"		// 線分のテクスチャ
#define	NUM_STAR			(15)						// 魚座の頂点数
#define NUM_EDGE			(15)						// 魚座の辺数
#define SIZE_ARRAY			(NUM_STAR*(NUM_STAR+1)/2)	// 配列のサイズ
#define SCORE				(90)						// 配列のサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVtxPis(LPDIRECT3DDEVICE9 pDevice);			// 星の頂点をつくる
HRESULT MakeVtxPisLine(LPDIRECT3DDEVICE9 pDevice);		// 線分の頂点をつくる	
void SetVtxColPis(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffPis;		// 星の頂点バッファ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffPisLine;	// 線分の頂点バッファ
LPDIRECT3DTEXTURE9		g_pD3DTexPis;			// 星のテクスチャ
LPDIRECT3DTEXTURE9		g_pD3DTexPisLine;		// 線分のテクスチャ
SEIZA					g_Pis;					// 魚座の構造体へのポインター

// 魚座の頂点情報
D3DXVECTOR3	g_posPis[NUM_STAR] =
{
	{-8.9f,  -6.4f, 0.0f },	
	{-2.8f  , -3.38f , 0.0f},		// 1
	{0.41f  , -3.19f  , 0.0f},		// 2
	{2.98f   , -2.62f  , 0.0f},		// 3
	{4.58f   , -2.62f  , 0.0f},		// 4
	{5.54f   , -2.05f  , 0.0f},		// 5
	{8.11f   , -2.53f , 0.0f},		// 6
	{7.15f  , -2.9f , 0.0f},		// 7
	{6.19f  , -4.04f , 0.0f},		// 8
	{4.58f  , -4.13f , 0.0f},		// 9
	{-6.65f , -0.54f , 0.0f},		// 10
	{-5.37f   , 1.92f , 0.0f},		// 11
	{-5.56f  , 3.24f  , 0.0f},		// 12
	{-6.2f  , 4.37f  , 0.0f},		// 13
	{-5.72f   , 5.89f  , 0.0f},		// 14
};

static D3DXVECTOR3 posBuff[NUM_STAR];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPis()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Pis.name = PIS;
	g_Pis.belong = DEFAULT;

	// 星座構造体の初期化
	InitSeiZa(&g_Pis, NUM_STAR, NUM_EDGE);

	// 星の初期化
	for (int i = 0; i < g_Pis.Nv; i++)
	{
		g_Pis.star[i].pos = g_posPis[i];
		g_Pis.star[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Pis.star[i].color = HALF_WHITE;
		g_Pis.star[i].radius = SIZE_STAR / 4.0f;
		g_Pis.star[i].fade = GetRandomFloat(0.002f, 0.006f);
		g_Pis.star[i].rot = GetRandomFloat(0.0f, 6.28f);
		g_Pis.star[i].isLight = false;
		g_Pis.star[i].effectIdx = -1;

		posBuff[i] = g_Pis.star[i].pos;
	}

	// 線分の初期化
	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		g_Pis.G[i] = 0;
		g_Pis.line[i].isLight = false;
		g_Pis.line[i].rot = 0.0f;
		g_Pis.line[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Pis.line[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Pis.line[i].color = WHITE;
	}

	//// 星の番号により、線分を設定
	//SetLine(&g_Pis, 0, 1);
	//SetLine(&g_Pis, 1, 2);
	//SetLine(&g_Pis, 2, 3);
	//SetLine(&g_Pis, 3, 4);
	//SetLine(&g_Pis, 4, 5);
	//SetLine(&g_Pis, 5, 6);
	//SetLine(&g_Pis, 6, 7);
	//SetLine(&g_Pis, 7, 8);
	//SetLine(&g_Pis, 8, 9);
	//SetLine(&g_Pis, 9, 3);
	//SetLine(&g_Pis, 0, 10);
	//SetLine(&g_Pis, 10, 11);
	//SetLine(&g_Pis, 11, 12);
	//SetLine(&g_Pis, 12, 13);
	//SetLine(&g_Pis, 13, 14);

	// 頂点をつくる
	MakeVtxPis(pDevice);
	MakeVtxPisLine(pDevice);

	// load textures
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME, &g_pD3DTexPis);
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME_LINE, &g_pD3DTexPisLine);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPis()
{
	if (g_pD3DVtxBuffPis)
	{
		g_pD3DVtxBuffPis->Release();
		g_pD3DVtxBuffPis = NULL;
	}

	if (g_pD3DTexPis)
	{
		g_pD3DTexPis->Release();
		g_pD3DTexPis = NULL;
	}

	if (g_pD3DTexPisLine)
	{
		g_pD3DTexPisLine->Release();
		g_pD3DTexPisLine = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePis()
{
	//static D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 scl = (GetSeizaEffect() + 11)->scl;
	D3DXVECTOR3 pos = (GetSeizaEffect() + 11)->pos;

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

	for (int i = 0; i < g_Pis.Nv; i++)
	{
		D3DXVec3TransformCoord(&g_Pis.star[i].pos, &posBuff[i], &mtxScale);//S
		D3DXVec3TransformCoord(&g_Pis.star[i].pos, &g_Pis.star[i].pos, &mtxRot);//R
		D3DXVec3TransformCoord(&g_Pis.star[i].pos, &g_Pis.star[i].pos, &mtxTran);//T

	}

	// 星の番号により、線分を設定
	SetLine(&g_Pis, 0, 1);
	SetLine(&g_Pis, 1, 2);
	SetLine(&g_Pis, 2, 3);
	SetLine(&g_Pis, 3, 4);
	SetLine(&g_Pis, 4, 5);
	SetLine(&g_Pis, 5, 6);
	SetLine(&g_Pis, 6, 7);
	SetLine(&g_Pis, 7, 8);
	SetLine(&g_Pis, 8, 9);
	SetLine(&g_Pis, 9, 3);
	SetLine(&g_Pis, 0, 10);
	SetLine(&g_Pis, 10, 11);
	SetLine(&g_Pis, 11, 12);
	SetLine(&g_Pis, 12, 13);
	SetLine(&g_Pis, 13, 14);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	UpdateSeiza(&g_Pis, SCORE);

	SetVtxColPis(pDevice);

	//PrintDebugProc("Pis: scl(%f,%f) pos(%f,%f)\n\n", scl.x, scl.y, pos.x, pos.y);

	//for (int i = 0; i < g_Pis.Nv; i++)
	//{
	//	PrintDebugProc("Pis[%d]: pos(%f,%f)\n", i, g_Pis.star[i].pos.x, g_Pis.star[i].pos.y);
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPis()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 線分を描画------------------------------------------------------------------------------------
	// render state
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		D3DXMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;
		if (g_Pis.line[i].isLight)
		{
			D3DXMatrixIdentity(&mtxWorld);

			// 拡大縮小
			D3DXMatrixScaling(&mtxScale, g_Pis.line[i].scale.x, g_Pis.line[i].scale.y, g_Pis.line[i].scale.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Pis.line[i].rot);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Pis.line[i].pos.x, g_Pis.line[i].pos.y, g_Pis.line[i].pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 例の設定
			pDevice->SetStreamSource(0, g_pD3DVtxBuffPisLine, 0, sizeof(VERTEX_3D));
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pD3DTexPisLine);

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
		D3DXMatrixScaling(&mtxScale, g_Pis.star[i].scale.x, g_Pis.star[i].scale.y, g_Pis.star[i].scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Pis.star[i].rot);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 移動を反映		
		D3DXMatrixTranslation(&mtxTranslate, g_Pis.star[i].pos.x, g_Pis.star[i].pos.y, g_Pis.star[i].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 例の設定
		pDevice->SetStreamSource(0, g_pD3DVtxBuffPis, 0, sizeof(VERTEX_3D));
		pDevice->SetFVF(FVF_VERTEX_3D);
		pDevice->SetTexture(0, g_pD3DTexPis);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVtxPis(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_STAR,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffPis,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffPis->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffPis->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
HRESULT MakeVtxPisLine(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SIZE_ARRAY,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffPisLine,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffPisLine->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffPisLine->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
void SetVtxColPis(LPDIRECT3DDEVICE9 pDevice)
{
	// 色の更新		
	VERTEX_3D *pVtx;
	g_pD3DVtxBuffPis->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_STAR; i++, pVtx += NUM_VERTEX)
	{
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = g_Pis.star[i].color;

		g_pD3DVtxBuffPis->Unlock();
	}
}

//=============================================================================
// 
//=============================================================================
SEIZA* GetPis()
{
	return &g_Pis;
}

