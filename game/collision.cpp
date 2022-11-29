//==========================================
//
//  当たり判定系の総括制御(collision.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "colllision.h"
#include "meshcylinder.h"

//==========================================
//  円柱に対する当たり判定
//==========================================
bool CollisionCylinder(D3DXVECTOR3 pos)
{
	//ローカル変数宣言
	D3DXVECTOR3 *pVtxWall = GetVtxPos();
	int nCheck = (int)(MAX_VERTEX_CYLINDER * 0.5f);
	bool nReturn = false;

	//判定
	for (int nCntWall = 0; nCntWall < nCheck; nCntWall++, pVtxWall++)
	{
		//判定用変数宣言
		D3DXVECTOR3 posEnd = *pVtxWall;
		D3DXVECTOR3 posStart = *pVtxWall + 1;

		//数値の補正
		if (nCntWall == nCheck -1)
		{
			pVtxWall -= nCntWall;
			posStart = *pVtxWall;
		}

		//判定の処理
		if ((posEnd.z - posStart.z) * (pos.x - posStart.x) - (posEnd.x - posStart.x) * (pos.z - posStart.z) < 0.0f)
		{
			nReturn = true;
		}
	}

	return nReturn;
}
