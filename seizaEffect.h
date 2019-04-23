//=============================================================================
//
// 
// Author : GP11A_341 ���p�j 
//
//=============================================================================
#ifndef _SEIZAEFFECT_H_
#define _SEIZAEFFECT_H_

#include "main.h"
#include "seiza.h"
#include "mouse.h"

typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �s���T
	D3DXVECTOR3 scl;		// �s���S
	bool		use;		// �g�p���Ă��邩�ǂ���
	D3DXCOLOR	color;
	float delta;			//�A���t�@�̕ω��l
} SEIZAEFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSeizaEffect(int type);
void UninitSeizaEffect(void);
void UpdateSeizaEffect(void);
void DrawSeizaEffect(void);

void SetSeizaEffect(ZODIAC seiza, D3DXVECTOR3 pos, D3DXVECTOR3 scl);

void DelSeizaEffect(int index);

SEIZAEFFECT* GetSeizaEffect(void);
#endif
