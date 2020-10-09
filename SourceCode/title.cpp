//*************************************************************************************************************
//
// �^�C�g������ [title.h]
// Author:KOKI NISHIYAMA
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "title.h"
#include "2DUI.h"
#include "keyboard.h"
#include "renderer.h"
#include "fade.h"
#include "3Dparticle.h"
#include "2Deffect.h"
#include "ui.h"
#include "Scene2D.h"
#include "sound.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define SIZEORIGIN	(180)		// ���T�C�Y
#define ROTLIMIT	(0.75f)			// �p�x�̌��E�l
#define ROTCHANGE	(0.02f)			// �p�x�̌��E�l

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CTitle * CTitle::Create(void)
{
	CTitle *pTitle = new CTitle;
	pTitle->Init();
	return pTitle;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CTitle::Init(void)
{
	// �ϐ��錾
	C2DUi::SETING2DUI set[TITLEUI_MAX];	// UI�ݒ�p�ϐ�
	// 2D�G�t�F�N�g����
	C2DEffect::Create();
	// UI�̎擾
	std::vector<CUi::UI_LOAD> Ui_Load = CUi::GetUi(CUi::UITYPE_TITLEUI);
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
	// �^�C�g���w�i�̐���
	pC2dui[TITLEUI_BG] = C2DUi::Create(set[TITLEUI_BG]);

	// �^�C�g���̐ݒ�
	set[TITLEUI_NAME].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	// �^�C�g���̐���
	pC2dui[TITLEUI_NAME] = C2DUi::Create(set[TITLEUI_NAME]);
	pC2dui[TITLEUI_NAME]->GetFlashing()->m_nTiming = 3;
	pC2dui[TITLEUI_NAME]->GetFade()->bLoop = true;
	pC2dui[TITLEUI_NAME]->GetFade()->nTiming = 90;
	pC2dui[TITLEUI_NAME]->GetFade()->fChangeValue = 1.0f / pC2dui[TITLEUI_NAME]->GetFade()->nTiming;
	pC2dui[TITLEUI_NAME]->GetFade()->nAddSign = 1;

	// �v���X�G���^�[�̐ݒ�
	set[TITLEUI_PRESSENTERNAME].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	// �v���X�G���^�[�̐���
	pC2dui[TITLEUI_PRESSENTERNAME] = C2DUi::Create(set[TITLEUI_PRESSENTERNAME]);
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFlashing()->m_nTiming = 3;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->bLoop = true;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->nTiming = 30;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->fChangeValue = 1.0f / pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->nTiming;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->nAddSign = 1;

	// �v���X�G���^�[�A�C�R���̐ݒ�
	set[TITLEUI_PRESSENTERICON].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	// �v���X�G���^�[�̃A�C�R������
	pC2dui[TITLEUI_PRESSENTERICON] = C2DUi::Create(set[TITLEUI_PRESSENTERICON]);
	pC2dui[TITLEUI_PRESSENTERICON]->GetFlashing()->m_nTiming = 3;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->bLoop = true;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->nTiming = 30;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->fChangeValue = 1.0f / pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->nTiming;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->nAddSign = 1;

	// �v���C���[1�̃A�C�R������
	// *GETSIZE�f�����ė���
	// �l��ς���
	// �X�V�o�[�e�N�X�ŕς���
	pC2dui[TITLEUI_PLAYER1ICON] = C2DUi::Create(set[TITLEUI_PLAYER1ICON]);

	// �v���C���[2�̃A�C�R������
	pC2dui[TITLEUI_PLAYER2ICON] = C2DUi::Create(set[TITLEUI_PLAYER2ICON]);

	// �s�G��1�̃A�C�R������
	pC2dui[TITLEUI_PIEN1] = C2DUi::Create(set[TITLEUI_PIEN1]);

	// �s�G��2�̃A�C�R������
	pC2dui[TITLEUI_PIEN2] = C2DUi::Create(set[TITLEUI_PIEN2]);

	// �g�嗦�̐ݒ�
	m_Scal.fScalChange = 1.2f;
	// �^�C���`�F���W
	m_Scal.nTimeChange = 30;
	// �g�嗦����
	m_Scal.fScalDiff = (m_Scal.fScalChange - m_Scal.fScal) / m_Scal.nTimeChange;
	// �����T�C�Y
	m_Scal.OriginSize = D3DXVECTOR2(SIZEORIGIN, SIZEORIGIN);

	// �p�x�̐ݒ�
	m_fRotChange[0] = ROTCHANGE;
	m_fRotChange[1] = -ROTCHANGE;

	this->m_State = STATE_NORMAL;

	CMode::Init(STATE_NORMAL, 30);
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CTitle::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CTitle::Update(void)
{
	// �T�C�Y�ύX����
	Update_SizeChange();
	// �p�x�ύX����
	Update_RotChange();
	if (this->m_State == STATE_NORMAL)
	{
		// �t�F�[�h�X�V����
		for (int nCntUi = 0; nCntUi < TITLEUI_MAX; nCntUi++)
		{
			if (pC2dui[nCntUi]->GetFade() == NULL) continue;
			pC2dui[nCntUi]->GetFade()->Update(pC2dui[nCntUi]->GetImage());
		}
	}
	else if (this->m_State == STATE_OUT)
	{
		// �t���b�V���X�V����
		for (int nCntUi = 0; nCntUi < TITLEUI_MAX; nCntUi++)
		{
			if (pC2dui[nCntUi]->GetFlashing() == NULL) continue;
			pC2dui[nCntUi]->GetFlashing()->Update(pC2dui[nCntUi]);
		}
		if (this->m_nCntState == this->m_nMaxCntState)
		{
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_TUTORIAL);
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
void CTitle::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �T�C�Y�ύX�̍X�V����
//-------------------------------------------------------------------------------------------------------------
void CTitle::Update_SizeChange(void)
{
	// �ϐ��錾
	// UI�̎擾
	std::vector<CUi::UI_LOAD> Ui_Load = CUi::GetUi(CUi::UITYPE_TITLEUI);
	D3DXVECTOR2 * pSize;	// �T�C�Y���
	// �T�C�Y���̎擾
	pSize = pC2dui[TITLEUI_PLAYER1ICON]->GetImage()->GetSize();
	// �؂�ւ�OFF|�؂�ւ��ON/OFF
	if (m_Scal.nCntTimeChange == m_Scal.nTimeChange)
	{
		// �؂�ւ��ON/OFF
		m_Scal.bChange = !m_Scal.bChange;
		// �؂�ւ��OFF��
		if (!m_Scal.bChange)
		{
			// �g�嗦�̍���
			m_Scal.fScalDiff = (m_Scal.fScalChange - m_Scal.fScal) / m_Scal.nTimeChange;
			// �ڕW�̊g�嗦�ɋ����ω�
			m_Scal.fScal = 1.0f;
		}
		// �؂�ւ��ON��
		else
		{
			// �ڕW�̊g�嗦�ɋ����ω�
			m_Scal.fScal = m_Scal.fScalChange;
			// �g�嗦�̍���
			m_Scal.fScalDiff = (1.0f - m_Scal.fScalChange) / m_Scal.nTimeChange;
		}
		// �J�E���g������
		m_Scal.nCntTimeChange = 0;
		// �֐��𔲂���
		return;
	}
	// �؂�ւ�鎞�ԉ��Z
	m_Scal.nCntTimeChange++;
	// �g�嗦�̕ω�
	m_Scal.fScal += m_Scal.fScalDiff;
	// �T�C�Y�̕ω�
	*pSize = m_Scal.OriginSize * m_Scal.fScal;
	// ���_�J���[�̐ݒ�
	pC2dui[TITLEUI_PLAYER1ICON]->GetImage()->UpdateVatexPosition();
	pSize = NULL;
	// �T�C�Y���̎擾
	pSize = pC2dui[TITLEUI_PLAYER2ICON]->GetImage()->GetSize();
	// �T�C�Y�̕ω�
	*pSize = m_Scal.OriginSize * m_Scal.fScal;
	// ���_�J���[�̐ݒ�
	pC2dui[TITLEUI_PLAYER2ICON]->GetImage()->UpdateVatexPosition();

}

//-------------------------------------------------------------------------------------------------------------
// �p�x�ύX�̍X�V����
//-------------------------------------------------------------------------------------------------------------
void CTitle::Update_RotChange(void)
{
	// �ϐ��錾
	// UI�̎擾
	std::vector<CUi::UI_LOAD> Ui_Load = CUi::GetUi(CUi::UITYPE_TITLEUI);
	float fRot;	// �T�C�Y���
	// �T�C�Y���̎擾
	fRot = pC2dui[TITLEUI_PIEN1]->GetImage()->GetRotation();
	// �p�x�̉��Z
	fRot += m_fRotChange[0];
	// �p�x�̌��x�ݒ�
	if (fRot < -ROTLIMIT)
	{
		m_fRotChange[0] *= -1;
	}
	else if (fRot > ROTLIMIT)
	{
		m_fRotChange[0] *= -1;
	}
	// �p�x�̐ݒ�
	pC2dui[TITLEUI_PIEN1]->GetImage()->SetRotation(fRot);
	pC2dui[TITLEUI_PIEN1]->GetImage()->UpdateVatexPosition();
	// �T�C�Y���̎擾
	fRot = pC2dui[TITLEUI_PIEN2]->GetImage()->GetRotation();
	// �p�x�̉��Z
	fRot += m_fRotChange[1];
	// �p�x�̌��x�ݒ�
	if (fRot < -ROTLIMIT)
	{
		m_fRotChange[1] *= -1;
	}
	else if (fRot > ROTLIMIT)
	{
		m_fRotChange[1] *= -1;
	}
	// �p�x�̐ݒ�
	pC2dui[TITLEUI_PIEN2]->GetImage()->SetRotation(fRot);
	pC2dui[TITLEUI_PIEN2]->GetImage()->UpdateVatexPosition();
}
