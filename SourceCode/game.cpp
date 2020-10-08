//*************************************************************************************************************
//
// タイトル処理 [title.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "game.h"
#include "2DUI.h"

#include "player.h"

#include "renderer.h"
#include "keyboard.h"
#include "fade.h"

#include "NumericString.h"

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CGame * CGame::Create(void)
{
	CGame *pTitle = new CGame;
	pTitle->Init();
	return pTitle;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CGame::Init(void)
{
	C2DUi::SETING2DUI set;
	set.bDisp = true;
	set.col = MYLIB_D3DXCOR_SET;
	set.fRotation = 0.0f;
	set.mask.unMask = 0;
	set.nTextureID = 3;
	set.nValue = 0;
	set.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	set.size = D3DXVECTOR2(640.0f, 360.0f);
	pC2dui[UI_BG] = C2DUi::Create(set,CScene::PRIORITY_BG);

	C2DUi::SETING2DUI set2;
	set2.bDisp = true;
	set2.col = MYLIB_D3DXCOR_SET;
	set2.fRotation = 0.0f;
	set2.mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_NUMBER;
	set2.nTextureID = 1;
	set2.nValue = 90;
	set2.pos = D3DXVECTOR3(620.0f, 70.0f, 0.0f);
	set2.size = D3DXVECTOR2(20.0f, 60.0f);
	pC2dui[UI_TIMER] = C2DUi::Create(set2, CScene::PRIORITY_2DUI);

	pC2dui[UI_TIMER]->GetNumericString()->m_nAddSign = -1;

	pC2dui[UI_TIMER] ->GetFade()->bLoop = true;
	pC2dui[UI_TIMER] ->GetFade()->nTiming = 20;
	pC2dui[UI_TIMER] ->GetFade()->fChangeValue = 1.0f / pC2dui[UI_TIMER]->GetFade()->nTiming;
	pC2dui[UI_TIMER] ->GetFade()->nAddSign = 1;

	D3DXVECTOR3 pos = D3DXVECTOR3(100.0f, 600.0f, 0.0f);
	D3DXVECTOR2 size = D3DXVECTOR3(50.0f, 50.0f, 0.0f);

	m_pPlayer = CPlayer::Create(pos, size);


	CMode::Init(STATE_NORMAL, 10);
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CGame::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CGame::Update(void)
{
	switch (this->m_State)
	{
		MLB_CASE(STATE_NORMAL)	UpdateNormal();// 通常更新
		MLB_CASE(STATE_OUT)UpdateOut();// アウト更新
	MLB_DEFAULT:;
	}

#ifdef _DEBUG
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_RESULT);
	}
#endif

}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CGame::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 通常更新
//-------------------------------------------------------------------------------------------------------------
void CGame::UpdateNormal(void)
{
	switch (CManager::GetRenderer().GetFade()->GetFadeState())
	{
		MLB_CASE(CFade::FADE_NONE)
			if (m_nCntState == 60) {
				pC2dui[UI_TIMER]->GetNumericString()->AddValue();
				pC2dui[UI_TIMER]->GetNumericString()->Update();
				m_nCntState = 0;
			}
			else {
				m_nCntState++;
			}
			if (pC2dui[UI_TIMER]->GetNumericString()->m_nValue <= 0) {
				this->SetState(STATE_OUT);
				m_nCntState = 0;
			}
			else if (pC2dui[UI_TIMER]->GetNumericString()->m_nValue <= 10) {
				pC2dui[UI_TIMER]->GetFade()->Update(pC2dui[UI_TIMER]->GetNumericString());
			}
		
			MLB_DEFAULT:break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// アウト更新
//-------------------------------------------------------------------------------------------------------------
void CGame::UpdateOut(void)
{
	if (this->m_nCntState == this->m_nMaxCntState)
	{
		CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_RESULT);
	}
	this->m_nCntState++;
}
