//=============================================================================
//
// スコアエフェクト処理 [score_effect.cpp]
// Author : 増田　光汰
//
//=============================================================================
#include "score_effect.h"



//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/explosionFlare.png"	// 読み込むテクスチャファイル名

#define	PARTICLE_SIZE_X		(5.0f)								// ビルボードの幅
#define	PARTICLE_SIZE_Y		(5.0f)								// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE	(2.0f)								// 移動速度

#define BEZ_CONTROL1_XZ		((float)(rand() % 1200) / 10)//0-119.9
#define BEZ_CONTROL2_XZ		((float)(rand() % 1200) / 10)//0-119.9
#define	BEZ_CONTROL2_Y		(0.0f)
#define	BEZ_CONTROL1_Y		(0.0f)

#define	MAX_PARTICLE		(256)								// ビルボード最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBezier_Particle(LPDIRECT3DDEVICE9 Device);
void SetVertexBezier_Particle(int nIdxBullet, float fSizeX, float fSizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureParticle = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffParticle = NULL;			// 頂点バッファインターフェースへのポインタ
D3DXMATRIX				mtxWorldParticle;					// ワールドマトリックス

BEZIER_PARTICLEH		Bezier_Particle[MAX_PARTICLE];		// パーティクルワーク
static int				PTAlpha;							// アルファテストの閾値

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBezier_Particle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の作成
	MakeVertexBezier_Particle(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,		// デバイスへのポインタ
		TEXTURE_PARTICLE,					// ファイルの名前
		&D3DTextureParticle);				// 読み込むメモリー

	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{
		Bezier_Particle[CntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bezier_Particle[CntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bezier_Particle[CntParticle].scale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
		Bezier_Particle[CntParticle].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Bezier_Particle[CntParticle].fSizeX = PARTICLE_SIZE_X;
		Bezier_Particle[CntParticle].fSizeY = PARTICLE_SIZE_Y;
		Bezier_Particle[CntParticle].bUse = false;
	}

	return S_OK;

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBezier_Particle(void)
{
	if (D3DTextureParticle != NULL)
	{// テクスチャの開放
		D3DTextureParticle->Release();
		D3DTextureParticle = NULL;
	}

	if (D3DVtxBuffParticle != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffParticle->Release();
		D3DVtxBuffParticle = NULL;
	}

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBezier_Particle(void)
{
	/*ここからスクリーン座標変換のプログラム*/

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビュー行列、プロジェクション行列、ビューポート行列
	D3DXMATRIX mtxView, mtxProjection, mtxViewPort;

	//ビューポート行列を作成
	D3DXMatrixIdentity(&mtxViewPort);									//初期化
	mtxViewPort._11 = SCREEN_WIDTH / 2.0f;								//各成分をスクリーンサイズから計算
	mtxViewPort._22 = -SCREEN_HEIGHT / 2.0f;
	mtxViewPort._41 = SCREEN_WIDTH / 2.0f;
	mtxViewPort._42 = SCREEN_HEIGHT / 2.0f;

	//プロジェクション行列取得
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);

	//ビュー行列取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ビュー逆行列、プロジェクション逆行列、ビューポート行列
	D3DXMATRIX invView, invProjection, invViewport;

	//逆行列を作成
	D3DXMatrixInverse(&invView, NULL, &mtxView);
	D3DXMatrixInverse(&invProjection, NULL, &mtxProjection);
	D3DXMatrixInverse(&invViewport, NULL, &mtxViewPort);

	// スコアのスクリーン座標に置き換える
	D3DXVECTOR3 screenPos = D3DXVECTOR3(940.0f, 80.0f, 0.0f);					//D3DXVECTOR3型に変換

	//POINT cursor;
	//GetCursorPos(&cursor);												//カーソルのスクリーン座標を取得
	//D3DXVECTOR3 screenPos = D3DXVECTOR3(cursor.x, cursor.y, 0.0f);		//D3DXVECTOR3型に変換

	//カーソルのスクリーン座標をワールド座標に変換
	D3DXVECTOR3 worldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);												//変換後のワールド座標を格納する変数
	D3DXMATRIX temp = invViewport * invProjection * invView;			//変換する行列を作成
	D3DXVec3TransformCoord(&worldPos, &screenPos, &temp);				//スクリーン座標を変換行列でワールド座標へ変換


	// ベジェ曲線の計算式
	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{
		if (Bezier_Particle[CntParticle].bUse)
		{
			// カウントフレームをインクリメント
			Bezier_Particle[CntParticle].cntFrame++;
			// カウントクリックをインクリメント
			Bezier_Particle[CntParticle].cntclick++;
			// 到達フレームの処理(1秒の場合60.0f)
			float t = (float)Bezier_Particle[CntParticle].cntFrame / Bezier_Particle[CntParticle].frame;
			//n乗を求める関数 powf(1-t, n))
			D3DXVECTOR3 p0 = (1 - t) * (1 - t) * (1 - t) * Bezier_Particle[CntParticle].start;	// 星座の位置からスタート
			D3DXVECTOR3 p1 = 3 * (1 - t) * (1 - t) * t * Bezier_Particle[CntParticle].control1;	// コントロール1(曲がり具合の計算)
			D3DXVECTOR3 p2 = 3 * (1 - t) * t * t * Bezier_Particle[CntParticle].control2;		// コントロール2(曲がり具合の計算)
			D3DXVECTOR3 p3 = t * t * t * worldPos;												// 終点(ここでスコアの座標に行く)
			Bezier_Particle[CntParticle].pos = p0 + p1 + p2 + p3;

			// ここでスコアのスクリーン座標へ
			if (Bezier_Particle[CntParticle].pos == worldPos)
			{
				// 未使用にして消す
				Bezier_Particle[CntParticle].bUse = false;

			}
		}
	}
	//SetBezier_Particle(D3DXVECTOR3(0.0f, 0.0f, 0.0f));


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBezier_Particle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;


	// ライティングを無効に
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	//加算合成
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// 結果 = 転送先(DEST) - 転送元(SRC)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Z比較なし
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);


	// αテストを有効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, PTAlpha);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{
		if (Bezier_Particle[CntParticle].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldParticle);

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, Bezier_Particle[CntParticle].scale.x, Bezier_Particle[CntParticle].scale.y, Bezier_Particle[CntParticle].scale.z);
			D3DXMatrixMultiply(&mtxWorldParticle, &mtxWorldParticle, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, Bezier_Particle[CntParticle].pos.x, Bezier_Particle[CntParticle].pos.y, Bezier_Particle[CntParticle].pos.z);
			D3DXMatrixMultiply(&mtxWorldParticle, &mtxWorldParticle, &mtxTranslate);

			// ワールドマトリックスの設定
			Device->SetTransform(D3DTS_WORLD, &mtxWorldParticle);

			// 頂点バッファをデバイスのデータストリームにバインド
			Device->SetStreamSource(0, D3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureParticle);

			// ポリゴンの描画
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntParticle * NUM_VERTEX), NUM_POLYGON);
		}
	}

	// ライティングを有効に
	//Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//Z比較を戻す
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBezier_Particle(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffParticle,						// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += NUM_VERTEX)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		D3DVtxBuffParticle->Unlock();
	}

	return S_OK;

}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBezier_Particle(int nIdxParticle, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * NUM_VERTEX);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffParticle->Unlock();
	}

}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorBezier_Particle(int nIdxParticle, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * NUM_VERTEX);

		// 頂点座標の設定
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		D3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成 引数　発生させたい位置
//=============================================================================
void SetBezier_Particle(D3DXVECTOR3 pos)
{
	//未使用を目指す
	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{

		if (!Bezier_Particle[CntParticle].bUse)
		{
			// 発生させたい位置に設定
			Bezier_Particle[CntParticle].start = pos;
			// 軌道の設定
			Bezier_Particle[CntParticle].control1 = D3DXVECTOR3(BEZ_CONTROL1_XZ, BEZ_CONTROL1_XZ, BEZ_CONTROL2_Y) + pos;
			// 軌道の設定2
			//Bezier_Particle[CntParticle].control2 = D3DXVECTOR3(BEZ_CONTROL2_XZ, BEZ_CONTROL1_XZ, BEZ_CONTROL2_Y) + pos;
			Bezier_Particle[CntParticle].control2 = D3DXVECTOR3(10.0f, 50.0f, 0.0f) + pos;//集まる
			// カウントフレームの初期化
			Bezier_Particle[CntParticle].cntFrame = 0;
			// カウントクリックの初期化
			Bezier_Particle[CntParticle].cntclick = 0;
			// 使用中か、未使用か
			Bezier_Particle[CntParticle].bUse = true;
			// 何秒後に到達するか設定
			Bezier_Particle[CntParticle].frame = (rand() % 50) + 80;//80~129
			return;

		}
	}
}
//=============================================================================
// 関数取得
//=============================================================================
BEZIER_PARTICLEH *GetBezier_Particle(void)
{
	return Bezier_Particle;
}
