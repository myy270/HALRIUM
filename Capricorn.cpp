#include "Capricorn.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FILENAME		"data/TEXTURE/star.png"		// 星のテクスチャ
#define TEX_FILENAME_LINE	"data/TEXTURE/line.png"		// 線分のテクスチャ
#define	NUM_STAR			(9)							// 山羊座の頂点数
#define NUM_EDGE			(9)							// 山羊座の辺数
#define SIZE_ARRAY			(NUM_STAR*(NUM_STAR+1)/2)	// 配列のサイズ
#define SCORE				(100)						// 配列のサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVtxCap(LPDIRECT3DDEVICE9 pDevice);			// 星の頂点をつくる
HRESULT MakeVtxCapLine(LPDIRECT3DDEVICE9 pDevice);		// 線分の頂点をつくる	
void SetVtxColCap(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffCap;		// 星の頂点バッファ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffCapLine;	// 線分の頂点バッファ
LPDIRECT3DTEXTURE9		g_pD3DTexCap;			// 星のテクスチャ
LPDIRECT3DTEXTURE9		g_pD3DTexCapLine;		// 線分のテクスチャ
SEIZA					g_Cap;					// 山羊座の構造体へのポインター

// 山羊座の頂点情報
D3DXVECTOR3	g_posCap[NUM_STAR] =
{
	{ -6.10f,  1.1f, 0.0f },	
	{ -4.3f , 0.67f  , 0.0f},		// 1
	{ -1.31f , -3.6f , 0.0f},		// 2
	{ 1.68f , -5.74f  , 0.0f},		// 3
	{ 3.48f ,  -4.03f  , 0.0f},		// 4
	{ 5.27f , 0.67f  , 0.0f},		// 5
	{ 5.87f ,  2.81f  , 0.0f},		// 6
	{2.88f , -0.18f  , 0.0f},		// 7
	{-0.71f , -0.61f  , 0.0f},		// 8
};

static D3DXVECTOR3 posBuff[NUM_STAR];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCap()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Cap.name = CAP;
	g_Cap.belong = DEFAULT;

	// 星座構造体の初期化
	InitSeiZa(&g_Cap, NUM_STAR, NUM_EDGE);

	// 星の初期化
	for (int i = 0; i < g_Cap.Nv; i++)
	{
		g_Cap.star[i].pos = g_posCap[i];
		g_Cap.star[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Cap.star[i].color = HALF_WHITE;
		g_Cap.star[i].radius = SIZE_STAR / 4.0f;
		g_Cap.star[i].fade = GetRandomFloat(0.002f, 0.006f);
		g_Cap.star[i].rot = GetRandomFloat(0.0f, 6.28f);
		g_Cap.star[i].isLight = false;
		g_Cap.star[i].effectIdx = -1;

		posBuff[i] = g_Cap.star[i].pos;
	}

	// 線分の初期化
	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		g_Cap.G[i] = 0;
		g_Cap.line[i].isLight = false;
		g_Cap.line[i].rot = 0.0f;
		g_Cap.line[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Cap.line[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Cap.line[i].color = WHITE;
	}

	//// 星の番号により、線分を設定
	//SetLine(&g_Cap, 0, 1);
	//SetLine(&g_Cap, 1, 2);
	//SetLine(&g_Cap, 1, 8);
	//SetLine(&g_Cap, 2, 3);
	//SetLine(&g_Cap, 3, 4);
	//SetLine(&g_Cap, 4, 5);
	//SetLine(&g_Cap, 5, 6);
	//SetLine(&g_Cap, 5, 7);
	//SetLine(&g_Cap, 7, 8);

	// 頂点をつくる
	MakeVtxCap(pDevice);
	MakeVtxCapLine(pDevice);

	// load textures
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME, &g_pD3DTexCap);
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME_LINE, &g_pD3DTexCapLine);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCap()
{
	if (g_pD3DVtxBuffCap)
	{
		g_pD3DVtxBuffCap->Release();
		g_pD3DVtxBuffCap = NULL;
	}

	if (g_pD3DTexCap)
	{
		g_pD3DTexCap->Release();
		g_pD3DTexCap = NULL;
	}

	if (g_pD3DTexCapLine)
	{
		g_pD3DTexCapLine->Release();
		g_pD3DTexCapLine = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCap()
{
	//static D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 scl = (GetSeizaEffect() + 9)->scl;
	D3DXVECTOR3 pos = (GetSeizaEffect() + 9)->pos;

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

	for (int i = 0; i < g_Cap.Nv; i++)
	{
		D3DXVec3TransformCoord(&g_Cap.star[i].pos, &posBuff[i], &mtxScale);//S
		D3DXVec3TransformCoord(&g_Cap.star[i].pos, &g_Cap.star[i].pos, &mtxRot);//R
		D3DXVec3TransformCoord(&g_Cap.star[i].pos, &g_Cap.star[i].pos, &mtxTran);//T

	}

	// 星の番号により、線分を設定
	SetLine(&g_Cap, 0, 1);
	SetLine(&g_Cap, 1, 2);
	SetLine(&g_Cap, 1, 8);
	SetLine(&g_Cap, 2, 3);
	SetLine(&g_Cap, 3, 4);
	SetLine(&g_Cap, 4, 5);
	SetLine(&g_Cap, 5, 6);
	SetLine(&g_Cap, 5, 7);
	SetLine(&g_Cap, 7, 8);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	UpdateSeiza(&g_Cap, SCORE);

	SetVtxColCap(pDevice);

	//PrintDebugProc("Cap: scl(%f,%f) pos(%f,%f)\n\n", scl.x, scl.y, pos.x, pos.y);

	//for (int i = 0; i < g_Cap.Nv; i++)
	//{
	//	PrintDebugProc("Cap[%d]: pos(%f,%f)\n", i, g_Cap.star[i].pos.x, g_Cap.star[i].pos.y);
	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCap()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 線分を描画------------------------------------------------------------------------------------
	// render state
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		D3DXMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;
		if (g_Cap.line[i].isLight)
		{
			D3DXMatrixIdentity(&mtxWorld);

			// 拡大縮小
			D3DXMatrixScaling(&mtxScale, g_Cap.line[i].scale.x, g_Cap.line[i].scale.y, g_Cap.line[i].scale.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Cap.line[i].rot);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Cap.line[i].pos.x, g_Cap.line[i].pos.y, g_Cap.line[i].pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 例の設定
			pDevice->SetStreamSource(0, g_pD3DVtxBuffCapLine, 0, sizeof(VERTEX_3D));
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pD3DTexCapLine);

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
		D3DXMatrixScaling(&mtxScale, g_Cap.star[i].scale.x, g_Cap.star[i].scale.y, g_Cap.star[i].scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Cap.star[i].rot);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 移動を反映		
		D3DXMatrixTranslation(&mtxTranslate, g_Cap.star[i].pos.x, g_Cap.star[i].pos.y, g_Cap.star[i].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 例の設定
		pDevice->SetStreamSource(0, g_pD3DVtxBuffCap, 0, sizeof(VERTEX_3D));
		pDevice->SetFVF(FVF_VERTEX_3D);
		pDevice->SetTexture(0, g_pD3DTexCap);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVtxCap(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_STAR,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffCap,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffCap->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffCap->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
HRESULT MakeVtxCapLine(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SIZE_ARRAY,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffCapLine,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffCapLine->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffCapLine->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
void SetVtxColCap(LPDIRECT3DDEVICE9 pDevice)
{
	// 色の更新		
	VERTEX_3D *pVtx;
	g_pD3DVtxBuffCap->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_STAR; i++, pVtx += NUM_VERTEX)
	{
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = g_Cap.star[i].color;

		g_pD3DVtxBuffCap->Unlock();
	}
}

//=============================================================================
// 
//=============================================================================
SEIZA* GetCap()
{
	return &g_Cap;
}

