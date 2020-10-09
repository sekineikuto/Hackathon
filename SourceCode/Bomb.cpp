//*************************************************************************************************************
//
// �v���C���[���� [player.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Bomb.h"
#include "Scene2D.h"
#include "texture.h"
#include "game.h"
#include "sound.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define BOMB_BASEFORCE 13.5f

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CBomb * CBomb::Create(D3DXVECTOR3 & pos, D3DXVECTOR2 & size, int nPlayer_ID)
{
	CBomb *pPlayer = new CBomb;
	pPlayer->Init(pos, size, nPlayer_ID);
	pPlayer->SetObjectTyoe(TYPE_BULLET);
	return pPlayer;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CBomb::Init(D3DXVECTOR3 & pos, D3DXVECTOR2 & size, int &nPlayer_ID)
{
	m_bBGM = false;
	m_nPlayer_ID = nPlayer_ID;
	m_fDistance = 0.0f;
	m_StartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pScene2D = CScene2D::Create(PRIORITY::PRIORITY_PLAYER, pos, ORIGINVERTEXTYPE::ORIGINVERTEXTYPE_ROTCENTER, size);
	m_pScene2D->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_PIENBOMB));
	m_State = STATE_STANDBY;
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CBomb::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//------------------------------------------------------------------------------------------------------------
void CBomb::Update(void)
{
	switch (m_State)
	{
		MLB_CASE(STATE_STANDBY)UpdateStandby();
		//MLB_CASE(STATE_MOVEING)UpdateMoveing();
		MLB_CASE(STATE_LANDING)UpdateLanding();
	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
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
	if (m_State == STATE_MOVEING)
	{
		if (m_bBGM == false)
		{
			CManager::GetSound().PlaySoundA(CSound::SOUND_LABEL_SE_THROW);
			m_bBGM = true;
		}

		// �ʒu�̎擾
		D3DXVECTOR3 *pos = m_pScene2D->GetPosition();

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

}

void CBomb::UpdateLanding(void)
{
}

void CBomb::Expansion(void)
{
	// �ʒu�̎擾
	D3DXVECTOR2 *size = m_pScene2D->GetSize();

	size->x += 15.0f;
	size->y += 15.0f;
	m_pScene2D->UpdateVatexPosition();

}

void CBomb::Fire(float & fForce, float & fForceY, D3DXVECTOR3 &pos)
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

	cout << "�v���C���[ID ==  " << m_nPlayer_ID << "\n";
	cout << "X����Force ==  " << fResultForce << "\n";
	cout << "Y����Force ==  " << fResultForceY << "\n";

	// �ʒu�̎擾
	*m_pScene2D->GetPosition() = pos;
	m_StartPos = *m_pScene2D->GetPosition();
	m_pScene2D->UpdateVatexPosition();
	m_State = STATE_MOVEING;
}


D3DXVECTOR3 & CBomb::GetPos(void)
{
	return *m_pScene2D->GetPosition();
}