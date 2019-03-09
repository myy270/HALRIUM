//=============================================================================
//
// �o���b�g�G�t�F�N�g���� [smoke.h]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#ifndef _STAREFFECT_H_
#define _STAREFFECT_H_

#include "main.h"

typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �s���T
	D3DXVECTOR3 scl;		// �s���S
	bool		use;		// �g�p���Ă��邩�ǂ���
	D3DXCOLOR	color;

} STAREFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitStarEffect(int type);
void UninitStarEffect(void);
void UpdateStarEffect(void);
void DrawStarEffect(void);
int SetStarEffect(D3DXVECTOR3 pos, int indexPlayer);

void DelStarEffect(int index);
#endif
