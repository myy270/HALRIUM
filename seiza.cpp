//=============================================================================
//
// 星座クラス [seiza.cpp]
// Author : HEW チームE
// 
//=============================================================================
#include "seiza.h"
#include "score_effect.h"
//*****************************************************************************
// 星座構造体の初期化、初期化した構造体を利用して
// numStar : 星の数
// numEdge : 線分数
//*****************************************************************************
void InitSeiZa(SEIZA* pSeiza, int numStar, int numEdge)
{
	int sizeArray = numStar * (numStar + 1) / 2;

	pSeiza->star = (STAR*)malloc(sizeof(struct _star) * numStar);	// 星
	pSeiza->G = (int*)malloc(sizeof(int) * sizeArray);				// 配列
	pSeiza->line = (LINE*)malloc(sizeof(struct _star) * sizeArray);	// 線分
	pSeiza->Nv = numStar;		// 星の数を設定
	pSeiza->Ne = numEdge;		// 線分の数を設定
	pSeiza->isConnect = false;
}

//*****************************************************************************
// 星座の星v1とv2間の線分を設定
// seiza : 星座のポインター
// v1, v2: 星の番号
//*****************************************************************************
void SetLine(SEIZA* pSeiza, int v1, int v2)
{
	// 配列のインデックスは v2 > v1 の前提で計算
	if (v1 > v2)
	{
		// v1, v2を交換
		int temp = v1;
		v1 = v2;
		v2 = temp;
	}

	// インデックスを計算
	int index = v2 * (v2 + 1) / 2 + v1;

	//if (pSeiza->G[index] == 1)	// 既に設定した場合
	//	return;

	// 頂点 v1 と v2 の通路を設定
	pSeiza->G[index] = 1;

	// ベクトルを計算
	D3DXVECTOR3 edge = pSeiza->star[v2].pos - pSeiza->star[v1].pos;

	// ベクトルの長さで線分をスケール
	pSeiza->line[index].scale.x = D3DXVec3Length(&edge);

	// 線分の角度を計算
	D3DXVec3Normalize(&edge, &edge);
	if (edge.y > 0)
		pSeiza->line[index].rot = acosf(D3DXVec3Dot(&edge, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)));
	else
		pSeiza->line[index].rot = -acosf(D3DXVec3Dot(&edge, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)));

	// 位置を計算
	pSeiza->line[index].pos = (pSeiza->star[v2].pos + pSeiza->star[v1].pos) / 2.0f;
}

//*****************************************************************************
// 範囲内のランダム数を生成
//*****************************************************************************
float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	// [0, 1]
	float f = (float)((rand() % 10000) * 0.0001);//0~0.9999

	// [low, high]
	return (f * (highBound - lowBound)) + lowBound;
}


//*****************************************************************************
// 範囲内のランダムベクトルを生成
//*****************************************************************************
void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

//*****************************************************************************
// P1とP2の距離を計算
//*****************************************************************************
float GetDistance(D3DXVECTOR3* pV1, D3DXVECTOR3* pV2)
{
	return D3DXVec3Length(&(*pV1 - *pV2));
}


void UpdateSeiza(SEIZA* seiza, int Score)
{
	// 星の更新
	for (int i = 0; i < seiza->Nv; i++)
	{
		if ((!seiza->star[i].isLight) || (seiza->isConnect))
		{//まだ選択されってない星はきらきらする
			seiza->star[i].color.a += seiza->star[i].fade;
			if (seiza->star[i].color.a > 0.8f)
			{
				seiza->star[i].color.a = 0.8f;
			}

			if (seiza->star[i].color.a < 0.3f)
			{
				seiza->star[i].color.a = 0.3f;
			}

			if (seiza->star[i].color.a == 0.8f || seiza->star[i].color.a == 0.3f)
				seiza->star[i].fade = -seiza->star[i].fade;//変化量の方向を反転
		}
	}

	// 線分の更新
	for (int i = 0; i < seiza->Nv; i++)
	{
		for (int j = i + 1; j < seiza->Nv; j++)
		{
			// 線分のインデックスを計算
			int index = j * (j + 1) / 2 + i;

			if (seiza->G[index] == 1)	// 頂点 i,j の間通路がある場合
			{
				if (seiza->star[i].isLight && seiza->star[j].isLight)	// 線分両端の星全部選択せれた場合
				{
					seiza->line[index].isLight = true;	// 線分を描画
				}
				else
				{
					seiza->line[index].isLight = false;	// 描画しない
				}
			}
		}
	}

	// 星が全部繋がっているかどうか
	if (!seiza->isConnect)
	{
		seiza->isConnect = true;
		for (int i = 0; i < seiza->Nv; i++)
		{
			if (!seiza->star[i].isLight)
			{
				seiza->isConnect = false;
				break;
			}
		}
		if (seiza->isConnect)
		{
			//スコアボーナス
			if (seiza->belong == PLAYER_ONE)
			{
				ChangeScore2(Score);
			}
			else if (seiza->belong == PLAYER_TWO)
			{
				ChangeScore(Score);
			}

			PlaySound(SOUND_LABEL_SE_COMPLETE, XAUDIO2_NO_LOOP_REGION);

			//ベジェ曲線
			for (int i = 0; i < seiza->Nv; i++)
			{
				SetBezier_Particle(seiza->star[i].pos);
			}

		}
	}
}