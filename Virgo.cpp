#include "Virgo.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FILENAME		"data/TEXTURE/star.png"		// 星のテクスチャ
#define TEX_FILENAME_LINE	"data/TEXTURE/line.png"		// 線分のテクスチャ
#define	NUM_STAR			(14)						// 乙女座の頂点数
#define NUM_EDGE			(13)						// 乙女座の辺数
#define SIZE_ARRAY			(NUM_STAR*(NUM_STAR+1)/2)	// 配列のサイズ
#define SCORE				(100)						// 配列のサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVtxVir(LPDIRECT3DDEVICE9 pDevice);			// 星の頂点をつくる
HRESULT MakeVtxVirLine(LPDIRECT3DDEVICE9 pDevice);		// 線分の頂点をつくる	
void SetVtxColVir(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffVir;		// 星の頂点バッファ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffVirLine;	// 線分の頂点バッファ
LPDIRECT3DTEXTURE9		g_pD3DTexVir;			// 星のテクスチャ
LPDIRECT3DTEXTURE9		g_pD3DTexVirLine;		// 線分のテクスチャ
SEIZA					g_Vir;					// 乙女座の構造体へのポインター

// 乙女座の頂点情報
D3DXVECTOR3	g_posVir[NUM_STAR] =
{
	{ -6.55f,  -0.45f, 0.0f },	
	{-3.63f, 0.37f, 0.0f},		// 1
	{-1.29f,-0.86f , 0.0f},		// 2
	{-0.13f, 1.18f, 0.0f},		// 3
	{0.17f,4.04f , 0.0f},		// 4
	{4.25f, 4.04f, 0.0f},		// 5
	{6.59f,3.63f , 0.0f},		// 6
	{6.59f, 0.77f, 0.0f},		// 7
	{3.67f, -1.27f, 0.0f},		// 8
	{1.33f, -2.49f, 0.0f},		// 9
	{0.75f, -5.75f, 0.0f},		// 10
	{-1.0f, -4.94f, 0.0f},		// 11
	{-2.17f,-2.90f , 0.0f},		// 12
	{-6.84f, -3.71f, 0.0f},		// 13
};

static D3DXVECTOR3 posBuff[NUM_STAR];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitVir()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Vir.name = VIR;
	g_Vir.belong = DEFAULT;

	// 星座構造体の初期化
	InitSeiZa(&g_Vir, NUM_STAR, NUM_EDGE);

	// 星の初期化
	for (int i = 0; i < g_Vir.Nv; i++)
	{
		g_Vir.star[i].pos = g_posVir[i];
		g_Vir.star[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Vir.star[i].color = HALF_WHITE;
		g_Vir.star[i].radius = SIZE_STAR / 4.0f;
		g_Vir.star[i].fade = GetRandomFloat(0.002f, 0.006f);
		g_Vir.star[i].rot = GetRandomFloat(0.0f, 6.28f);
		g_Vir.star[i].isLight = false;
		g_Vir.star[i].effectIdx = -1;

		posBuff[i] = g_Vir.star[i].pos;
	}

	// 線分の初期化
	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		g_Vir.G[i] = 0;
		g_Vir.line[i].isLight = false;
		g_Vir.line[i].rot = 0.0f;
		g_Vir.line[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Vir.line[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Vir.line[i].color = WHITE;
	}

	// 星の番号により、線分を設定
	//SetLine(&g_Vir, 0, 1);
	//SetLine(&g_Vir, 1, 3);
	//SetLine(&g_Vir, 2, 3);
	//SetLine(&g_Vir, 3, 4);
	//SetLine(&g_Vir, 3, 9);
	//SetLine(&g_Vir, 4, 5);
	//SetLine(&g_Vir, 5, 6);
	//SetLine(&g_Vir, 7, 8);
	//SetLine(&g_Vir, 8, 9);
	//SetLine(&g_Vir, 9, 10);
	//SetLine(&g_Vir, 9, 11);
	//SetLine(&g_Vir, 9, 12);
	//SetLine(&g_Vir, 12, 13);

	// 頂点をつくる
	MakeVtxVir(pDevice);
	MakeVtxVirLine(pDevice);

	// load textures
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME, &g_pD3DTexVir);
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME_LINE, &g_pD3DTexVirLine);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitVir()
{
	if (g_pD3DVtxBuffVir)
	{
		g_pD3DVtxBuffVir->Release();
		g_pD3DVtxBuffVir = NULL;
	}

	if (g_pD3DTexVir)
	{
		g_pD3DTexVir->Release();
		g_pD3DTexVir = NULL;
	}

	if (g_pD3DTexVirLine)
	{
		g_pD3DTexVirLine->Release();
		g_pD3DTexVirLine = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateVir()
{
	//static D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 scl = (GetSeizaEffect() + 5)->scl;
	D3DXVECTOR3 pos = (GetSeizaEffect() + 5)->pos;

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

	for (int i = 0; i < g_Vir.Nv; i++)
	{
		D3DXVec3TransformCoord(&g_Vir.star[i].pos, &posBuff[i], &mtxScale);//S
		D3DXVec3TransformCoord(&g_Vir.star[i].pos, &g_Vir.star[i].pos, &mtxRot);//R
		D3DXVec3TransformCoord(&g_Vir.star[i].pos, &g_Vir.star[i].pos, &mtxTran);//T

	}

	SetLine(&g_Vir, 0, 1);
	SetLine(&g_Vir, 1, 3);
	SetLine(&g_Vir, 2, 3);
	SetLine(&g_Vir, 3, 4);
	SetLine(&g_Vir, 3, 9);
	SetLine(&g_Vir, 4, 5);
	SetLine(&g_Vir, 5, 6);
	SetLine(&g_Vir, 7, 8);
	SetLine(&g_Vir, 8, 9);
	SetLine(&g_Vir, 9, 10);
	SetLine(&g_Vir, 9, 11);
	SetLine(&g_Vir, 9, 12);
	SetLine(&g_Vir, 12, 13);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	UpdateSeiza(&g_Vir, SCORE);

	SetVtxColVir(pDevice);

	//PrintDebugProc("Vir: scl(%f,%f) pos(%f,%f)\n\n", scl.x, scl.y, pos.x, pos.y);

	//for (int i = 0; i < g_Vir.Nv; i++)
	//{
	//	PrintDebugProc("Vir[%d]: pos(%f,%f)\n", i, g_Vir.star[i].pos.x, g_Vir.star[i].pos.y);
	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawVir()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 線分を描画------------------------------------------------------------------------------------
	// render state
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		D3DXMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;
		if (g_Vir.line[i].isLight)
		{
			D3DXMatrixIdentity(&mtxWorld);

			// 拡大縮小
			D3DXMatrixScaling(&mtxScale, g_Vir.line[i].scale.x, g_Vir.line[i].scale.y, g_Vir.line[i].scale.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Vir.line[i].rot);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Vir.line[i].pos.x, g_Vir.line[i].pos.y, g_Vir.line[i].pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 例の設定
			pDevice->SetStreamSource(0, g_pD3DVtxBuffVirLine, 0, sizeof(VERTEX_3D));
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pD3DTexVirLine);

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
		D3DXMatrixScaling(&mtxScale, g_Vir.star[i].scale.x, g_Vir.star[i].scale.y, g_Vir.star[i].scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Vir.star[i].rot);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 移動を反映		
		D3DXMatrixTranslation(&mtxTranslate, g_Vir.star[i].pos.x, g_Vir.star[i].pos.y, g_Vir.star[i].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 例の設定
		pDevice->SetStreamSource(0, g_pD3DVtxBuffVir, 0, sizeof(VERTEX_3D));
		pDevice->SetFVF(FVF_VERTEX_3D);
		pDevice->SetTexture(0, g_pD3DTexVir);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVtxVir(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_STAR,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffVir,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffVir->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffVir->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
HRESULT MakeVtxVirLine(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SIZE_ARRAY,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffVirLine,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffVirLine->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffVirLine->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
void SetVtxColVir(LPDIRECT3DDEVICE9 pDevice)
{
	// 色の更新		
	VERTEX_3D *pVtx;
	g_pD3DVtxBuffVir->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_STAR; i++, pVtx += NUM_VERTEX)
	{
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = g_Vir.star[i].color;

		g_pD3DVtxBuffVir->Unlock();
	}
}

//=============================================================================
// 
//=============================================================================
SEIZA* GetVir()
{
	return &g_Vir;
}

