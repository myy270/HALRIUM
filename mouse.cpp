#include "mouse.h"
#include "starEffect.h"
#include "seizaEffect.h"
#include "score.h"
#include "score_effect.h"
#include "sound.h"
#include "debugproc.h"
#include "timer.h"

#include "Aries.h"
#include "Taurus.h"
#include "Gemini.h"
#include "Cancer.h"
#include "Leo.h"
#include "Virgo.h"
#include "Libra.h"
#include "Scorpio.h"
#include "Sagittarius.h"
#include "Capricorn.h"
#include "Aquarius.h"
#include "Pisces.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FILENAME_MOUSE	"data/TEXTURE/mouse_01.dds"
#define SIZE_MOUSE			(10.0f)
#define UNIT_MOVE			(0.23f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexMouse(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffMouse;
LPDIRECT3DTEXTURE9		g_pD3DTexMouse;
static D3DXMATRIX		g_mtxWorld;
MOUSE					g_mouse[MAX_MOUSE];		// マウス
SEIZA*					g_Seiza[MAX_SEIZA];	// 各星座へのポインターの配列

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMouse()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MakeVertexMouse(pDevice);

	// P1の初期化
	g_mouse[0].tag = DEFAULT;	// 何も選択していない
	g_mouse[0].pos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	g_mouse[0].use = true;
	// P2の初期化
	g_mouse[1].tag = DEFAULT;
	g_mouse[1].pos = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	g_mouse[1].use = true;
	D3DXCreateTextureFromFile(pDevice, TEX_FILENAME_MOUSE, &g_pD3DTexMouse);

	// ポインター配列の初期化
	for (int i = 0; i < MAX_SEIZA; i++)
	{
		g_Seiza[i] = NULL;
	}

	// 星座のデータを取得
	g_Seiza[ARI] = GetAri();
	g_Seiza[TAU] = GetTau();
	g_Seiza[GEM] = GetGem();
	g_Seiza[CAN] = GetCan();
	g_Seiza[LEO] = GetLeo();
	g_Seiza[VIR] = GetVir();
	g_Seiza[LIB] = GetLib();
	g_Seiza[SCO] = GetSco();
	g_Seiza[SAG] = GetSag();
	g_Seiza[CAP] = GetCap();
	g_Seiza[AQU] = GetAqu();
	g_Seiza[PIS] = GetPis();

	g_Seiza[LEO] = GetLeo();	// 獅子座
	g_Seiza[LIB] = GetLib();	// 天秤座
	g_Seiza[SCO] = GetSco();	// さそり座
	








	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMouse2()
{
	if (g_pD3DVtxBuffMouse)
	{
		g_pD3DVtxBuffMouse->Release();
		g_pD3DVtxBuffMouse = NULL;
	}

	if (g_pD3DTexMouse)
	{
		g_pD3DTexMouse->Release();
		g_pD3DTexMouse = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMouse()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	if (!GetTimeOut())
	{
		// P1の更新処理-------------------------------------------------------------
		if (GetKeyboardPress(DIK_SPACE) || IsButtonPress(0, BUTTON_CIRCLE))
		{
			bool isHit = false;
			for (int idxSeiza = 0; idxSeiza < MAX_SEIZA; idxSeiza++)
			{
				if (g_Seiza[idxSeiza] != NULL)
				{
					SEIZA* pSeiza = g_Seiza[idxSeiza];
					if (pSeiza->isConnect)
					{
						continue;
					}

					for (int idxStar = 0; idxStar < pSeiza->Nv; idxStar++)
					{
						// マウスと星の距離を計算、星の半径より小さいなら、選択られる
						float distance = GetDistance(&g_mouse[0].pos, &pSeiza->star[idxStar].pos);

						if (distance <= pSeiza->star[idxStar].radius)
						{
							// 二重ループをブロックする
							isHit = true;

							if ((pSeiza->belong == DEFAULT || pSeiza->belong == PLAYER_ONE) &&
								(g_mouse[0].tag == DEFAULT || g_mouse[0].tag == pSeiza->name))
							{
								// 選択した星の星座を記録
								g_mouse[0].tag = pSeiza->name;
								if (!pSeiza->star[idxStar].isLight) // 星が光っていない場合
								{

									pSeiza->star[idxStar].isLight = true;	// 光る
									pSeiza->star[idxStar].color = WHITE;
									pSeiza->belong = PLAYER_ONE;	// 星座がP1に所属、P2が選択できない

									pSeiza->star[idxStar].effectIdx = SetStarEffect(pSeiza->star[idxStar].pos, 0);//0は赤

									ChangeScore2(10);
									PlaySound(SOUND_LABEL_SE_SELECT, XAUDIO2_COMMIT_NOW);

									//for (int i = 0; i < 10; i++)
									//{
									//	SetBezier_Particle(pSeiza->star[idxStar].pos);
									//}


								}
								else
								{//キャンセル機能
									//pSeiza->star[idxStar].isLight = false;
									//pSeiza->star[idxStar].color = HALF_WHITE;
								}

							}
							else // 選択が間違えた場合
							{
								// 二重ループをブロックする
								isHit = true;

								if (g_mouse[0].tag != DEFAULT)
								{
									if (!g_Seiza[g_mouse[0].tag]->isConnect)
									{
										// 元の星座を消す
										g_Seiza[g_mouse[0].tag]->belong = DEFAULT;
										for (int i = 0; i < g_Seiza[g_mouse[0].tag]->Nv; i++)
										{
											g_Seiza[g_mouse[0].tag]->star[i].isLight = false;
											g_Seiza[g_mouse[0].tag]->star[i].color = HALF_WHITE;

											DelStarEffect(g_Seiza[g_mouse[0].tag]->star[i].effectIdx);
											g_Seiza[g_mouse[0].tag]->star[i].effectIdx = -1;//!!!重要									
										}
										ChangeScore2(-GetScore2() / 2);
									}
									else
									{
										ChangeScore2(10);
										PlaySound(SOUND_LABEL_SE_SELECT, XAUDIO2_COMMIT_NOW);
									}

									g_mouse[0].tag = DEFAULT;

									// 今選択した星座がディフォルトの場合
									if (pSeiza->belong == DEFAULT)
									{
										pSeiza->star[idxStar].isLight = true;
										pSeiza->star[idxStar].color = WHITE;
										pSeiza->belong = PLAYER_ONE;
										g_mouse[0].tag = pSeiza->name;

										pSeiza->star[idxStar].effectIdx = SetStarEffect(pSeiza->star[idxStar].pos, 0);//0は赤

									}
								}

							}
							break;

						}
					}
				}
				if (isHit) break;
			}
		}

		if (GetKeyboardPress(DIK_A) || IsButtonPress(0, BUTTON_LEFT) || IsButtonPress(0, BUTTON_LSTICK_LEFT))
		{
			if (GetKeyboardPress(DIK_W) || IsButtonPress(0, BUTTON_UP) || IsButtonPress(0, BUTTON_LSTICK_UP))
			{// 左前移動
				g_mouse[0].pos.x -= sinf(D3DX_PI * 0.75f) * UNIT_MOVE;
				g_mouse[0].pos.y -= cosf(D3DX_PI * 0.75f) * UNIT_MOVE;

			}
			else if (GetKeyboardPress(DIK_S) || IsButtonPress(0, BUTTON_DOWN) || IsButtonPress(0, BUTTON_LSTICK_DOWN))
			{// 左後移動
				g_mouse[0].pos.x -= sinf(D3DX_PI * 0.25f) * UNIT_MOVE;
				g_mouse[0].pos.y -= cosf(D3DX_PI * 0.25f) * UNIT_MOVE;

			}
			else
			{// 左移動
				g_mouse[0].pos.x -= sinf(D3DX_PI * 0.50f) * UNIT_MOVE;
				g_mouse[0].pos.y -= cosf(D3DX_PI * 0.50f) * UNIT_MOVE;

			}
		}
		else if (GetKeyboardPress(DIK_D) || IsButtonPress(0, BUTTON_RIGHT) || IsButtonPress(0, BUTTON_LSTICK_RIGHT))
		{

			if (GetKeyboardPress(DIK_W) || IsButtonPress(0, BUTTON_UP) || IsButtonPress(0, BUTTON_LSTICK_UP))
			{// 右前移動
				g_mouse[0].pos.x -= sinf(-D3DX_PI * 0.75f) * UNIT_MOVE;
				g_mouse[0].pos.y -= cosf(-D3DX_PI * 0.75f) * UNIT_MOVE;

			}
			else if (GetKeyboardPress(DIK_S) || IsButtonPress(0, BUTTON_DOWN) || IsButtonPress(0, BUTTON_LSTICK_DOWN))
			{// 右後移動
				g_mouse[0].pos.x -= sinf(-D3DX_PI * 0.25f) * UNIT_MOVE;
				g_mouse[0].pos.y -= cosf(-D3DX_PI * 0.25f) * UNIT_MOVE;

			}
			else
			{// 右移動
				g_mouse[0].pos.x -= sinf(-D3DX_PI * 0.50f) * UNIT_MOVE;
				g_mouse[0].pos.y -= cosf(-D3DX_PI * 0.50f) * UNIT_MOVE;

			}
		}
		else if (GetKeyboardPress(DIK_W) || IsButtonPress(0, BUTTON_UP) || IsButtonPress(0, BUTTON_LSTICK_UP))
		{

			// 前移動
			g_mouse[0].pos.x -= sinf(D3DX_PI) * UNIT_MOVE;
			g_mouse[0].pos.y -= cosf(D3DX_PI) * UNIT_MOVE;

		}
		else if (GetKeyboardPress(DIK_S) || IsButtonPress(0, BUTTON_DOWN) || IsButtonPress(0, BUTTON_LSTICK_DOWN))
		{

			// 後移動
			g_mouse[0].pos.x -= sinf(0.0f) * UNIT_MOVE;
			g_mouse[0].pos.y -= cosf(0.0f) * UNIT_MOVE;

		}


		//マウスの移動範囲（仮に）
		if (g_mouse[0].pos.x < -40.2f)
		{
			g_mouse[0].pos.x = -40.2f;
		}
		if (g_mouse[0].pos.x > 40.2f)
		{
			g_mouse[0].pos.x = 40.2f;
		}
		if (g_mouse[0].pos.y < -40.0f)
		{
			g_mouse[0].pos.y = -40.0f;
		}
		if (g_mouse[0].pos.y > 40.0f)
		{
			g_mouse[0].pos.y = 40.0f;
		}


		// P2の更新処理-------------------------------------------------------------
		if (GetKeyboardPress(DIK_NUMPAD1) || IsButtonPress(1, BUTTON_CIRCLE))
		{
			bool isHit = false;
			for (int idxSeiza = 0; idxSeiza < MAX_SEIZA; idxSeiza++)
			{
				if (g_Seiza[idxSeiza] != NULL)
				{
					SEIZA* pSeiza = g_Seiza[idxSeiza];
					if (pSeiza->isConnect)
					{
						continue;
					}

					for (int idxStar = 0; idxStar < pSeiza->Nv; idxStar++)
					{
						// マウスと星の距離を計算、星の半径より小さいなら、選択られる
						float distance = GetDistance(&g_mouse[1].pos, &pSeiza->star[idxStar].pos);

						if (distance <= pSeiza->star[idxStar].radius)
						{
							// 二重ループをブロックする
							isHit = true;

							if ((pSeiza->belong == DEFAULT || pSeiza->belong == PLAYER_TWO) &&
								(g_mouse[1].tag == DEFAULT || g_mouse[1].tag == pSeiza->name))
							{
								// 選択した星の星座を記録
								g_mouse[1].tag = pSeiza->name;
								if (!pSeiza->star[idxStar].isLight) // 星が光っていない場合
								{
									pSeiza->star[idxStar].isLight = true;	// 光る
									pSeiza->star[idxStar].color = WHITE;
									pSeiza->belong = PLAYER_TWO;	// 星座がP2に所属、P1が選択できない

									pSeiza->star[idxStar].effectIdx = SetStarEffect(pSeiza->star[idxStar].pos, 1);//1は青

									ChangeScore(10);
									PlaySound(SOUND_LABEL_SE_SELECT, XAUDIO2_COMMIT_NOW);
								}
								else
								{//キャンセル機能
									//pSeiza->star[idxStar].isLight = false;
									//pSeiza->star[idxStar].color = HALF_WHITE;
								}

							}
							else // 選択が間違えた場合
							{//マウスがデファルトで、他人の星座をクリックする時、ifを実行しない
							//マウスがすでに一つの星座をクリックしてる状態で、他の星座をクリックする時、ifを実行する
								if (g_mouse[1].tag != DEFAULT)
								{
									if (!g_Seiza[g_mouse[1].tag]->isConnect)
									{
										// 元の星座を消す
										g_Seiza[g_mouse[1].tag]->belong = DEFAULT;
										for (int i = 0; i < g_Seiza[g_mouse[1].tag]->Nv; i++)
										{
											g_Seiza[g_mouse[1].tag]->star[i].isLight = false;
											g_Seiza[g_mouse[1].tag]->star[i].color = HALF_WHITE;

											DelStarEffect(g_Seiza[g_mouse[1].tag]->star[i].effectIdx);
											g_Seiza[g_mouse[1].tag]->star[i].effectIdx = -1;//!!!重要

										}
										ChangeScore(-GetScore() / 2);
									}
									else
									{
										ChangeScore(10);
										PlaySound(SOUND_LABEL_SE_SELECT, XAUDIO2_COMMIT_NOW);
									}

									g_mouse[1].tag = DEFAULT;

									// 今選択した星座がディフォルトの場合
									if (pSeiza->belong == DEFAULT)
									{
										pSeiza->star[idxStar].isLight = true;
										pSeiza->star[idxStar].color = WHITE;
										pSeiza->belong = PLAYER_TWO;
										g_mouse[1].tag = pSeiza->name;

										pSeiza->star[idxStar].effectIdx = SetStarEffect(pSeiza->star[idxStar].pos, 1);//1は青

									}

								}
							}

							break;
						}
					}
				}
				if (isHit) break;
			}
		}

		if (GetKeyboardPress(DIK_LEFT) || IsButtonPress(1, BUTTON_LEFT) || IsButtonPress(1, BUTTON_LSTICK_LEFT))
		{
			if (GetKeyboardPress(DIK_UP) || IsButtonPress(1, BUTTON_UP) || IsButtonPress(1, BUTTON_LSTICK_UP))
			{// 左前移動
				g_mouse[1].pos.x -= sinf(D3DX_PI * 0.75f) * UNIT_MOVE;
				g_mouse[1].pos.y -= cosf(D3DX_PI * 0.75f) * UNIT_MOVE;

			}
			else if (GetKeyboardPress(DIK_DOWN) || IsButtonPress(1, BUTTON_DOWN) || IsButtonPress(1, BUTTON_LSTICK_DOWN))
			{// 左後移動
				g_mouse[1].pos.x -= sinf(D3DX_PI * 0.25f) * UNIT_MOVE;
				g_mouse[1].pos.y -= cosf(D3DX_PI * 0.25f) * UNIT_MOVE;

			}
			else
			{// 左移動
				g_mouse[1].pos.x -= sinf(D3DX_PI * 0.50f) * UNIT_MOVE;
				g_mouse[1].pos.y -= cosf(D3DX_PI * 0.50f) * UNIT_MOVE;

			}
		}
		else if (GetKeyboardPress(DIK_RIGHT) || IsButtonPress(1, BUTTON_RIGHT) || IsButtonPress(1, BUTTON_LSTICK_RIGHT))
		{
			if (GetKeyboardPress(DIK_UP) || IsButtonPress(1, BUTTON_UP) || IsButtonPress(1, BUTTON_LSTICK_UP))
			{// 右前移動
				g_mouse[1].pos.x -= sinf(-D3DX_PI * 0.75f) * UNIT_MOVE;
				g_mouse[1].pos.y -= cosf(-D3DX_PI * 0.75f) * UNIT_MOVE;

			}
			else if (GetKeyboardPress(DIK_DOWN) || IsButtonPress(1, BUTTON_DOWN) || IsButtonPress(1, BUTTON_LSTICK_DOWN))
			{// 右後移動
				g_mouse[1].pos.x -= sinf(-D3DX_PI * 0.25f) * UNIT_MOVE;
				g_mouse[1].pos.y -= cosf(-D3DX_PI * 0.25f) * UNIT_MOVE;

			}
			else
			{// 右移動
				g_mouse[1].pos.x -= sinf(-D3DX_PI * 0.50f) * UNIT_MOVE;
				g_mouse[1].pos.y -= cosf(-D3DX_PI * 0.50f) * UNIT_MOVE;

			}
		}
		else if (GetKeyboardPress(DIK_UP) || IsButtonPress(1, BUTTON_UP) || IsButtonPress(1, BUTTON_LSTICK_UP))
		{

			// 前移動
			g_mouse[1].pos.x -= sinf(D3DX_PI) * UNIT_MOVE;
			g_mouse[1].pos.y -= cosf(D3DX_PI) * UNIT_MOVE;

		}
		else if (GetKeyboardPress(DIK_DOWN) || IsButtonPress(1, BUTTON_DOWN) || IsButtonPress(1, BUTTON_LSTICK_DOWN))
		{

			// 後移動
			g_mouse[1].pos.x -= sinf(0.0f) * UNIT_MOVE;
			g_mouse[1].pos.y -= cosf(0.0f) * UNIT_MOVE;

		}


		//マウスの移動範囲（仮に）
		if (g_mouse[1].pos.x < -40.2f)
		{
			g_mouse[1].pos.x = -40.2f;
		}
		if (g_mouse[1].pos.x > 40.2f)
		{
			g_mouse[1].pos.x = 40.2f;
		}
		if (g_mouse[1].pos.y < -40.0f)
		{
			g_mouse[1].pos.y = -40.0f;
		}
		if (g_mouse[1].pos.y > 40.0f)
		{
			g_mouse[1].pos.y = 40.0f;
		}
	}


	PrintDebugProc("cursor: pos(%f,%f)\n\n\n\n", g_mouse[0].pos.x, g_mouse[0].pos.y);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMouse()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;

	for (int i = 0; i < MAX_MOUSE; i++)
	{
		if (g_mouse[i].use)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorld);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_mouse[i].pos.x, g_mouse[i].pos.y, g_mouse[i].pos.z);
			D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

			// 例の設定
			pDevice->SetStreamSource(0, g_pD3DVtxBuffMouse, 0, sizeof(VERTEX_3D));
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pD3DTexMouse);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexMouse(LPDIRECT3DDEVICE9 pDevice)
{


	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_MOUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffMouse,
		NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pD3DVtxBuffMouse->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < MAX_MOUSE; i++, pVtx += NUM_VERTEX)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-SIZE_MOUSE / 2, SIZE_MOUSE / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(SIZE_MOUSE / 2, SIZE_MOUSE / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-SIZE_MOUSE / 2, -SIZE_MOUSE / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(SIZE_MOUSE / 2, -SIZE_MOUSE / 2, 0.0f);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			pVtx[0].diffuse = RED;
			pVtx[1].diffuse = RED;
			pVtx[2].diffuse = RED;
			pVtx[3].diffuse = RED;

			if (i == 1)
			{
				pVtx[0].diffuse = BLUE;
				pVtx[1].diffuse = BLUE;
				pVtx[2].diffuse = BLUE;
				pVtx[3].diffuse = BLUE;
			}

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		g_pD3DVtxBuffMouse->Unlock();

		return S_OK;
	}
}

MOUSE* GetMouse(void)
{
	return &g_mouse[0];
}

SEIZA** GetSeiza(void)
{
	return g_Seiza;
}