//=============================================================================
//
// �X�R�A�G�t�F�N�g���� [score_effect.h]
// Author : ���c�@����
//
//=============================================================================
#ifndef _BEZIER_PARTICLEH_
#define _BEZIER_PARTICLEH_H_

#include "main.h"
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR	col;		// �F
	float		fSizeX;		// ��
	float		fSizeY;		// ����
	int			nIdxShadow;	// �eID
	int			nLife;		// ����
	bool		bUse;		// �g�p���Ă��邩�ǂ���
	int			frame;		// ���B�t���[��
	int			cntFrame;	// �J�E���g�t���[��
	D3DXVECTOR3 start;		//				�X�^�[�g�ʒu
	D3DXVECTOR3 control1;	// �x�W�F�Ȑ��p �O��1
	D3DXVECTOR3 control2;	//				�O��2
	int			cntclick;	// �J�E���g�t���[��

} BEZIER_PARTICLEH;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBezier_Particle(void);				// ����������
void UninitBezier_Particle(void);				// �I������
void UpdateBezier_Particle(void);				// �X�V����
void DrawBezier_Particle(void);					// �`�揈��
void SetBezier_Particle(D3DXVECTOR3 pos);		// �Z�b�g�֐�(�o�������ꏊ�ɐݒ�)
BEZIER_PARTICLEH *GetBezier_Particle(void);		// �֐��擾
#endif

#pragma once
