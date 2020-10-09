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
	// 変数宣言
	C2DUi::SETING2DUI set[2];	// UI設定用変数
	// 2Dエフェクト生成
	C2DEffect::Create();
	// UIの取得
	std::vector<CUi::UI_LOAD> Ui_Load = CUi::GetUi(CUi::UITYPE_TITLEUI);
	// UI全体の設定
	for (int nCntUi = 0; nCntUi < (signed)Ui_Load.size(); nCntUi++)
	{
		set[nCntUi].bDisp = true;
		set[nCntUi].col = Ui_Load[nCntUi].col;
		set[nCntUi].fRotation = Ui_Load[nCntUi].fRot;
		set[nCntUi].nTextureID = Ui_Load[nCntUi].nTexType;
		set[nCntUi].pos = Ui_Load[nCntUi].pos;
		set[nCntUi].size = Ui_Load[nCntUi].size;
	}
	// タイトルの設定
	// タイトルの生成
	C2DUi::Create(set[0]);

	// プレスエンターの設定
	set[1].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	set[1].nValue = 123456789;
	// プレスエンターの生成
	pC2dui = C2DUi::Create(set[1]);
	pC2dui->GetFade()->bLoop = true;
	pC2dui->GetFade()->nTiming = 30;
	pC2dui->GetFade()->fChangeValue = 1.0f / pC2dui->GetFade()->nTiming;
	pC2dui->GetFade()->nAddSign = 1;
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
#ifdef _DEBUG
	// テストパーティクル
	if (CManager::GetKeyboard().GetTrigger(DIK_SPACE))
	{
		C3DParticle::Create(C3DParticle::PARTICLE_ID_EXPLOSION, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
#endif // _DEBUG

}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CTitle::Draw(void)
{
}
