//==========================================
//
//  �����蔻��n�̑�������(collision.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "colllision.h"
#include "meshcylinder.h"

//==========================================
//  �~���ɑ΂��铖���蔻��
//==========================================
bool CollisionCylinder(D3DXVECTOR3 pos)
{
	//���[�J���ϐ��錾
	D3DXVECTOR3 *pVtxWall = GetVtxPos();
	int nCheck = (int)(MAX_VERTEX_CYLINDER * 0.5f);
	bool nReturn = false;

	//����
	for (int nCntWall = 0; nCntWall < nCheck; nCntWall++, pVtxWall++)
	{
		//����p�ϐ��錾
		D3DXVECTOR3 posEnd = *pVtxWall;
		D3DXVECTOR3 posStart = *pVtxWall + 1;

		//���l�̕␳
		if (nCntWall == nCheck -1)
		{
			pVtxWall -= nCntWall;
			posStart = *pVtxWall;
		}

		//����̏���
		if ((posEnd.z - posStart.z) * (pos.x - posStart.x) - (posEnd.x - posStart.x) * (pos.z - posStart.z) < 0.0f)
		{
			nReturn = true;
		}
	}

	return nReturn;
}
