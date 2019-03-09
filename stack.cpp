//=============================================================================
//
// 
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "stack.h"
#include "mouse.h"

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
#define	TEXTURE_FRAME		("data/TEXTURE/stack_frame.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_BG01		("data/TEXTURE/stack_bg01.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_BG02		("data/TEXTURE/stack_bg02.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_ARI			("data/TEXTURE/ari.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TAU			("data/TEXTURE/tau.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_GEM			("data/TEXTURE/gem.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_CAN			("data/TEXTURE/can.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_LEO			("data/TEXTURE/leo.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_VIR			("data/TEXTURE/vir.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_LIB			("data/TEXTURE/lib.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_SCO			("data/TEXTURE/sco.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_SAG			("data/TEXTURE/sag.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_CAP			("data/TEXTURE/cap.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_AQU			("data/TEXTURE/aqu.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PIS			("data/TEXTURE/pis.png")	// �ǂݍ��ރe�N�X�`���t�@�C����

#define STACK_MAX			(12)
#define BG_TEXTURE_MAX		(2)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexStack(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9				D3DTextureFrame = NULL;							// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9				D3DTextureBg[BG_TEXTURE_MAX] = { NULL,NULL };	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9				D3DTextureZodiac[STACK_MAX];					// �e�N�X�`���ւ̃|�C���^

VERTEX_2D						VertexWkFrame[STACK_MAX][NUM_VERTEX];	// ���_���i�[���[�N
VERTEX_2D						VertexWkBg[STACK_MAX][NUM_VERTEX];		// ���_���i�[���[�N
VERTEX_2D						VertexWkZodiac[STACK_MAX][NUM_VERTEX];	// ���_���i�[���[�N


