//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : ���p�j
//
//=============================================================================
#include "game.h"
#include "light.h"
#include "basicScene.h"
#include "particleStar.h"
#include "mouse.h"
#include "timer.h"
#include "sound.h"

#include "starEffect.h"
#include "stack.h"
#include "seizaEffect.h"
#include "score.h"
#include "score_effect.h"

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
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	// �^�C�}�[�̏�����
	InitTimer();
	ResetTimer(999);

	{
		InitAri();
		InitTau();
		InitGem();
		InitCan();
		InitLeo();
		InitVir();
		InitLib();
		InitSco();
		InitSag();
		InitCap();
		InitAqu();
		InitPis();
	}

	InitMouse();	// �}�E�X�̏������͕K�������̌�

	InitStarEffect(0);

	InitSeizaEffect(0);//�����̌�

	InitStack(0);

	InitScore();

	InitBezier_Particle();


	// BGM�Đ� ������loop�ł���ɂ���!
	PlaySound(SOUND_LABEL_BGM001, XAUDIO2_LOOP_INFINITE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// �^�C�}�[�̏I������
	UninitTimer();

	UninitBasicScene();

	UninitParticleStar();

	UninitMouse2();

	{
		UninitAri();
		UninitTau();
		UninitGem();
		UninitCan();
		UninitLeo();
		UninitVir();
		UninitLib();
		UninitSco();
		UninitSag();
		UninitCap();
		UninitAqu();
		UninitPis();
	}

	UninitStarEffect();
	UninitSeizaEffect();
	UninitStack();
	UninitScore();
	UninitBezier_Particle();

	// BGM��~
	StopSound(SOUND_LABEL_BGM001);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	UpdateBasicScene();
	UpdateParticleStar();

	UpdateMouse();

	{
		UpdateAri();
		UpdateTau();
		UpdateGem();
		UpdateCan();
		UpdateLeo();
		UpdateVir();
		UpdateLib();
		UpdateSco();
		UpdateSag();
		UpdateCap();
		UpdateAqu();
		UpdatePis();
	}

	UpdateStarEffect();
	UpdateSeizaEffect();//�����̌��ɒu��

	UpdateStack();//�����̌��ɒu��

	UpdateScore();

	UpdateBezier_Particle();

	// �^�C�}�[�����̍X�V
	UpdateTimer();

}

//=============================================================================
// �`�揈�� �`�揇����
//=============================================================================
void DrawGame(void)
{
	DrawBasicScene();//z 80
	DrawParticleStar();//z 1~50

	DrawSeizaEffect();//z 0

	{
		DrawAri();
		DrawTau();
		DrawGem();
		DrawCan();
		DrawLeo();
		DrawVir();
		DrawLib();
		DrawSco();
		DrawSag();
		DrawCap();
		DrawAqu();
		DrawPis();
	}

	DrawStarEffect();//�����̌�ɒu��

	DrawMouse();

	DrawScore();//2D�`��
	DrawTimer();//2D�`��
	DrawStack();//2D�`��


	DrawBezier_Particle();
}

