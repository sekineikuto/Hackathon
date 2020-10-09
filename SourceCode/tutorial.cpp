//*************************************************************************************************************
//
// �`���[�g���A������ [tutorial.h]
// Author:KOKI NISHIYAMA
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "tutorial.h"
#include "2DUI.h"
#include "keyboard.h"
#include "renderer.h"
#include "fade.h"
#include "ui.h"
#include "sound.h"

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CTutorial * CTutorial::Create(void)
{
	CTutorial *pTitle = new CTutorial;
	pTitle->Init();
	return pTitle;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Init(void)
{
	C2DUi::SETING2DUI set[TUTORIALUI_MAX];
	// UI�̎擾
	std::vector<CUi::UI_LOAD> Ui_Load = CUi::GetUi(CUi::UITYPE_TUTORIALUI);
	// UI�S�̂̐ݒ�
	for (int nCntUi = 0; nCntUi < (signed)Ui_Load.size(); nCntUi++)
	{
		set[nCntUi].bDisp = true;
		set[nCntUi].col = Ui_Load[nCntUi].col;
		set[nCntUi].fRotation = Ui_Load[nCntUi].fRot;
		set[nCntUi].nTextureID = Ui_Load[nCntUi].nTexType;
		set[nCntUi].pos = Ui_Load[nCntUi].pos;
		set[nCntUi].size = Ui_Load[nCntUi].size;
	}

	pC2dui[TUTORIALUI_DISCRIPTION] = C2DUi::Create(set[TUTORIALUI_DISCRIPTION]);
	// �v���X�G���^�[�A�C�R���̐ݒ�
	set[TUTORIALUI_PRESSENTERICON].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	// �v���X�G���^�[�̃A�C�R������
	pC2dui[TUTORIALUI_PRESSENTERICON] = C2DUi::Create(set[TUTORIALUI_PRESSENTERICON]);
	pC2dui[TUTORIALUI_PRESSENTERICON]->GetFlashing()->m_nTiming = 3;
	pC2dui[TUTORIALUI_PRESSENTERICON]->GetFade()->bLoop = true;
	pC2dui[TUTORIALUI_PRESSENTERICON]->GetFade()->nTiming = 30;
	pC2dui[TUTORIALUI_PRESSENTERICON]->GetFade()->fChangeValue = 1.0f / pC2dui[TUTORIALUI_PRESSENTERICON]->GetFade()->nTiming;
	pC2dui[TUTORIALUI_PRESSENTERICON]->GetFade()->nAddSign = 1;

	this->m_State = STATE_NORMAL;

	CMode::Init(STATE_NORMAL, 30);
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Update(void)
{
	if (this->m_State == STATE_NORMAL)
	{
		// �t�F�[�h�X�V����
		for (int nCntUi = 0; nCntUi < TUTORIALUI_MAX; nCntUi++)
		{
			if (pC2dui[nCntUi]->GetFade() == NULL) continue;
			pC2dui[nCntUi]->GetFade()->Update(pC2dui[nCntUi]->GetImage());
		}
	}
	else if (this->m_State == STATE_OUT)
	{
		// �t���b�V���X�V����
		for (int nCntUi = 0; nCntUi < TUTORIALUI_MAX; nCntUi++)
		{
			if (pC2dui[nCntUi]->GetFlashing() == NULL) continue;
			pC2dui[nCntUi]->GetFlashing()->Update(pC2dui[nCntUi]);
		}
		if (this->m_nCntState == this->m_nMaxCntState)
		{
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_GAME);
		}
		this->m_nCntState++;
	}
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
		{
			this->SetState(STATE_OUT);
			// SE������
			CManager::GetSound().PlaySound(CSound::SOUND_LABEL_SE_ENTER);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Draw(void)
{
}