STACK			StackWk[STACK_MAX];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitStack(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	STACK *stack = &StackWk[0];


	for (int i = 0; i < STACK_MAX; i++, stack++)
	{
		stack->use = false;		// �X�^�b�N��BG�̎g�p���
		stack->texNum = 0;	// �X�^�b�NBG�̃e�N�X�`���ԍ�
	}

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FRAME,					// �t�@�C���̖��O
			&D3DTextureFrame);				// �ǂݍ��ރ������[
				// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BG01,					// �t�@�C���̖��O
			&D3DTextureBg[0]);				// �ǂݍ��ރ������[
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BG02,					// �t�@�C���̖��O
			&D3DTextureBg[1]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ARI,					// �t�@�C���̖��O
			&D3DTextureZodiac[ARI]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TAU,					// �t�@�C���̖��O
			&D3DTextureZodiac[TAU]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GEM,					// �t�@�C���̖��O
			&D3DTextureZodiac[GEM]);				// �ǂݍ��ރ������[
		
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_CAN,					// �t�@�C���̖��O
			&D3DTextureZodiac[CAN]);				// �ǂݍ��ރ������[
		
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_LEO,					// �t�@�C���̖��O
			&D3DTextureZodiac[LEO]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_VIR,					// �t�@�C���̖��O
			&D3DTextureZodiac[VIR]);				// �ǂݍ��ރ������[
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_LIB,					// �t�@�C���̖��O
			&D3DTextureZodiac[LIB]);				// �ǂݍ��ރ������[
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SCO,					// �t�@�C���̖��O
			&D3DTextureZodiac[SCO]);				// �ǂݍ��ރ������[
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SAG,					// �t�@�C���̖��O
			&D3DTextureZodiac[SAG]);				// �ǂݍ��ރ������[
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_CAP,					// �t�@�C���̖��O
			&D3DTextureZodiac[CAP]);				// �ǂݍ��ރ������[
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_AQU,					// �t�@�C���̖��O
			&D3DTextureZodiac[AQU]);				// �ǂݍ��ރ������[
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PIS,					// �t�@�C���̖��O
			&D3DTextureZodiac[PIS]);				// �ǂݍ��ރ������[

	}
	
	// ���_���̍쐬
	MakeVertexStack();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitStack(void)
{
	if(D3DTextureFrame != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureFrame->Release();
		D3DTextureFrame = NULL;
	}

	for (int i = 0; i < BG_TEXTURE_MAX; i++)
	{
		if (D3DTextureBg[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureBg[i]->Release();
			D3DTextureBg[i] = NULL;
		}
	}

	for (int j = 0; j < STACK_MAX; j++)
	{
		if (D3DTextureBg[j] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureBg[j]->Release();
			D3DTextureBg[j] = NULL;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateStack(void)
{
	SEIZA** seiza = GetSeiza();
	
	for (int i = 0; i < MAX_SEIZA; i++, seiza++)
	{
		if ((*seiza)->isConnect)
		{
			StackWk[i].use = true;
			if ((*seiza)->belong == PLAYER_ONE)
			{
				StackWk[i].texNum = 0;
			}
			else if ((*seiza)->belong == PLAYER_TWO)
			{
				StackWk[i].texNum = 1;
			}
		}
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawStack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	STACK *stack = &StackWk[0];

	for (int i = 0; i < STACK_MAX; i++, stack++)
	{
		//�g�̔w�i
		if (stack->use)
		{
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTextureBg[stack->texNum]);//�F

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkBg[i], sizeof(VERTEX_2D));
		}
	
		//�g�̐����}��
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureZodiac[i]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkZodiac[i], sizeof(VERTEX_2D));


		//�g
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureFrame);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, VertexWkFrame[i], sizeof(VERTEX_2D));
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexStack(void)
{
	for(int i = 0; i < STACK_MAX; i++)
	{
		// �g
		{
			// ���_���W�̐ݒ�
			VertexWkFrame[i][0].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkFrame[i][1].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkFrame[i][2].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);
			VertexWkFrame[i][3].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);

			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			VertexWkFrame[i][0].rhw =
				VertexWkFrame[i][1].rhw =
				VertexWkFrame[i][2].rhw =
				VertexWkFrame[i][3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			VertexWkFrame[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 155);
			VertexWkFrame[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 155);
			VertexWkFrame[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 155);
			VertexWkFrame[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 155);

			// �e�N�X�`�����W�̐ݒ�
			VertexWkFrame[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
			VertexWkFrame[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
			VertexWkFrame[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
			VertexWkFrame[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ����
		{
			// ���_���W�̐ݒ�
			VertexWkZodiac[i][0].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkZodiac[i][1].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkZodiac[i][2].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);
			VertexWkZodiac[i][3].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);

			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			VertexWkZodiac[i][0].rhw =
				VertexWkZodiac[i][1].rhw =
				VertexWkZodiac[i][2].rhw =
				VertexWkZodiac[i][3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			VertexWkZodiac[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 205);
			VertexWkZodiac[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 205);
			VertexWkZodiac[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 205);
			VertexWkZodiac[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 205);

			// �e�N�X�`�����W�̐ݒ�
			VertexWkZodiac[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
			VertexWkZodiac[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
			VertexWkZodiac[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
			VertexWkZodiac[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// BG
		{
			// ���_���W�̐ݒ�
			VertexWkBg[i][0].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkBg[i][1].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y, 0.0f);
			VertexWkBg[i][2].vtx = D3DXVECTOR3(STACK_POS_X + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);
			VertexWkBg[i][3].vtx = D3DXVECTOR3(STACK_POS_X + STACK_SIZE + (STACK_SIZE* i), STACK_POS_Y + STACK_SIZE, 0.0f);

			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			VertexWkBg[i][0].rhw =
				VertexWkBg[i][1].rhw =
				VertexWkBg[i][2].rhw =
				VertexWkBg[i][3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			VertexWkBg[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 95);
			VertexWkBg[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 95);
			VertexWkBg[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 95);
			VertexWkBg[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 95);

			// �e�N�X�`�����W�̐ݒ�
			VertexWkBg[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
			VertexWkBg[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
			VertexWkBg[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
			VertexWkBg[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}


	return S_OK;
}
