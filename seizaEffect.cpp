//=============================================================================
//
// 
// Author : GP11A_341 ���p�j 
//
//=============================================================================
#include "seizaEffect.h"
#include "debugproc.h"


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
#define	TEXTURE_ARI			("data/TEXTURE/ari00.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TAU			("data/TEXTURE/tau00.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_GEM			("data/TEXTURE/gem000.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_CAN			("data/TEXTURE/can00.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_LEO			("data/TEXTURE/leo00.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_VIR			("data/TEXTURE/vir00.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_LIB			("data/TEXTURE/lib000.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_SCO			("data/TEXTURE/sco00.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_SAG			("data/TEXTURE/sag00.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_CAP			("data/TEXTURE/cap00.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_AQU			("data/TEXTURE/aqu000.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PIS			("data/TEXTURE/pis00.png")	// �ǂݍ��ރe�N�X�`���t�@�C����


#define	SEIZAEFFECT_SIZE_X		(83.0f / 4)						// �r���{�[�h�̕�
#define	SEIZAEFFECT_SIZE_Y		(63.0f / 3)						// �r���{�[�h�̍���
#define SEIZAEFFECT_MAX			(12)						//�G�t�F�N�g�̔ԍ�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexSeizaEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexSeizaEffect(int Index, float fSizeX, float fSizeY);
void SetDiffuseSeizaEffect(int Index, D3DCOLOR val);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

LPDIRECT3DTEXTURE9			D3DTextureSeizaEffect[SEIZAEFFECT_MAX];	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		D3DVtxBuffSeizaEffect = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

SEIZAEFFECT					seizaEffectWk[SEIZAEFFECT_MAX];	// SEIZAEFFECT�\���̗p�ϐ�

D3DXVECTOR3 offsetPos[SEIZAEFFECT_MAX] =
{
	D3DXVECTOR3(5.4f, 2.3f, 0.0f),		// 0 ���r��
	D3DXVECTOR3(9.0f, -8.0f, 0.0f),		// 1 ������
	D3DXVECTOR3(3.0f, -10.0f, 0.0f),	// 2 �o�q��
	D3DXVECTOR3(-4.3f, 12.0f, 0.0f),	// 3 �I��
	D3DXVECTOR3(-11.0f, -7.0f, 0.0f),	// 4 ���q��
	D3DXVECTOR3(9.0f, -1.9f, 0.0f),		// 5 ������
	D3DXVECTOR3(-6.8f, 16.0f, 0.0f),	// 6 �V����
	D3DXVECTOR3(-0.5f, -1.2f, 0.0f),	// 7 嶍�
	D3DXVECTOR3(3.8f, 13.0f, 0.0f),		// 8 �ˎ��
	D3DXVECTOR3(10.0f, -3.0f, 0.0f),	// 9 �R�r��
	D3DXVECTOR3(5.0f, -5.0f, 0.0f),		//10 ���r��
	D3DXVECTOR3(9.0f, 10.8f, 0.0f),		//11 ����	
};

D3DXVECTOR3 offsetScl[SEIZAEFFECT_MAX] =
{
	D3DXVECTOR3(14.5f * ARI_DISTANCE * 3.2f, 14.5f * ARI_DISTANCE * 3.2f, 1.0f),	// 0 ���r��
	D3DXVECTOR3(14.5f * TAU_DISTANCE * 3.2f, 14.5f * TAU_DISTANCE * 5.5f, 1.0f),	// 1 ������
	D3DXVECTOR3(14.5f * GEM_DISTANCE * 3.2f, 14.5f * GEM_DISTANCE * 3.2f, 1.0f),	// 2 �o�q��
	D3DXVECTOR3(14.5f * CAN_DISTANCE * 5.7f, 14.5f * CAN_DISTANCE * 8.5f, 1.0f),	// 3 �I��
	D3DXVECTOR3(14.5f * LEO_DISTANCE * 2.1f, 14.5f * LEO_DISTANCE * 3.15f, 1.0f),	// 4 ���q��
	D3DXVECTOR3(14.5f * VIR_DISTANCE * 2.5f, 14.5f * VIR_DISTANCE * 5.0f, 1.0f),	// 5 ������
	D3DXVECTOR3(14.5f * LIB_DISTANCE * 1.7f, 14.5f * LIB_DISTANCE * 2.5f, 1.0f),	// 6 �V����
	D3DXVECTOR3(14.5f * SCO_DISTANCE * 2.3f, 14.5f * SCO_DISTANCE * 3.5f, 1.0f),	// 7 嶍�
	D3DXVECTOR3(14.5f * SAG_DISTANCE * 5.3f, 14.5f * SAG_DISTANCE * 6.5f, 1.0f),	// 8 �ˎ��
	D3DXVECTOR3(14.5f * CAP_DISTANCE * 2.3f, 14.5f * CAP_DISTANCE * 3.5f, 1.0f),	// 9 �R�r��
	D3DXVECTOR3(14.5f * AQU_DISTANCE * 3.3f, 14.5f * AQU_DISTANCE * 4.5f, 1.0f),	//10 ���r��
	D3DXVECTOR3(14.5f * PIS_DISTANCE * 5.0f, 14.5f * PIS_DISTANCE * 8.5f, 1.0f),	//11 ����	
}; 

