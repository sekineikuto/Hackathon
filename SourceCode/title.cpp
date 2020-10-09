//*************************************************************************************************************
//
// タイトル処理 [title.h]
// Author:KOKI NISHIYAMA
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
	C2DUi::SETING2DUI set[TITLEUI_MAX];	// UI設定用変数
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
	set[TITLEUI_NAME].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	// タイトルの生成
	pC2dui[TITLEUI_NAME] = C2DUi::Create(set[TITLEUI_NAME]);
	pC2dui[TITLEUI_NAME]->GetFade()->bLoop = true;
	pC2dui[TITLEUI_NAME]->GetFade()->nTiming = 90;
	pC2dui[TITLEUI_NAME]->GetFade()->fChangeValue = 1.0f / pC2dui[TITLEUI_NAME]->GetFade()->nTiming;
	pC2dui[TITLEUI_NAME]->GetFade()->nAddSign = 1;

	// プレスエンターの設定
	set[TITLEUI_PRESSENTERNAME].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	// プレスエンターの生成
	pC2dui[TITLEUI_PRESSENTERNAME] = C2DUi::Create(set[TITLEUI_PRESSENTERNAME]);
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->bLoop = true;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->nTiming = 30;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->fChangeValue = 1.0f / pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->nTiming;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->nAddSign = 1;
	// プレスエンターアイコンの設定
	set[TITLEUI_PRESSENTERICON].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	// プレスエンターのアイコン生成
	pC2dui[TITLEUI_PRESSENTERICON] = C2DUi::Create(set[TITLEUI_PRESSENTERICON]);
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->bLoop = true;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->nTiming = 30;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->fChangeValue = 1.0f / pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->nTiming;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->nAddSign = 1;
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
		pC2dui[TITLEUI_NAME]->GetFade()->Update(pC2dui[TITLEUI_NAME]->GetImage());
		pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->Update(pC2dui[TITLEUI_PRESSENTERNAME]->GetImage());
		pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->Update(pC2dui[TITLEUI_PRESSENTERICON]->GetImage());
	}
	else if (this->m_State == STATE_OUT)
	{
		pC2dui[TITLEUI_NAME]->GetFlashing()->Update(pC2dui[TITLEUI_NAME]);
		pC2dui[TITLEUI_PRESSENTERNAME]->GetFlashing()->Update(pC2dui[TITLEUI_PRESSENTERNAME]);
		pC2dui[TITLEUI_PRESSENTERICON]->GetFlashing()->Update(pC2dui[TITLEUI_PRESSENTERICON]);
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
