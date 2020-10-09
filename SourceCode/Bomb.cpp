//*************************************************************************************************************
//
// プレイヤー処理 [player.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Bomb.h"
#include "Scene2D.h"
#include "texture.h"
#include "game.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define BOMB_BASEFORCE 13.5f

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CBomb * CBomb::Create(D3DXVECTOR3 & pos, D3DXVECTOR2 & size, int nPlayer_ID)
{
	CBomb *pPlayer = new CBomb;
	pPlayer->Init(pos, size, nPlayer_ID);
	pPlayer->SetObjectTyoe(TYPE_BULLET);
	return pPlayer;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CBomb::Init(D3DXVECTOR3 & pos, D3DXVECTOR2 & size, int &nPlayer_ID)
{
	m_nPlayer_ID = nPlayer_ID;
	m_fDistance = 0.0f;
	m_StartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pScene2D = CScene2D::Create(PRIORITY::PRIORITY_PLAYER, pos, ORIGINVERTEXTYPE::ORIGINVERTEXTYPE_ROTCENTER, size);
	m_pScene2D->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_PIENBOMB));
	m_State = STATE_STANDBY;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CBomb::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//------------------------------------------------------------------------------------------------------------
void CBomb::Update(void)
{
	switch (m_State)
	{
		MLB_CASE(STATE_STANDBY)UpdateStandby();
		MLB_CASE(STATE_MOVEING)UpdateMoveing();
		MLB_CASE(STATE_LANDING)UpdateLanding();
	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CBomb::Draw(void)
{
	if (m_pScene2D != nullptr)
	{
		m_pScene2D->Draw();
	}
}

void CBomb::UpdateStandby(void)
{
}

void CBomb::UpdateMoveing(void)
{
	// 位置の取得
	D3DXVECTOR3 *pos = 	m_pScene2D->GetPosition();

	*pos += m_move;
	m_move.y += 0.5f;

	if (pos->y >= 600.0f)
	{
		pos->y = 600.0f;
		m_State = STATE_LANDING;

		m_fDistance = abs(pos->x - m_StartPos.x);
	}

	m_pScene2D->UpdateVatexPosition();
}

void CBomb::UpdateLanding(void)
{
}

void CBomb::Expansion(void)
{
	// 位置の取得
	D3DXVECTOR2 *size = m_pScene2D->GetSize();

	size->x += 15.0f;
	size->y += 15.0f;
	m_pScene2D->UpdateVatexPosition();

}

void CBomb::Fire(float & fForce, float & fForceY)
{
	if (m_State != STATE_STANDBY)
	{
		return;
	}
	float fResultForce = fForce *0.5f +0.5f;
	float fResultForceY = fForceY *0.5f +0.5f;


	if (m_nPlayer_ID == CGame::PLAYER_1)
	{
		m_move.x = BOMB_BASEFORCE*fResultForce;
	}
	else
	{
		m_move.x = -BOMB_BASEFORCE*fResultForce;
	}

	m_move.y = -BOMB_BASEFORCE*fResultForceY;

	cout << "プレイヤーID ==  " << m_nPlayer_ID << "\n";
	cout << "X軸のForce ==  " << fResultForce << "\n";
	cout << "Y軸のForce ==  " << fResultForceY << "\n";

	// 位置の取得
	m_StartPos = *m_pScene2D->GetPosition();
	m_State = STATE_MOVEING;
}


D3DXVECTOR3 & CBomb::GetPos(void)
{
	return *m_pScene2D->GetPosition();
}