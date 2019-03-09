//=============================================================================
//
// �g
// Author : GP11A_341_22_�c�����z
//
//=============================================================================
#ifndef _STACK_H_
#define _STACK_H_

#include "main.h"
#include "seiza.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	STACK_SIZE					(SCREEN_WIDTH / 12.0f)		// �X�^�b�N�̕�
#define	STACK_POS_X					(0.0f)			// �X�^�b�N�̘g�\���ʒu
#define	STACK_POS_Y					(SCREEN_HEIGHT - STACK_SIZE)		// �X�^�b�N�̕\���ʒu


typedef struct {

	bool		use;
	int			texNum;		//�F�@0�ԁ@1��

}STACK;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitStack(int type);
void UninitStack(void);
void UpdateStack(void);
void DrawStack(void);

#endif
