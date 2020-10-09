//*************************************************************************************************************
//
// �^�C�g������ [title.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "result.h"
#include "2DUI.h"

#include "renderer.h"
#include "keyboard.h"
#include "fade.h"
#include "score.h"
#include "texture.h"
#include "sound.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define DIFWIDTH_PIENDISTANCE	(300.0f)					// �P�ʂ̉���
#define DIFHEIGHT_PIENDISTANCE	(90.0f)						// �P�ʂ̍���
#define SIZE_PIENDISTANCE		(D3DXVECTOR2(200.0f, 70.0f))	// �P�ʂ̃T�C�Y
#define SIZE_PIEN_JUDGE			(D3DXVECTOR2(250.0f, 250.0f))	// �҂���̃T�C�Y
#define SIZE_TEXT_JUDGE			(D3DXVECTOR2(200.0f, 100.0f))	// ���͂̃T�C�Y

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CResult::m_posScore[MAX_PLAYER] =
{
	D3DXVECTOR3(100.0f, 590.0f, 0.0f),
	D3DXVECTOR3(740.0f, 590.0f, 0.0f)
};

D3DXVECTOR2 CResult::m_sizeScore[MAX_PLAYER] =
{
	D3DXVECTOR2(40.0f, 80.0f),
	D3DXVECTOR2(40.0f, 80.0f)
};

D3DXVECTOR3 CResult::m_posPien[MAX_PLAYER] =
{
	D3DXVECTOR3(300.0f, SCREEN_WIDTH / 2 - 350.0f, 0.0f),
	D3DXVECTOR3(940.0f, SCREEN_WIDTH / 2 - 350.0f, 0.0f)
};

D3DXVECTOR3 CResult::m_posText[MAX_PLAYER] =
{
	D3DXVECTOR3(300.0f, SCREEN_WIDTH / 2 - 150.0f, 0.0f),
	D3DXVECTOR3(940.0f, SCREEN_WIDTH / 2 - 150.0f, 0.0f)
};

int CResult::m_nWinPlayer = 0;

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CResult * CResult::Create(void)
{
	CResult *pTitle = new CResult;
	pTitle->Init();
	return pTitle;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CResult::Init(void)
{
	/*C2DUi::SETING2DUI set;
	set.bDisp = true;
	set.col = MYLIB_D3DXCOR_SET;
	set.fRotation = 0.0f;
	set.mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_NUMBER;
	set.nTextureID = 1;
	set.nValue = 123456789;
	set.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	set.size = D3DXVECTOR2(60.0f, 120.0f);
	pC2dui = C2DUi::Create(set);

	pC2dui->GetFade()->bLoop = true;
	pC2dui->GetFade()->nTiming = 30;
	pC2dui->GetFade()->fChangeValue = 1.0f / pC2dui->GetFade()->nTiming;
	pC2dui->GetFade()->nAddSign = 1;

	//pC2dui->GetFlashing()->m_nTiming = 20;*/

	C2DUi::SETING2DUI set;
	set.bDisp = true;
	set.col = MYLIB_D3DXCOR_SET;
	set.fRotation = 0.0f;
	set.nValue = 123456789;

	// �v���C���[�̐������X�R�A����
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// �t���b�V���̐ݒ�
		bool bFlash = false;
		if (m_nWinPlayer == nCntPlayer + 1)
			bFlash = true;

		// �X�R�A
		m_pScore[nCntPlayer] = CScore::Create(m_posScore[nCntPlayer],
								m_sizeScore[nCntPlayer],
								CScore::GetPlayerScore(nCntPlayer), bFlash);
		// �P��
		set.nTextureID = CTexture::NAME_PIENDISTANCE;
		set.pos = D3DXVECTOR3(m_posScore[nCntPlayer].x + DIFWIDTH_PIENDISTANCE, m_posScore[nCntPlayer].y + DIFHEIGHT_PIENDISTANCE, m_posScore[nCntPlayer].z);
		set.size = SIZE_PIENDISTANCE;
		pPienDistance[nCntPlayer] = C2DUi::Create(set);

		// �҂���
		set.nTextureID = CTexture::NAME_PIEN_LOSE;
		if (m_nWinPlayer == nCntPlayer + 1)
			set.nTextureID = CTexture::NAME_PIEN_WIN;
		set.pos = D3DXVECTOR3(m_posPien[nCntPlayer].x, m_posPien[nCntPlayer].y, m_posPien[nCntPlayer].z);
		set.size = SIZE_PIEN_JUDGE;
		pPienIcon[nCntPlayer] = C2DUi::Create(set);

		// ���s�̕���
		set.nTextureID = CTexture::NAME_WIN;
		if (m_nWinPlayer != nCntPlayer + 1)
			set.nTextureID = CTexture::NAME_LOSE;
		if (m_nWinPlayer == 0)
			set.nTextureID = CTexture::NAME_DRAW;
		set.pos = D3DXVECTOR3(m_posText[nCntPlayer].x, m_posText[nCntPlayer].y, m_posText[nCntPlayer].z);
		set.size = SIZE_TEXT_JUDGE;
		pPienIcon[nCntPlayer] = C2DUi::Create(set);
	}

	CScore::SaveScore();
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CResult::Uninit(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (m_pScore[nCntPlayer])
		{
			m_pScore[nCntPlayer]->Uninit();
			delete m_pScore[nCntPlayer];
			m_pScore[nCntPlayer] = nullptr;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CResult::Update(void)
{
	//pC2dui->GetFade()->Update(pC2dui->GetNumericString());
	//pC2dui->GetFlashing()->Update(pC2dui);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (m_pScore[nCntPlayer])
			m_pScore[nCntPlayer]->Update();
	}

	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
		{
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_RANKING);
			CManager::GetSound().PlaySound(CSound::SOUND_LABEL_SE_ENTER);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CResult::Draw(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (m_pScore[nCntPlayer])
			m_pScore[nCntPlayer]->Draw();
	}
}
