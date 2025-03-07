#include "Aquarius.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FILENAME		"data/TEXTURE/star.png"		// 星のテクスチャ
#define TEX_FILENAME_LINE	"data/TEXTURE/line.png"		// 線分のテクスチャ
#define	NUM_STAR			(18)						// 水瓶座の頂点数
#define NUM_EDGE			(18)						// 水瓶座の辺数
#define SIZE_ARRAY			(NUM_STAR*(NUM_STAR+1)/2)	// 配列のサイズ
#define SCORE				(200)						// 配列のサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVtxAqu(LPDIRECT3DDEVICE9 pDevice);			// 星の頂点をつくる
HRESULT MakeVtxAquLine(LPDIRECT3DDEVICE9 pDevice);		// 線分の頂点をつくる	
void SetVtxColAqu(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffAqu;		// 星の頂点バッファ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffAquLine;	// 線分の頂点バッファ
LPDIRECT3DTEXTURE9		g_pD3DTexAqu;			// 星のテクスチャ
LPDIRECT3DTEXTURE9		g_pD3DTexAquLine;		// 線分のテクスチャ
SEIZA					g_Aqu;					// 水瓶座の構造体へのポインター

// 水瓶座の頂点情報
D3DXVECTOR3	g_posAqu[NUM_STAR] =
{
	{ -4.0f,  3.5f, 0.0f },	
	{-1.27f   ,  4.05f , 0.0f},	// 1
	{-2.74f  , 2.3f , 0.0f},	// 2
	{0.1f   , 4.33f  , 0.0f},	// 3
	{0.65f   , 5.78f  , 0.0f},	// 4
	{1.14f   , 3.14f , 0.0f},	// 5
	{2.51f   , 3.42f  , 0.0f},	// 6
	{4.76f   , 0.28f , 0.0f},	// 7
	{6.81f   , -2.46f , 0.0f},	// 8
	{2.34f , -2.15f  , 0.0f},	// 9
	{1.18f  , 0.53f  , 0.0f},	// 10
	{-0.49f  , -0.84f , 0.0f},	// 11
	{-2.63f  , -2.55f , 0.0f},	// 12
	{-4.32f  , -4.29f , 0.0f},	// 13
	{-3.86f   , -7.3f , 0.0f},	// 14
	{-5.86f  , -3.81f  , 0.0f},	// 15
	{-6.8f  , -1.14f  , 0.0f},	// 16
	{-4.83f   , 1.33f  , 0.0f},	// 17
};

static D3DXVECTOR3 posBuff[NUM_STAR];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAqu()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Aqu.name = AQU;
	g_Aqu.belong = DEFAULT;

	// 星座構造体の初期化
	InitSeiZa(&g_Aqu, NUM_STAR, NUM_EDGE);

	// 星の初期化
	for (int i = 0; i < g_Aqu.Nv; i++)
	{
		g_Aqu.star[i].pos = g_posAqu[i];
		g_Aqu.star[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Aqu.star[i].color = HALF_WHITE;
		g_Aqu.star[i].radius = SIZE_STAR / 4.0f;
		g_Aqu.star[i].fade = GetRandomFloat(0.002f, 0.006f);
		g_Aqu.star[i].rot = GetRandomFloat(0.0f, 6.28f);
		g_Aqu.star[i].isLight = false;
		g_Aqu.star[i].effectIdx = -1;

		posBuff[i] = g_Aqu.star[i].pos;
	}

	// 線分の初期化
	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		g_Aqu.G[i] = 0;
		g_Aqu.line[i].isLight = false;
		g_Aqu.line[i].rot = 0.0f;
		g_Aqu.line[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Aqu.line[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Aqu.line[i].color = WHITE;
	}

	//// 星の番号により、線分を設定
	//SetLine(&g_Aqu, 0, 1);
	//SetLine(&g_Aqu, 1, 2);
	//SetLine(&g_Aqu, 1, 3);
	//SetLine(&g_Aqu, 3, 4);
	//SetLine(&g_Aqu, 3, 5);
	//SetLine(&g_Aqu, 5, 6);
	//SetLine(&g_Aqu, 6, 7);
	//SetLine(&g_Aqu, 7, 8);
	//SetLine(&g_Aqu, 7, 9);
	//SetLine(&g_Aqu, 6, 10);
	//SetLine(&g_Aqu, 10, 11);
	//SetLine(&g_Aqu, 11, 12);
	//SetLine(&g_Aqu, 2, 13);
	//SetLine(&g_Aqu, 13, 14);
	//SetLine(&g_Aqu, 14, 15);
	//SetLine(&g_Aqu, 15, 16);
	//SetLine(&g_Aqu, 16, 17);
	//SetLine(&g_Aqu, 17, 0);

	// 頂点をつくる
	MakeVtxAqu(pDevice);
	MakeVtxAquLine(pDevice);

	// load textures
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME, &g_pD3DTexAqu);
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME_LINE, &g_pD3DTexAquLine);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAqu()
{
	if (g_pD3DVtxBuffAqu)
	{
		g_pD3DVtxBuffAqu->Release();
		g_pD3DVtxBuffAqu = NULL;
	}

	if (g_pD3DTexAqu)
	{
		g_pD3DTexAqu->Release();
		g_pD3DTexAqu = NULL;
	}

	if (g_pD3DTexAquLine)
	{
		g_pD3DTexAquLine->Release();
		g_pD3DTexAquLine = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateAqu()
{
	//static D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 scl = (GetSeizaEffect() + 10)->scl;
	D3DXVECTOR3 pos = (GetSeizaEffect() + 10)->pos;

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

	for (int i = 0; i < g_Aqu.Nv; i++)
	{
		D3DXVec3TransformCoord(&g_Aqu.star[i].pos, &posBuff[i], &mtxScale);//S
		D3DXVec3TransformCoord(&g_Aqu.star[i].pos, &g_Aqu.star[i].pos, &mtxRot);//R
		D3DXVec3TransformCoord(&g_Aqu.star[i].pos, &g_Aqu.star[i].pos, &mtxTran);//T

	}

	// 星の番号により、線分を設定
	SetLine(&g_Aqu, 0, 1);
	SetLine(&g_Aqu, 1, 2);
	SetLine(&g_Aqu, 1, 3);
	SetLine(&g_Aqu, 3, 4);
	SetLine(&g_Aqu, 3, 5);
	SetLine(&g_Aqu, 5, 6);
	SetLine(&g_Aqu, 6, 7);
	SetLine(&g_Aqu, 7, 8);
	SetLine(&g_Aqu, 7, 9);
	SetLine(&g_Aqu, 6, 10);
	SetLine(&g_Aqu, 10, 11);
	SetLine(&g_Aqu, 11, 12);
	SetLine(&g_Aqu, 2, 13);
	SetLine(&g_Aqu, 13, 14);
	SetLine(&g_Aqu, 14, 15);
	SetLine(&g_Aqu, 15, 16);
	SetLine(&g_Aqu, 16, 17);
	SetLine(&g_Aqu, 17, 0);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	UpdateSeiza(&g_Aqu, SCORE);

	SetVtxColAqu(pDevice);

	//PrintDebugProc("Aqu: scl(%f,%f) pos(%f,%f)\n\n", scl.x, scl.y, pos.x, pos.y);

	//for (int i = 0; i < g_Aqu.Nv; i++)
	//{
	//	PrintDebugProc("Aqu[%d]: pos(%f,%f)\n", i, g_Aqu.star[i].pos.x, g_Aqu.star[i].pos.y);
	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAqu()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 線分を描画------------------------------------------------------------------------------------
	// render state
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		D3DXMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;
		if (g_Aqu.line[i].isLight)
		{
			D3DXMatrixIdentity(&mtxWorld);

			// 拡大縮小
			D3DXMatrixScaling(&mtxScale, g_Aqu.line[i].scale.x, g_Aqu.line[i].scale.y, g_Aqu.line[i].scale.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Aqu.line[i].rot);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Aqu.line[i].pos.x, g_Aqu.line[i].pos.y, g_Aqu.line[i].pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 例の設定
			pDevice->SetStreamSource(0, g_pD3DVtxBuffAquLine, 0, sizeof(VERTEX_3D));
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pD3DTexAquLine);

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
		D3DXMatrixScaling(&mtxScale, g_Aqu.star[i].scale.x, g_Aqu.star[i].scale.y, g_Aqu.star[i].scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Aqu.star[i].rot);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 移動を反映		
		D3DXMatrixTranslation(&mtxTranslate, g_Aqu.star[i].pos.x, g_Aqu.star[i].pos.y, g_Aqu.star[i].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 例の設定
		pDevice->SetStreamSource(0, g_pD3DVtxBuffAqu, 0, sizeof(VERTEX_3D));
		pDevice->SetFVF(FVF_VERTEX_3D);
		pDevice->SetTexture(0, g_pD3DTexAqu);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVtxAqu(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_STAR,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffAqu,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffAqu->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffAqu->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
HRESULT MakeVtxAquLine(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SIZE_ARRAY,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffAquLine,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffAquLine->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffAquLine->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
void SetVtxColAqu(LPDIRECT3DDEVICE9 pDevice)
{
	// 色の更新		
	VERTEX_3D *pVtx;
	g_pD3DVtxBuffAqu->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_STAR; i++, pVtx += NUM_VERTEX)
	{
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = g_Aqu.star[i].color;

		g_pD3DVtxBuffAqu->Unlock();
	}
}

//=============================================================================
// 
//=============================================================================
SEIZA* GetAqu()
{
	return &g_Aqu;
}

