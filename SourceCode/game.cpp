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
#include "Bomb.h"

#include "renderer.h"
#include "keyboard.h"
#include "fade.h"

#include "NumericString.h"
#include "Scene2D.h"

#include "texture.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define GAME_GAGE_MAX	200.0f

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CGame::PLAYEROFFSET CGame::m_offset[PLAYER_MAX] = {
	{ D3DXVECTOR3(100.0f, 550.0f, 0.0f),D3DXVECTOR2(50.0f, 50.0f) },
	{ D3DXVECTOR3(100.0f, 550.0f, 0.0f),D3DXVECTOR2(50.0f, 50.0f) },
};

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
	C2DUi::SETING2DUI set[UI_MAX] =
	{
		C2DUi::SETING2DUI(0,CTexture::NAME_GAMEBG , true, D3DXVECTOR3(640.0f, 360.0f, 0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(640.0f, 360.0f),0.0f, 0),
		C2DUi::SETING2DUI(C2DUi::MASK_FADE | C2DUi::MASK_NUMBER, 1, true, D3DXVECTOR3(620.0f, 70.0f, 0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(20.0f, 60.0f),0.0f, 90),

		C2DUi::SETING2DUI(0,CTexture::NAME_GAMEBG, true, D3DXVECTOR3(52.0f,650.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(410.0f,40.0f),0.0f, 1000),
		C2DUi::SETING2DUI(0,CTexture::NAME_GAMEBG, true, D3DXVECTOR3(100.0f,530.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(40.0f,465.0f),0.0f, 1000),
		C2DUi::SETING2DUI(0,CTexture::NAME_GAMEBG, true, D3DXVECTOR3(1228.0f,650.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(410.0f,40.0f),0.0f, 1000),
		C2DUi::SETING2DUI(0,CTexture::NAME_GAMEBG, true, D3DXVECTOR3(1180.0f,530.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(40.0f,465.0f),0.0f, 1000),

		C2DUi::SETING2DUI(0,CTexture::NAME_POWERGAUGE_1P, true, D3DXVECTOR3(330.0f,650.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(300.0f,60.0f),0.0f, 0),
		C2DUi::SETING2DUI(0,CTexture::NAME_ANGLEGAUGE_1P, true, D3DXVECTOR3(100.0f,300.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(60.0f,280.0f),0.0f, 0),
		C2DUi::SETING2DUI(0,CTexture::NAME_POWERGAUGE_2P, true, D3DXVECTOR3(950.0f,650.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(300.0f,60.0f),0.0f, 0),
		C2DUi::SETING2DUI(0,CTexture::NAME_ANGLEGAUGE_2P, true, D3DXVECTOR3(1180.0f,300.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(60.0f,280.0f),0.0f, 0),
	};
	
	CScene::PRIORITY pri[UI_MAX] = {
		CScene::PRIORITY_BG,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_BG,
		CScene::PRIORITY_BG,
		CScene::PRIORITY_BG,
		CScene::PRIORITY_BG,
	};
	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{
		pC2dui[nCntUI] = C2DUi::Create(set[nCntUI], pri[nCntUI]);
	}

	pC2dui[UI_TIMER]->GetNumericString()->m_nAddSign = -1;
	pC2dui[UI_TIMER]->GetFade()->bLoop = true;
	pC2dui[UI_TIMER]->GetFade()->nTiming = 20;
	pC2dui[UI_TIMER]->GetFade()->fChangeValue = 1.0f / pC2dui[UI_TIMER]->GetFade()->nTiming;
	pC2dui[UI_TIMER]->GetFade()->nAddSign = 1;


	pC2dui[UI_P1_GAGE_X]->GetImage()->SetOriginType(ORIGINVERTEXTYPE_CENTERLEFT); 
	pC2dui[UI_P1_GAGE_X]->GetImage()->UpdateVatexPosition();

	pC2dui[UI_P1_GAGE_Y]->GetImage()->SetOriginType(ORIGINVERTEXTYPE_LOWERCENTER);
	pC2dui[UI_P1_GAGE_Y]->GetImage()->UpdateVatexPosition();

	pC2dui[UI_P2_GAGE_X]->GetImage()->SetOriginType(ORIGINVERTEXTYPE_CENTERRIGHT);
	pC2dui[UI_P2_GAGE_X]->GetImage()->UpdateVatexPosition();

	pC2dui[UI_P2_GAGE_Y]->GetImage()->SetOriginType(ORIGINVERTEXTYPE_LOWERCENTER);
	pC2dui[UI_P2_GAGE_Y]->GetImage()->UpdateVatexPosition();

	// プレイヤーの最大ループ
	for (int nCntPlayer = 0; nCntPlayer < PLAYER_MAX; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = CPlayer::Create(m_offset[nCntPlayer].pos, m_offset[nCntPlayer].size);
		m_pBomb[nCntPlayer] = CBomb::Create(m_offset[nCntPlayer].pos, m_offset[nCntPlayer].size, nCntPlayer);
	}

	CMode::Init(STATE_NORMAL, 10);


	for (int nCntaGage = 0; nCntaGage < SCAL_P2_GAGE_MAX ; nCntaGage++)
	{
		m_fGageScal[nCntaGage] = 0.0f;
		GageScaForceShuffle(&m_fGageScaForce[nCntaGage]);
		m_fGageScaSign[nCntaGage] = 1;
		m_bMoveGage[nCntaGage] = true;
	}
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
			if (pC2dui[UI_TIMER]->GetNumericString()->m_nValue < 0) {
				this->SetState(STATE_OUT);
				m_nCntState = 0;
			}
			else if (pC2dui[UI_TIMER]->GetNumericString()->m_nValue <= 10) {
				pC2dui[UI_TIMER]->GetFade()->Update(pC2dui[UI_TIMER]->GetNumericString());
			}
			// ゲージの更新
			GageUpdate();

			if (CManager::GetKeyboard().GetTrigger(DIK_RSHIFT))
			{
				m_bMoveGage[SCAL_P2_GAGE_X] = false;
				m_bMoveGage[SCAL_P2_GAGE_Y] = false;
				m_pBomb[PLAYER_2]->Fire(m_fGageScaForce[SCAL_P2_GAGE_X], m_fGageScaForce[SCAL_P2_GAGE_Y]);
			}

			if (CManager::GetKeyboard().GetTrigger(DIK_LSHIFT))
			{
				m_bMoveGage[SCAL_P1_GAGE_X] = false;
				m_bMoveGage[SCAL_P1_GAGE_Y] = false;
				m_pBomb[PLAYER_1]->Fire(m_fGageScaForce[SCAL_P1_GAGE_X], m_fGageScaForce[SCAL_P1_GAGE_Y]);
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

//-------------------------------------------------------------------------------------------------------------
// ゲージの更新
//-------------------------------------------------------------------------------------------------------------
void CGame::GageUpdate(void)
{
	for (int nCntaGage = 0; nCntaGage < SCAL_P2_GAGE_MAX; nCntaGage++)
	{
		if (m_bMoveGage[nCntaGage] == true)
		{
			m_fGageScal[nCntaGage] += m_fGageScaForce[nCntaGage] * m_fGageScaSign[nCntaGage];

			if (GageScalClamp(&m_fGageScal[nCntaGage], &m_fGageScaSign[nCntaGage]))
			{
				GageScaForceShuffle(&m_fGageScaForce[nCntaGage]);
			}
		}
	}

	if (m_bMoveGage[SCAL_P1_GAGE_X] == true)
	{
		pC2dui[UI_P1_GAGE_X]->GetImage()->SetSizeX(410.0f *m_fGageScal[0]);
		pC2dui[UI_P1_GAGE_X]->GetImage()->UpdateVatexPosition();
	}
	if (m_bMoveGage[SCAL_P1_GAGE_Y] == true)
	{
		pC2dui[UI_P1_GAGE_Y]->GetImage()->SetSizeY(465.0f *m_fGageScal[1]);
		pC2dui[UI_P1_GAGE_Y]->GetImage()->UpdateVatexPosition();
	}

	if (m_bMoveGage[SCAL_P2_GAGE_X] == true)
	{
		pC2dui[UI_P2_GAGE_X]->GetImage()->SetSizeX(410.0f *m_fGageScal[2]);
		pC2dui[UI_P2_GAGE_X]->GetImage()->UpdateVatexPosition();
	}
	if (m_bMoveGage[SCAL_P2_GAGE_Y] == true)
	{
		pC2dui[UI_P2_GAGE_Y]->GetImage()->SetSizeY(465.0f *m_fGageScal[3]);
		pC2dui[UI_P2_GAGE_Y]->GetImage()->UpdateVatexPosition();
	}
}

//-------------------------------------------------------------------------------------------------------------
// ゲージフォースのシャッフル
//-------------------------------------------------------------------------------------------------------------
void CGame::GageScaForceShuffle(float * pGageScalForce)
{
	*pGageScalForce = (rand() % 5) * 0.02f;
}

bool CGame::GageScalClamp(float * pGageScal, int *pSign)
{
	if (*pGageScal <= 0.0f)
	{
		*pGageScal = 0.0f;
		*pSign *= -1;
		return true;
	}
	else if (*pGageScal >= 1.0f)
	{
		*pGageScal = 1.0f;
		*pSign *= -1;
		return true;
	}

	return false;

}
