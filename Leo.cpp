#include "Leo.h"
#include "score_effect.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FILENAME		"data/TEXTURE/star.png"		// 星のテクスチャ
#define TEX_FILENAME_LINE	"data/TEXTURE/line.png"		// 線分のテクスチャ
#define	NUM_STAR			(15)						// 獅子座の頂点数
#define NUM_EDGE			(15)						// 獅子座の辺数
#define SIZE_ARRAY			(NUM_STAR*(NUM_STAR+1)/2)	// 配列のサイズ
#define SCORE				(200)						// 配列のサイズ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVtxLeo(LPDIRECT3DDEVICE9 pDevice);			// 星の頂点をつくる
HRESULT MakeVtxLeoLine(LPDIRECT3DDEVICE9 pDevice);		// 線分の頂点をつくる	
void SetVtxColLeo(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffLeo;		// 星の頂点バッファ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffLeoLine;	// 線分の頂点バッファ
LPDIRECT3DTEXTURE9		g_pD3DTexLeo;			// 星のテクスチャ
LPDIRECT3DTEXTURE9		g_pD3DTexLeoLine;		// 線分のテクスチャ
SEIZA					g_Leo;					// 獅子座の構造体へのポインター

// 獅子座の頂点情報
D3DXVECTOR3	g_posLeo[NUM_STAR] =
{
	{ 6.9f,  3.7f, 0.0f },	
	{4.85f ,  3.94f , 0.0f},		// 1
	{3.47f ,  5.31f , 0.0f},		// 2
	{1.46f , 3.59f , 0.0f},		// 3
	{1.22f , 2.19f , 0.0f},		// 4
	{2.6f , 0.81f , 0.0f},		// 5
	{3.0f , -1.98f , 0.0f},		// 6
	{0.65f , -3.71f , 0.0f},		// 7
	{6.08f ,  -3.04f , 0.0f},		// 8
	{-4.22f, 1.8f  , 0.0f},		// 9
	{-3.48f , -0.99f , 0.0f},		// 10
	{-4.8f , -3.26f , 0.0f},		// 11
	{-4.08f , -5.20f , 0.0f},		// 12
	{-4.86f , -6.9f , 0.0f},		// 13
	{-7.01f ,  -1.34f, 0.0f},	// 14
};

static D3DXVECTOR3 posBuff[NUM_STAR];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLeo()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Leo.name = LEO;
	g_Leo.belong = DEFAULT;

	// 星座構造体の初期化
	InitSeiZa(&g_Leo, NUM_STAR, NUM_EDGE);

	// 星の初期化
	for (int i = 0; i < g_Leo.Nv; i++)
	{
		g_Leo.star[i].pos = g_posLeo[i];
		g_Leo.star[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Leo.star[i].color = HALF_WHITE;
		g_Leo.star[i].radius = SIZE_STAR / 4.0f;
		g_Leo.star[i].fade = GetRandomFloat(0.002f, 0.006f);
		g_Leo.star[i].rot = GetRandomFloat(0.0f, 6.28f);
		g_Leo.star[i].isLight = false;
		g_Leo.star[i].effectIdx = -1;

		posBuff[i] = g_Leo.star[i].pos;
	}

	// 線分の初期化
	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		g_Leo.G[i] = 0;
		g_Leo.line[i].isLight = false;
		g_Leo.line[i].rot = 0.0f;
		g_Leo.line[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Leo.line[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Leo.line[i].color = WHITE;
	}

	//// 星の番号により、線分を設定
	//SetLine(&g_Leo, 0, 1);
	//SetLine(&g_Leo, 1, 2);
	//SetLine(&g_Leo, 2, 3);
	//SetLine(&g_Leo, 3, 4);
	//SetLine(&g_Leo, 4, 5);
	//SetLine(&g_Leo, 5, 6);
	//SetLine(&g_Leo, 5, 9);
	//SetLine(&g_Leo, 6, 7);
	//SetLine(&g_Leo, 6, 10);
	//SetLine(&g_Leo, 7, 8);
	//SetLine(&g_Leo, 9, 10);
	//SetLine(&g_Leo, 9, 14);
	//SetLine(&g_Leo, 10, 11);
	//SetLine(&g_Leo, 10, 14);
	//SetLine(&g_Leo, 11, 12);
	//SetLine(&g_Leo, 12, 13);

	// 頂点をつくる
	MakeVtxLeo(pDevice);
	MakeVtxLeoLine(pDevice);

	// load textures
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME, &g_pD3DTexLeo);
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME_LINE, &g_pD3DTexLeoLine);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLeo()
{
	if (g_pD3DVtxBuffLeo)
	{
		g_pD3DVtxBuffLeo->Release();
		g_pD3DVtxBuffLeo = NULL;
	}

	if (g_pD3DTexLeo)
	{
		g_pD3DTexLeo->Release();
		g_pD3DTexLeo = NULL;
	}

	if (g_pD3DTexLeoLine)
	{
		g_pD3DTexLeoLine->Release();
		g_pD3DTexLeoLine = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLeo()
{	
	//static D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	static D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 scl = (GetSeizaEffect() + 4)->scl;
	D3DXVECTOR3 pos = (GetSeizaEffect() + 4)->pos;

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

	for (int i = 0; i < g_Leo.Nv; i++)
	{
		D3DXVec3TransformCoord(&g_Leo.star[i].pos, &posBuff[i], &mtxScale);//S
		D3DXVec3TransformCoord(&g_Leo.star[i].pos, &g_Leo.star[i].pos, &mtxRot);//R
		D3DXVec3TransformCoord(&g_Leo.star[i].pos, &g_Leo.star[i].pos, &mtxTran);//T

	}

	// 星の番号により、線分を設定
	SetLine(&g_Leo, 0, 1);
	SetLine(&g_Leo, 1, 2);
	SetLine(&g_Leo, 2, 3);
	SetLine(&g_Leo, 3, 4);
	SetLine(&g_Leo, 4, 5);
	SetLine(&g_Leo, 5, 6);
	SetLine(&g_Leo, 5, 9);
	SetLine(&g_Leo, 6, 7);
	SetLine(&g_Leo, 6, 10);
	SetLine(&g_Leo, 7, 8);
	SetLine(&g_Leo, 9, 10);
	SetLine(&g_Leo, 9, 14);
	SetLine(&g_Leo, 10, 11);
	SetLine(&g_Leo, 10, 14);
	SetLine(&g_Leo, 11, 12);
	SetLine(&g_Leo, 12, 13);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	UpdateSeiza(&g_Leo, SCORE);

	SetVtxColLeo(pDevice);

	//PrintDebugProc("Leo: scl(%f,%f) pos(%f,%f)\n\n", scl.x, scl.y, pos.x, pos.y);

	//for (int i = 0; i < g_Leo.Nv; i++)
	//{
	//	PrintDebugProc("Leo[%d]: pos(%f,%f)\n", i, g_Leo.star[i].pos.x, g_Leo.star[i].pos.y);
	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLeo()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 線分を描画------------------------------------------------------------------------------------
	// render state
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < SIZE_ARRAY; i++)
	{
		D3DXMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;
		if (g_Leo.line[i].isLight)
		{
			D3DXMatrixIdentity(&mtxWorld);

			// 拡大縮小
			D3DXMatrixScaling(&mtxScale, g_Leo.line[i].scale.x, g_Leo.line[i].scale.y, g_Leo.line[i].scale.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Leo.line[i].rot);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Leo.line[i].pos.x, g_Leo.line[i].pos.y, g_Leo.line[i].pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 例の設定
			pDevice->SetStreamSource(0, g_pD3DVtxBuffLeoLine, 0, sizeof(VERTEX_3D));
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pD3DTexLeoLine);

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
		D3DXMatrixScaling(&mtxScale, g_Leo.star[i].scale.x, g_Leo.star[i].scale.y, g_Leo.star[i].scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_Leo.star[i].rot);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 移動を反映		
		D3DXMatrixTranslation(&mtxTranslate, g_Leo.star[i].pos.x, g_Leo.star[i].pos.y, g_Leo.star[i].pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// 例の設定
		pDevice->SetStreamSource(0, g_pD3DVtxBuffLeo, 0, sizeof(VERTEX_3D));
		pDevice->SetFVF(FVF_VERTEX_3D);
		pDevice->SetTexture(0, g_pD3DTexLeo);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * NUM_VERTEX, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVtxLeo(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_STAR,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffLeo,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffLeo->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffLeo->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
HRESULT MakeVtxLeoLine(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SIZE_ARRAY,
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,
											D3DPOOL_MANAGED,
											&g_pD3DVtxBuffLeoLine,
											NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffLeoLine->Lock(0, 0, (void**)&pVtx, 0);

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

		g_pD3DVtxBuffLeoLine->Unlock();

		return S_OK;
	}
}

//=============================================================================
// 
//=============================================================================
void SetVtxColLeo(LPDIRECT3DDEVICE9 pDevice)
{
	// 色の更新		
	VERTEX_3D *pVtx;
	g_pD3DVtxBuffLeo->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_STAR; i++, pVtx += NUM_VERTEX)
	{
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = g_Leo.star[i].color;

		g_pD3DVtxBuffLeo->Unlock();
	}
}

//=============================================================================
// 
//=============================================================================
SEIZA* GetLeo()
{
	return &g_Leo;
}

