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
#include "result.h"

#include "score.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義 
//-------------------------------------------------------------------------------------------------------------
#define GAME_GAGE_MAX			150.0f
#define GAME_EXPOSURE_RANGE		277.0f
#define GAME_TIME				30



#define GAME_CATIN_IN			300
#define GAME_CATIN_STOP			420
#define GAME_CATIN_OUT			540
#define GAME_CATIN_PLAY			560		// 更新再開


//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CGame::PLAYEROFFSET CGame::m_offset[PLAYER_MAX] = {
	{ D3DXVECTOR3(250.0f, 550.0f, 0.0f),D3DXVECTOR2(120.0f, 160.0f) },
	{ D3DXVECTOR3(1030.0f, 550.0f, 0.0f),D3DXVECTOR2(120.0f, 160.0f) },
};
CGame::BOMBOFFSET CGame::m_Bomoffset[PLAYER_MAX] =
{
	{ D3DXVECTOR3(180.0f, 480.0f, 0.0f),D3DXVECTOR3(300.0f, 500.0f, 0.0f),D3DXVECTOR2(50.0f, 50.0f) },
	{ D3DXVECTOR3(1080.0f, 500.0f, 0.0f),D3DXVECTOR3(980.0f, 500.0f, 0.0f),D3DXVECTOR2(50.0f, 50.0f) },
};

