#include "Aries.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FILENAME		"data/TEXTURE/star.png"		// 星のテクスチャ
#define TEX_FILENAME_LINE	"data/TEXTURE/line.png"		// 線分のテクスチャ
#define	NUM_STAR			(4)							// 牡羊座の頂点数
#define NUM_EDGE			(3)							// 牡羊座の辺数
#define SIZE_ARRAY			(NUM_STAR*(NUM_STAR+1)/2)	// 配列のサイズ
#define SCORE				(50)						// 配列のサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVtxAri(LPDIRECT3DDEVICE9 pDevice);			// 星の頂点をつくる
HRESULT MakeVtxAriLine(LPDIRECT3DDEVICE9 pDevice);		// 線分の頂点をつくる	
void SetVtxColAri(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffAri;		// 星の頂点バッファ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffAriLine;	// 線分の頂点バッファ
LPDIRECT3DTEXTURE9		g_pD3DTexAri;			// 星のテクスチャ
LPDIRECT3DTEXTURE9		g_pD3DTexAriLine;		// 線分のテクスチャ
SEIZA					g_Ari;					// 牡羊座の構造体へのポインター

// 牡羊座の頂点情報
D3DXVECTOR3	g_posAri[NUM_STAR] =
{
	{ -4.24f,  0.66f, 0.0f},
	{ 6.24f,  1.85f, 0.0f},		// 1
	{ 5.0f, 3.63f, 0.0f},		// 2
	{ 1.31f, 4.52f, 0.0f},		// 3
};

static D3DXVECTOR3 posBuff[NUM_STAR];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAri()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Ari.name = ARI;
	g_Ari.belong = DEFAULT;

	// 星座構造体の初期化
	InitSeiZa(&g_Ari, NUM_STAR, NUM_EDGE);

	// 星の初期化
	for (int i = 0; i < g_Ari.Nv; i++)
	{
		g_Ari.star[i].pos = g_posAri[i];
		g_Ari.star[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Ari.star[i].color = HALF_WHITE;
		g_Ari.star[i].radius = SIZE_STAR / 4.0f;
		g_Ari.star[i].fade = GetRandomFloat(0.002f, 0.006f);
		g_Ari.star[i].rot = GetRandomFloat(0.0f, 6.28f);
		g_Ari.star[i].isLight = false;
		g_Ari.star[i].effectIdx = -1;
	}

	//原点を中心に
	//D3DXVECTOR3 posOffset = D3DXVECTOR3(-(g_Ari.star[0].pos.x + g_Ari.star[1].pos.x) / 2, -(g_Ari.star[0].pos.y + g_Ari.star[3].pos.y) / 2, 0.0f);

	for (int i = 0; i < g_Ari.Nv; i++)
	{
		//g_Ari.star[i].pos += posOffset;
		posBuff[i] = g_Ari.star[i].pos;
	}

	// 線分の初期化
	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		g_Ari.G[i] = 0;
		g_Ari.line[i].isLight = false;
		g_Ari.line[i].rot = 0.0f;
		g_Ari.line[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Ari.line[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Ari.line[i].color = WHITE;
	}

	//// 星の番号により、線分を設定
	//SetLine(&g_Ari, 0, 1);
	//SetLine(&g_Ari, 1, 2);
	//SetLine(&g_Ari, 2, 3);

	// 頂点をつくる
	MakeVtxAri(pDevice);
	MakeVtxAriLine(pDevice);

	// load textures
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME, &g_pD3DTexAri);
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME_LINE, &g_pD3DTexAriLine);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAri()
{
	if (g_pD3DVtxBuffAri)
	{
		g_pD3DVtxBuffAri->Release();
		g_pD3DVtxBuffAri = NULL;
	}

	if (g_pD3DTexAri)
	{
		g_pD3DTexAri->Release();
		g_pD3DTexAri = NULL;
	}

	if (g_pD3DTexAriLine)
	{
		g_pD3DTexAriLine->Release();
		g_pD3DTexAriLine = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateAri()
{
	//static D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 scl = GetSeizaEffect()->scl;
	D3DXVECTOR3 pos = GetSeizaEffect()->pos;

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
		pos.y += 0.01f;
	}
	if (GetKeyboardPress(DIK_DECIMAL))
	{
		pos.y -= 0.01f;
	}
	if (GetKeyboardPress(DIK_NUMPAD0))
	{
		pos.x -= 0.01f;
	}
	if (GetKeyboardPress(DIK_NUMPADENTER))
	{
		pos.x += 0.01f;
	}
#endif


	D3DXMATRIX mtxScale, mtxTran;
	D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
	D3DXMatrixTranslation(&mtxTran, pos.x, pos.y, pos.z);

	for (int i = 0; i < g_Ari.Nv; i++)
	{
		D3DXVec3TransformCoord(&g_Ari.star[i].pos, &posBuff[i], &mtxScale);//S
		D3DXVec3TransformCoord(&g_Ari.star[i].pos, &g_Ari.star[i].pos, &mtxTran);//T
	}

	// 星の番号により、線分を設定
	SetLine(&g_Ari, 0, 1);
	SetLine(&g_Ari, 1, 2);
	SetLine(&g_Ari, 2, 3);


	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	UpdateSeiza(&g_Ari, SCORE);

	SetVtxColAri(pDevice);

	//PrintDebugProc("Ari: scl(%f,%f) pos(%f,%f)\n\n", scl.x, scl.y, pos.x, pos.y);

	//for (int i = 0; i < g_Ari.Nv; i++)
	//{
	//	PrintDebugProc("Ari[%d]: pos(%f,%f)\n", i, g_Ari.star[i].pos.x, g_Ari.star[i].pos.y);
	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAri()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 線分を描画------------------------------------------------------------------------------------
	// render state
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		D3DXMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;
		if (g_Ari.line[i].isLight)
		{
			D3DXMatrixIdentity(&mtxWorld);

			// 拡大縮小
			D3DXMatrixScaling(&mtxScale, g_Ari.line[i].scale.x, g_Ari.line[i].scale.y, g_Ari.line[i].scale.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Ari.line[i].rot);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Ari.line[i].pos.x, g_Ari.line[i].pos.y, g_Ari.line[i].pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 例の設定
			pDevice->SetStreamSource(0, g_pD3DVtxBuffAriLine, 0, sizeof(VERTEX_3D));
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pD3DTexAriLine);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
		}
	}
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ほしを描画------------------------------------------------------------------------------------
	for (int i = 0; i < NUM_STAR; i++)
	{
		D3DXMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate, mtxTranslate2;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 拡大縮小
		D3DXMatrixScaling(&mtxScale, g_Ari.star[i].scale.x, g_Ari.star[i].scale.y, g_Ari.star[i].scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Ari.star[i].rot);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 移動を反映		
		D3DXMatrixTranslation(&mtxTranslate, g_Ari.star[i].pos.x, g_Ari.star[i].pos.y, g_Ari.star[i].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 例の設定
		pDevice->SetStreamSource(0, g_pD3DVtxBuffAri, 0, sizeof(VERTEX_3D));
		pDevice->SetFVF(FVF_VERTEX_3D);
		pDevice->SetTexture(0, g_pD3DTexAri);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVtxAri(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_STAR,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffAri,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffAri->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffAri->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
HRESULT MakeVtxAriLine(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SIZE_ARRAY,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffAriLine,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffAriLine->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffAriLine->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
void SetVtxColAri(LPDIRECT3DDEVICE9 pDevice)
{
	// 色の更新		
	VERTEX_3D *pVtx;
	g_pD3DVtxBuffAri->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_STAR; i++, pVtx += NUM_VERTEX)
	{
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = g_Ari.star[i].color;

		g_pD3DVtxBuffAri->Unlock();
	}
}

//=============================================================================
// 
//=============================================================================
SEIZA* GetAri()
{
	return &g_Ari;
}

