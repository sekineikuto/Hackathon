//*************************************************************************************************************
//
// タイトル処理 [title.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "title.h"
#include "2DUI.h"
#include "keyboard.h"
#include "renderer.h"
#include "fade.h"
#include "3Dparticle.h"
#include "2Deffect.h"
#include "ui.h"

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CTitle * CTitle::Create(void)
{
	CTitle *pTitle = new CTitle;
	pTitle->Init();
	return pTitle;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CTitle::Init(void)
{
	// 2Dエフェクト生成
	C2DEffect::Create();
	// UIの取得
	std::vector<CUi::UI_LOAD> Ui_Load = CUi::GetUi(CUi::UITYPE_GAMEUI);
	C2DUi::SETING2DUI set[2];
	// 0番目
	set[0].bDisp = true;
	set[0].col = MYLIB_D3DXCOR_SET;
	set[0].fRotation = 0.0f;
	set[0].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	set[0].nTextureID = 2;
	set[0].nValue = 123456789;
	set[0].pos = D3DXVECTOR3(640.0f, 600.0f, 0.0f);
	set[0].size = D3DXVECTOR2(240.0f, 60.0f);
	pC2dui = C2DUi::Create(set[0]);
	pC2dui->GetFade()->bLoop = true;
	pC2dui->GetFade()->nTiming = 30;
	pC2dui->GetFade()->fChangeValue = 1.0f / pC2dui->GetFade()->nTiming;
	pC2dui->GetFade()->nAddSign = 1;

	// 1番目
	set[1].bDisp = true;
	set[1].col = Ui_Load[0].col;
	set[1].fRotation = Ui_Load[0].fRot;
	set[1].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	set[1].nTextureID = Ui_Load[0].nTexType;
	set[1].nValue = 123456789;
	set[1].pos = Ui_Load[0].pos;
	set[1].size = Ui_Load[0].size;

	pC2dui->GetFlashing()->m_nTiming = 3;

	C2DUi::Create(set[1]);
	this->m_State = STATE_NORMAL;

	CMode::Init(STATE_NORMAL, 30);
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CTitle::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CTitle::Update(void)
{
	if (this->m_State == STATE_NORMAL)
	{
		pC2dui->GetFade()->Update(pC2dui->GetImage());
	}
	else if (this->m_State == STATE_OUT)
	{
		pC2dui->GetFlashing()->Update(pC2dui);
		if (this->m_nCntState == this->m_nMaxCntState)
		{
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_TUTORIAL);
		}
		this->m_nCntState++;
	}
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		this->SetState(STATE_OUT);
	}
	if (CManager::GetKeyboard().GetTrigger(DIK_SPACE))
	{
		C3DParticle::Create(C3DParticle::PARTICLE_ID_UNKNOWN,D3DXVECTOR3(0.0f,0.0f,0.0f));
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CTitle::Draw(void)
{
}