CGame::GAMEWINSTATE CGame::m_GameWinState = WINSTATE_1P2P;
CGame::PLAYERPIEN   CGame::m_PlayerPien;
CGame::PLAYERDIST	CGame::m_PlaerDist;
float CGame::m_fAlphaValue[2] =
{
	1.0f / (GAME_CATIN_IN),
	1.0f / (GAME_CATIN_OUT - GAME_CATIN_STOP)
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
		C2DUi::SETING2DUI(C2DUi::MASK_FADE | C2DUi::MASK_NUMBER,CTexture::NAME_NUMBER, true, D3DXVECTOR3(620.0f, 70.0f, 0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(35.0f, 105.0f),0.0f, GAME_TIME),

		C2DUi::SETING2DUI(0,CTexture::NAME_POWERGAUGE, true, D3DXVECTOR3(52.0f,650.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(410.0f,60.0f),0.0f, 1000),
		C2DUi::SETING2DUI(0,CTexture::NAME_ANGLEGAUGE, true, D3DXVECTOR3(100.0f,530.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(50.0f,465.0f),0.0f, 1000),
		C2DUi::SETING2DUI(0,CTexture::NAME_POWERGAUGE, true, D3DXVECTOR3(1228.0f,650.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(410.0f,60.0f),0.0f, 1000),
		C2DUi::SETING2DUI(0,CTexture::NAME_ANGLEGAUGE, true, D3DXVECTOR3(1180.0f,530.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(50.0f,465.0f),0.0f, 1000),

		C2DUi::SETING2DUI(0,CTexture::NAME_POWERGAUGE_1P, true, D3DXVECTOR3(330.0f,650.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(300.0f,60.0f),0.0f, 0),
		C2DUi::SETING2DUI(0,CTexture::NAME_ANGLEGAUGE_1P, true, D3DXVECTOR3(100.0f,300.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(60.0f,280.0f),0.0f, 0),
		C2DUi::SETING2DUI(0,CTexture::NAME_POWERGAUGE_2P, true, D3DXVECTOR3(950.0f,650.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(300.0f,60.0f),0.0f, 0),
		C2DUi::SETING2DUI(0,CTexture::NAME_ANGLEGAUGE_2P, true, D3DXVECTOR3(1180.0f,300.0f,0.0f), MYLIB_D3DXCOR_SET, D3DXVECTOR2(60.0f,280.0f),0.0f, 0),

		C2DUi::SETING2DUI(0,CTexture::NAME_PY03, false, D3DXVECTOR3(300.0f,360.0f,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,0.0f), D3DXVECTOR2(350.0f,350.0f),0.0f, 0),
		C2DUi::SETING2DUI(0,CTexture::NAME_PB03, false, D3DXVECTOR3(980.0f,360.0f,0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,0.0f), D3DXVECTOR2(350.0f,350.0f),0.0f, 0),
	};
	
	CScene::PRIORITY pri[UI_MAX] = {
		CScene::PRIORITY_BG,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
		CScene::PRIORITY_2DUI,
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

		m_Catin[nCntPlayer].bCatin = false;
		m_Catin[nCntPlayer].nCntCaatin = 0;
	}
	for (int nCntPlayer = 0; nCntPlayer < PLAYER_MAX; nCntPlayer++)
	{
		m_pBomb[nCntPlayer] = CBomb::Create(m_Bomoffset[nCntPlayer].pos, m_Bomoffset[nCntPlayer].size, nCntPlayer);
	}
	m_pPlayer[0]->GetScene2D()->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_PY00));
	m_pPlayer[1]->GetScene2D()->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_PB00));
	CMode::Init(STATE_NORMAL, 30);

	for (int nCntaGage = 0; nCntaGage < SCAL_P2_GAGE_MAX ; nCntaGage++)
	{
		m_fGageScal[nCntaGage] = 0.0f;
		GageScaForceShuffle(&m_fGageScaForce[nCntaGage]);
		m_fGageScaSign[nCntaGage] = 1;
		m_bMoveGage[nCntaGage] = true;
	}


	m_bDuelist = false;
	m_nCntDuelist = 0;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CGame::Uninit(void)
{
	CResult::SetWinPlayer(m_GameWinState);
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CGame::Update(void)
{


	if (m_bDuelist == false)
	{
		switch (this->m_State)
		{
			MLB_CASE(STATE_NORMAL)UpdateNormal();// 通常更新
			MLB_CASE(STATE_OUT)UpdateOut();// アウト更新
		MLB_DEFAULT:;
		}
	}
	else
	{
		CatinProc();

		if (m_nCntDuelist++ == GAME_CATIN_PLAY)
		{
			m_bDuelist = false;
		}
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
			for (int nCntPlayer = 0; nCntPlayer < PLAYER_MAX; nCntPlayer++)
			{
				m_pBomb[nCntPlayer]->UpdateMoveing();
			}


			CatinProc();
			if (m_nCntState == 60) {
				pC2dui[UI_TIMER]->GetNumericString()->AddValue();
				pC2dui[UI_TIMER]->GetNumericString()->Update();
				m_nCntState = 0;
			}
			else {
				m_nCntState++;
			}
			if (pC2dui[UI_TIMER]->GetNumericString()->m_nValue < 0) {
				pC2dui[UI_TIMER]->GetNumericString()->m_nValue = 0;
				this->SetState(STATE_OUT);
				m_nCntState = 0;

				m_GameWinState = WINSTATE_1P2P;
				m_PlayerPien.bPlayer1Pien = true;
				m_PlayerPien.bPlayer2Pien = true;

				m_PlaerDist.fPlayer1Dist = 0.0f;
				m_PlaerDist.fPlayer2Dist = 0.0f;

				CScore::SetPlayerScore(PLAYER_1, 0.0f);
				CScore::SetPlayerScore(PLAYER_2, 0.0f);

			}
			else if (pC2dui[UI_TIMER]->GetNumericString()->m_nValue <= 5) {
				pC2dui[UI_TIMER]->GetFade()->Update(pC2dui[UI_TIMER]->GetNumericString());
			}
			// ゲージの更新
			GageUpdate();

			if (m_pBomb[PLAYER_1]->GetState() ==CBomb::STATE_LANDING &&
				m_pBomb[PLAYER_2]->GetState() == CBomb::STATE_LANDING)
			{
				float fDistance1 =  m_pBomb[PLAYER_1]->GetDistance();
				float fDistance2 =  m_pBomb[PLAYER_2]->GetDistance();

				CScore::SetPlayerScore(PLAYER_1, fDistance1);
				CScore::SetPlayerScore(PLAYER_2, fDistance2);

				m_PlaerDist.fPlayer1Dist = fDistance1;
				m_PlaerDist.fPlayer2Dist = fDistance2;

				if (fDistance1 == fDistance2)
				{
					m_GameWinState = WINSTATE_1P2P;
				}
				else if(fDistance1 > fDistance2)
				{
					m_GameWinState = WINSTATE_1P;
				}
				else
				{
					m_GameWinState = WINSTATE_2P;
				}

				// プレイヤー1とプレエイヤー2の爆弾の距離
				float fDistanceP1_B2 = m_pPlayer[PLAYER_1]->GetPos().x - m_pBomb[PLAYER_2]->GetPos().x;
				bool bExprosureP1 = (abs(fDistanceP1_B2) >= GAME_EXPOSURE_RANGE);
				m_pPlayer[PLAYER_1]->SetState(bExprosureP1 ? CPlayer::STATE_NOTEXPOSURE : CPlayer::STATE_EXPOSURE);

				// プレイヤー2とプレエイヤー1の爆弾の距離
				float fDistanceP2_B1 = m_pPlayer[PLAYER_2]->GetPos().x - m_pBomb[PLAYER_1]->GetPos().x;
				bool bExprosureP2 = (abs(fDistanceP2_B1) >= GAME_EXPOSURE_RANGE);
				m_pPlayer[PLAYER_2]->SetState((bExprosureP2) ? CPlayer::STATE_NOTEXPOSURE : CPlayer::STATE_EXPOSURE);

				cout << "とんだ距離1 == " << fDistance1 << "\n";
				cout << "とんだ距離2 == " << fDistance2 << "\n";

				cout << "ピエン == " << fDistanceP1_B2 << "\n";
				cout << "ピエン == " << fDistanceP2_B1 << "\n";

				m_PlayerPien.bPlayer1Pien = bExprosureP1;
				m_PlayerPien.bPlayer2Pien = bExprosureP2;

				this->SetState(STATE_OUT);
				m_nCntState = 0;
			}

			if (CManager::GetKeyboard().GetTrigger(DIK_LSHIFT))
			{
				m_bMoveGage[SCAL_P1_GAGE_X] = false;
				m_bMoveGage[SCAL_P1_GAGE_Y] = false;
				m_pBomb[PLAYER_1]->Fire(m_fGageScal[SCAL_P1_GAGE_X], m_fGageScal[SCAL_P1_GAGE_Y], m_Bomoffset[PLAYER_1].fire);


				m_pPlayer[PLAYER_1]->GetScene2D()->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_PY01));
				if (m_fGageScal[SCAL_P1_GAGE_X] >= 0.75f &&
					m_fGageScal[SCAL_P1_GAGE_Y] >= 0.75f)
				{
					pC2dui[UI_CUTIN_P1]->SetDisp(true);
					m_Catin[PLAYER_1].bCatin = true;

					m_bDuelist = true;
				}

				pC2dui[UI_CUTIN_P1]->SetDisp(true);
				m_Catin[PLAYER_1].bCatin = true;
				m_bDuelist = true;
				m_nCntDuelist = 0;
			}

			if (CManager::GetKeyboard().GetTrigger(DIK_RSHIFT))
			{
				m_bMoveGage[SCAL_P2_GAGE_X] = false;
				m_bMoveGage[SCAL_P2_GAGE_Y] = false;
				m_pBomb[PLAYER_2]->Fire(m_fGageScal[SCAL_P2_GAGE_X], m_fGageScal[SCAL_P2_GAGE_Y], m_Bomoffset[PLAYER_2].fire);

				m_pPlayer[PLAYER_2]->GetScene2D()->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_PB01));

				if (m_fGageScal[SCAL_P2_GAGE_X] >= 0.75f &&
					m_fGageScal[SCAL_P2_GAGE_Y] >= 0.75f)
				{
					pC2dui[UI_CUTIN_P2]->SetDisp(true);
					m_Catin[PLAYER_2].bCatin = true;

					m_bDuelist = true;
				}
				pC2dui[UI_CUTIN_P2]->SetDisp(true);
				m_Catin[PLAYER_2].bCatin = true;
				m_bDuelist = true;
				m_nCntDuelist = 0;
			}
			MLB_DEFAULT:break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// アウト更新
//-------------------------------------------------------------------------------------------------------------
void CGame::UpdateOut(void)
{
	m_pBomb[PLAYER_1]->Expansion();
	m_pBomb[PLAYER_2]->Expansion();

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
		pC2dui[UI_P1_GAGE_X]->GetImage()->SetSizeX(410.0f *m_fGageScal[SCAL_P1_GAGE_X]);
		pC2dui[UI_P1_GAGE_X]->GetImage()->UpdateVatexPosition();
		D3DXCOLOR *col = pC2dui[UI_P1_GAGE_X]->GetImage()->GetColor();
		col->b = 1.0f - m_fGageScal[SCAL_P1_GAGE_X];
		col->g = 1.0f - m_fGageScal[SCAL_P1_GAGE_X];
		pC2dui[UI_P1_GAGE_X]->GetImage()->UpdateVatexColor();

	}
	if (m_bMoveGage[SCAL_P1_GAGE_Y] == true)
	{
		pC2dui[UI_P1_GAGE_Y]->GetImage()->SetSizeY(465.0f *m_fGageScal[SCAL_P1_GAGE_Y]);
		pC2dui[UI_P1_GAGE_Y]->GetImage()->UpdateVatexPosition();

		D3DXCOLOR *col = pC2dui[UI_P1_GAGE_Y]->GetImage()->GetColor();
		col->b = 1.0f - m_fGageScal[SCAL_P1_GAGE_Y];
		col->g = 1.0f - m_fGageScal[SCAL_P1_GAGE_Y];
		pC2dui[UI_P1_GAGE_Y]->GetImage()->UpdateVatexColor();
	}

	if (m_bMoveGage[SCAL_P2_GAGE_X] == true)
	{
		pC2dui[UI_P2_GAGE_X]->GetImage()->SetSizeX(410.0f *m_fGageScal[SCAL_P2_GAGE_X]);
		pC2dui[UI_P2_GAGE_X]->GetImage()->UpdateVatexPosition();

		D3DXCOLOR *col = pC2dui[UI_P2_GAGE_X]->GetImage()->GetColor();
		col->b = 1.0f - m_fGageScal[SCAL_P2_GAGE_X];
		col->g = 1.0f - m_fGageScal[SCAL_P2_GAGE_X];
		pC2dui[UI_P2_GAGE_X]->GetImage()->UpdateVatexColor();
	}
	if (m_bMoveGage[SCAL_P2_GAGE_Y] == true)
	{
		pC2dui[UI_P2_GAGE_Y]->GetImage()->SetSizeY(465.0f *m_fGageScal[SCAL_P2_GAGE_Y]);
		pC2dui[UI_P2_GAGE_Y]->GetImage()->UpdateVatexPosition();

		D3DXCOLOR *col = pC2dui[UI_P2_GAGE_Y]->GetImage()->GetColor();
		col->b = 1.0f - m_fGageScal[SCAL_P2_GAGE_Y];
		col->g = 1.0f - m_fGageScal[SCAL_P2_GAGE_Y];
		pC2dui[UI_P2_GAGE_Y]->GetImage()->UpdateVatexColor();
	}
}

//-------------------------------------------------------------------------------------------------------------
// ゲージフォースのシャッフル
//-------------------------------------------------------------------------------------------------------------
void CGame::GageScaForceShuffle(float * pGageScalForce)
{
	*pGageScalForce = (rand() % 5+ 3) * 0.01f;
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

void CGame::CatinProc(void)
{
	for (int nCntPlayer = 0; nCntPlayer < PLAYER_MAX; nCntPlayer++)
	{
		if (m_Catin[nCntPlayer].bCatin == true)
		{
			m_Catin[nCntPlayer].nCntCaatin++;

			int nIndex = (nCntPlayer == PLAYER_1) ? UI_CUTIN_P1 : UI_CUTIN_P2;

			if (m_Catin[nCntPlayer].nCntCaatin <= GAME_CATIN_IN)
			{
				D3DXVECTOR2 *pSize =  pC2dui[nIndex]->GetImage()->GetSize();
				float *pAlpha = &pC2dui[nIndex]->GetImage()->GetColor()->a;
				*pAlpha += m_fAlphaValue[0];
				pC2dui[nIndex]->GetImage()->UpdateVatexColor();
			}
			if (m_Catin[nCntPlayer].nCntCaatin >= GAME_CATIN_STOP)
			{
				D3DXVECTOR2 *pSize = pC2dui[nIndex]->GetImage()->GetSize();
				float *pAlpha = &pC2dui[nIndex]->GetImage()->GetColor()->a;
				*pAlpha -= m_fAlphaValue[1];
				pSize->x += 5.0f;
				pSize->y += 5.0f;
				pC2dui[nIndex]->GetImage()->UpdateVatexColor();
				pC2dui[nIndex]->GetImage()->UpdateVatexPosition();
			}
			if (m_Catin[nCntPlayer].nCntCaatin == GAME_CATIN_OUT)
			{
				D3DXVECTOR2 *pSize = pC2dui[nIndex]->GetImage()->GetSize();
				pSize->x = 0.0f;
				pSize->y = 0.0f;
				pC2dui[nIndex]->GetImage()->UpdateVatexPosition();
				m_Catin[nCntPlayer].bCatin = false;
				pC2dui[nIndex]->SetDisp(false);
			}
		}
	}
}
