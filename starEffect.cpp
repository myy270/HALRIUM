//=============================================================================
//
// �G�t�F�N�g���� [starEffect.cpp]
// Author : GP11A_341_22_�c�����z 
//
//=============================================================================
#include "starEffect.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_STAREFFECT		"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	STAREFFECT_SIZE_X		(4.0f)						// �r���{�[�h�̕�
#define	STAREFFECT_SIZE_Y		(4.0f)						// �r���{�[�h�̍���
#define STAREFFECT_MAX			(240)							//�G�t�F�N�g�̔ԍ�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexStarEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexStarEffect(int Index, float fSizeX, float fSizeY);
void SetDiffuseStarEffect(int Index, D3DCOLOR val);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

LPDIRECT3DTEXTURE9			D3DTextureStarEffect;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		D3DVtxBuffStarEffect = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

STAREFFECT					starEffectWk[STAREFFECT_MAX];	// STAREFFECT�\���̗p�ϐ�
//=============================================================================
// ����������
//=============================================================================
HRESULT InitStarEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	STAREFFECT *starEffect = &starEffectWk[0];

	// ���_���̍쐬
	MakeVertexStarEffect(pDevice);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			TEXTURE_STAREFFECT,					// �t�@�C���̖��O
			&D3DTextureStarEffect);			// �ǂݍ��ރ������[
	}

	for (int i = 0; i < STAREFFECT_MAX; i++, starEffect++)
	{
		starEffect->use = false;
		starEffect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		starEffect->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		starEffect->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitStarEffect(void)
{
	if (D3DTextureStarEffect != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureStarEffect->Release();
		D3DTextureStarEffect = NULL;
	}


	if (D3DVtxBuffStarEffect != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffStarEffect->Release();
		D3DVtxBuffStarEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateStarEffect(void)
{


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawStarEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	STAREFFECT *starEffect = &starEffectWk[0];

	
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	for (int i = 0; i < STAREFFECT_MAX; i++, starEffect++)
	{
		if (starEffect->use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&starEffect->mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, starEffect->scl.x,
				starEffect->scl.y,
				starEffect->scl.z);
			D3DXMatrixMultiply(&starEffect->mtxWorld,
				&starEffect->mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, starEffect->pos.x,
				starEffect->pos.y,
				starEffect->pos.z);
			D3DXMatrixMultiply(&starEffect->mtxWorld,
				&starEffect->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &starEffect->mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, D3DVtxBuffStarEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTextureStarEffect);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * NUM_VERTEX), NUM_POLYGON);
		}
	}

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexStarEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * STAREFFECT_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffStarEffect,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffStarEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < STAREFFECT_MAX; i++, pVtx += NUM_VERTEX)
		{
			// ���_���W�̐ݒ�
			SetVertexStarEffect(i, STAREFFECT_SIZE_X, STAREFFECT_SIZE_Y);

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
		D3DVtxBuffStarEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexStarEffect(int Index, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffStarEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffStarEffect->Unlock();
	}
}

//============================================================================
// ���G�t�F�N�g�̓��ߏ���
//============================================================================
void SetDiffuseStarEffect(int Index, D3DCOLOR val)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffStarEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (Index * 4);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = val;

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffStarEffect->Unlock();
	}
}

//==========================================================================
// ���G�t�F�N�g�̐ݒu
// ���@���Findex �v���C���[�̔ԍ�
// �߂�l�F�@�G�t�F�N�g�̃C���f�N�X�ԍ�
//==========================================================================
int SetStarEffect(D3DXVECTOR3 pos, int indexPlayer)
{
	STAREFFECT *starEffect = &starEffectWk[0];

	for (int i = 0; i < STAREFFECT_MAX; i++, starEffect++)
	{
		if (!starEffect->use)
		{
			starEffect->use = true;	// �g�p��
			starEffect->pos = pos;

			indexPlayer == 0 ? starEffect->color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f) :
				starEffect->color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.4f);

			SetDiffuseStarEffect(i, starEffect->color);

			return i;
		}
	}

	return -1;
}

void DelStarEffect(int index)
{

	if ((index >= 0) && (index < STAREFFECT_MAX))
	{

		starEffectWk[index].use = false;
	}

}

