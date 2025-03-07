//=============================================================================
//
// ライト処理 [light.cpp]
// Author : Huading Zhou
//
//=============================================================================
#include "light.h"


//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXVECTOR3 g_lightDir;
D3DXCOLOR   g_lightCol;

//=============================================================================
// ライトの初期化処理
//=============================================================================
HRESULT InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_lightDir = D3DXVECTOR3(0.707f, -0.707f, 0.707f);//右下後
	//g_lightDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	g_lightCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = InitDirectionalLight(&g_lightDir, &g_lightCol);

	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, true);
	
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

//=============================================================================
// 平行光の初期化処理
//=============================================================================
D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.4f;
	light.Direction = *direction;

	return light;
}

//=============================================================================
// ポイントライトの初期化処理
//=============================================================================
D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

//=============================================================================
// スポットライトの初期化処理
//=============================================================================
D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.2f;
	light.Position = *position;
	light.Direction = *direction;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta = 0.5f;
	light.Phi = 0.7f;

	return light;
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void UpdateLight(void)
{
}

//=============================================================================
// 
//=============================================================================
D3DXVECTOR3 getLightDir()
{
	return g_lightDir;
}