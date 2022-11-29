//==========================================
//
//  ���b�V���V�����_�[�̐���(meshcylinder.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_
#include "main.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_INDEX_CYLINDER (66) //�C���f�b�N�X��
#define NUM_VERTEX (32.0f) //������
#define VERTEX_ANGLE (NUM_VERTEX * 0.5f) //�p�x
#define CYLINDER_SIZE (300.0f) //�V�����_�[�̔��a
#define CYLINDER_HEIGHT (100.0f) //�V�����_�[�̍���
#define MAX_VERTEX_CYLINDER (NUM_VERTEX * 2 + 2) //���_��

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);
D3DXVECTOR3* GetVtxPos(void);

#endif