static D3DXVECTOR3 pos[12];
static D3DXVECTOR3 scl[12];

static D3DXVECTOR3 posCom[12];
static D3DXVECTOR3 sclCom[12];

int randam[12];
//=============================================================================
// ����������
//=============================================================================
HRESULT InitSeizaEffect(int type)
{
	
	
	posCom[0] = D3DXVECTOR3(-29.1f, 23.7f, 0.0f);
	posCom[1] = D3DXVECTOR3(-7.5f, 23.7f, 0.0f);
	posCom[2] = D3DXVECTOR3( 12.5f, 23.7f, 0.0f);
	posCom[3] = D3DXVECTOR3( 28.6f, 23.7f, 0.0f);

	posCom[4] = D3DXVECTOR3(-29.1f, 2.5f, 0.0f);
	posCom[5] =	D3DXVECTOR3(-7.5f, 2.5f, 0.0f);
	posCom[6] =	D3DXVECTOR3(12.5f, 2.5f, 0.0f);
	posCom[7] =	D3DXVECTOR3(28.6f, 2.5f, 0.0f);

	posCom[8] =	D3DXVECTOR3(-29.1f, -16.5f, 0.0f);
	posCom[9] =	D3DXVECTOR3(-7.5f, -16.5f, 0.0f);
	posCom[10] =	D3DXVECTOR3(12.5f, -16.5f, 0.0f);
	posCom[11] =	D3DXVECTOR3(28.6f, -16.5f, 0.0f);
	
	


	for (int i = 0; i < 12; i++)
	{
		pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		scl[i] = D3DXVECTOR3(1.3f, 1.4f, 1.0f);
		randam[i] = -1;
	}


	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SEIZAEFFECT *seizaEffect = &seizaEffectWk[0];

	// ���_���̍쐬
	MakeVertexSeizaEffect(pDevice);

	if (type == 0)
	{
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_ARI,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[ARI]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TAU,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[TAU]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GEM,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[GEM]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_CAN,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[CAN]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_LEO,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[LEO]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_VIR,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[VIR]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_LIB,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[LIB]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SCO,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[SCO]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_SAG,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[SAG]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_CAP,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[CAP]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_AQU,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[AQU]);				// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PIS,					// �t�@�C���̖��O
			&D3DTextureSeizaEffect[PIS]);				// �ǂݍ��ރ������[

	}

	for (int i = 0; i < SEIZAEFFECT_MAX; i++, seizaEffect++)
	{
		int val;
		for (int j = 0; j < 12; j++)
		{
			if (randam[j] == -1)
			{
				while (1)
				{
					val = rand() % 12;
					if (randam[val] == -1)
					{
						seizaEffectWk[i].pos = posCom[val];
						randam[val] = i;
						break;
					}
				}
				break;
			}
		}

		seizaEffect->use = false;
		//seizaEffect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		seizaEffect->scl = D3DXVECTOR3(1.3f, 1.4f, 1.0f);
		seizaEffect->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		seizaEffect->delta = 0.003f;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSeizaEffect(void)
{
	for (int j = 0; j < SEIZAEFFECT_MAX; j++)
	{
		if (D3DTextureSeizaEffect[j] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureSeizaEffect[j]->Release();
			D3DTextureSeizaEffect[j] = NULL;
		}
	}


	if (D3DVtxBuffSeizaEffect != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffSeizaEffect->Release();
		D3DVtxBuffSeizaEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSeizaEffect(void)
{
	SEIZA** seiza = GetSeiza();

	static int index = 0;

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_0))
	{
		index = 0;
	}
	if (GetKeyboardPress(DIK_1))
	{
		index = 1;
	}
	if (GetKeyboardPress(DIK_2))
	{
		index = 2;
	}
	if (GetKeyboardPress(DIK_3))
	{
		index = 3;
	}
	if (GetKeyboardPress(DIK_4))
	{
		index = 4;
	}
	if (GetKeyboardPress(DIK_5))
	{
		index = 5;
	}
	if (GetKeyboardPress(DIK_6))
	{
		index = 6;
	}
	if (GetKeyboardPress(DIK_7))
	{
		index = 7;
	}
	if (GetKeyboardPress(DIK_8))
	{
		index = 8;
	}
	if (GetKeyboardPress(DIK_9))
	{
		index = 9;
	}
	if (GetKeyboardPress(DIK_MINUS))
	{
		index = 10;
	}
	if (GetKeyboardPress(DIK_YEN))
	{
		index = 11;
	}



	if (GetKeyboardPress(DIK_T))
	{
		seizaEffectWk[index].pos.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_G))
	{
		seizaEffectWk[index].pos.y -= 0.1f;
	}
	if (GetKeyboardPress(DIK_F))
	{
		seizaEffectWk[index].pos.x -= 0.1f;
	}
	if (GetKeyboardPress(DIK_H))
	{
		seizaEffectWk[index].pos.x += 0.1f;
	}

	for (int i = 0;i < 12;i++)
	{
		if (GetKeyboardPress(DIK_I))
		{
			scl[i].y += 0.01f;
		}
		if (GetKeyboardPress(DIK_K))
		{
			scl[i].y -= 0.01f;
		}
		if (GetKeyboardPress(DIK_J))
		{
			scl[i].x -= 0.01f;
		}
		if (GetKeyboardPress(DIK_L))
		{
			scl[i].x += 0.01f;
		}
	}
#endif

	for (int i = 0; i < SEIZAEFFECT_MAX; i++, seiza++)
	{
		//if (i == 1)
		//{
			//(*seiza)->isConnect = true;
		//}

		//int val;

		if ((*seiza)->isConnect)
		{
			//if (!seizaEffectWk[i].use)
			//{
			//	for (int j = 0; j < 12; j++)
			//	{
			//		if (randam[j] == -1)
			//		{
			//			while (1)
			//			{
			//				val = rand() % 12;
			//				if (randam[val] == -1)
			//				{
			//					seizaEffectWk[i].pos = posCom[val];
			//					randam[val] = i;
			//					break;
			//				}
			//			}
			//			break;
			//		}
			//	}
			//}
			SetSeizaEffect((*seiza)->name, seizaEffectWk[i].pos, scl[i]);

			//SetSeizaEffect((*seiza)->name, pos[i], scl[i]);
		
			//SetSeizaEffect((*seiza)->name, (*seiza)->star->pos + offsetPos[i], offsetScl[i]);
			
		}
	}

	PrintDebugProc("\n %d Effect: pos(%f,%f)  scl(%f,%f)\n", index, seizaEffectWk[index].pos.x, seizaEffectWk[index].pos.y, scl[index].x, scl[index].y);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSeizaEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTranslate;
	SEIZAEFFECT *seizaEffect = &seizaEffectWk[0];

	for (int i = 0; i < SEIZAEFFECT_MAX; i++, seizaEffect++)
	{
		if (seizaEffect->use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&seizaEffect->mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, seizaEffect->scl.x,
				seizaEffect->scl.y,
				seizaEffect->scl.z);

			D3DXMatrixMultiply(&seizaEffect->mtxWorld,&seizaEffect->mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, seizaEffect->pos.x,
				seizaEffect->pos.y,
				seizaEffect->pos.z);

			D3DXMatrixMultiply(&seizaEffect->mtxWorld,&seizaEffect->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &seizaEffect->mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, D3DVtxBuffSeizaEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTextureSeizaEffect[i]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
		}
	}

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexSeizaEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SEIZAEFFECT_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffSeizaEffect,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffSeizaEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < SEIZAEFFECT_MAX; i++, pVtx += NUM_VERTEX)
		{
			// ���_���W�̐ݒ�
			SetVertexSeizaEffect(i, SEIZAEFFECT_SIZE_X, SEIZAEFFECT_SIZE_Y);

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}
		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffSeizaEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexSeizaEffect(int Index, float fSizeX, float fSizeY)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	D3DVtxBuffSeizaEffect->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (Index * 4);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffSeizaEffect->Unlock();
	
}

//============================================================================
// 
//============================================================================
void SetDiffuseSeizaEffect(int Index, D3DCOLOR val)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffSeizaEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = val;

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffSeizaEffect->Unlock();
	}
}

//==========================================================================

//==========================================================================
void SetSeizaEffect(ZODIAC seiza,D3DXVECTOR3 pos, D3DXVECTOR3 scl)
{
	seizaEffectWk[seiza].use = true;
	seizaEffectWk[seiza].pos = pos;
	seizaEffectWk[seiza].scl = scl;

	seizaEffectWk[seiza].color.a += seizaEffectWk[seiza].delta;

	if (seizaEffectWk[seiza].color.a > 0.8f)
	{
		seizaEffectWk[seiza].delta = -0.001f;

		//seizaEffectWk[seiza].color.a = 0.8f;
	}

	if (seizaEffectWk[seiza].color.a < 0.2f)
	{
		seizaEffectWk[seiza].color.a = 0.2f;//0.2
	}

	//���_�ɔ��f����
	SetDiffuseSeizaEffect(seiza, seizaEffectWk[seiza].color);

}

void DelSeizaEffect(int index)
{

	if ((index >= 0) && (index < SEIZAEFFECT_MAX))
	{

		seizaEffectWk[index].use = false;
	}

}

SEIZAEFFECT* GetSeizaEffect(void)
{
	return seizaEffectWk;
}

