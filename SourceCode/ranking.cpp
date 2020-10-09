//*************************************************************************************************************
//
// ランキング処理 [ranking.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "ranking.h"
#include "2DUI.h"

#include "renderer.h"
#include "keyboard.h"
#include "fade.h"
#include "score.h"
#include "texture.h"
#include "result.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define SIZE_SCORE	(D3DXVECTOR2(40.0f, 80.0f))	// スコアのサイズ

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CRanking::m_scorePos[MAX_NUMRANK] =
{
	D3DXVECTOR3(SCREEN_WIDTH / 2 - 140.0f, 200.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2 - 140.0f, 300.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2 - 140.0f, 400.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2 - 140.0f, 500.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2 - 140.0f, 600.0f, 0.0f)
};

D3DXVECTOR3 CRanking::m_UIPos[CRanking::RANKUI_MAX] =
{
	D3DXVECTOR3(310.0f, 210.0f, 0.0f),
	D3DXVECTOR3(310.0f, 310.0f, 0.0f),
	D3DXVECTOR3(310.0f, 410.0f, 0.0f),
	D3DXVECTOR3(310.0f, 510.0f, 0.0f),
	D3DXVECTOR3(310.0f, 610.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, 80.0f, 0.0f)
};

D3DXVECTOR2 CRanking::m_UISize[CRanking::RANKUI_MAX] =
{
	D3DXVECTOR2(150.0f, 100.0f),
	D3DXVECTOR2(150.0f, 100.0f),
	D3DXVECTOR2(150.0f, 100.0f),
	D3DXVECTOR2(150.0f, 100.0f),
	D3DXVECTOR2(150.0f, 100.0f),
	D3DXVECTOR2(300.0f, 140.0f)
};

int CRanking::m_nPlayerScore = 0;

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CRanking * CRanking::Create(void)
{
	// メモリ確保
	CRanking *pRanking = new CRanking;
	// 初期化
	pRanking->Init();
	// 値を返す
	return pRanking;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CRanking::Init(void)
{
	int nWinPlayer = CResult::GetWinPlayer();
	// 勝者のスコア取得
	float fPlayerScore = CScore::GetPlayerScore(nWinPlayer - 1);

	for (int nCntScore = 0; nCntScore < MAX_NUMRANK; nCntScore++)
	{
		// フラッシュの設定
		bool bFlash = false;
		// プレイヤーのスコアの場合
		if (CScore::GetDefaultScore(nCntScore) == fPlayerScore)
		{
			// 番号を保存
			m_nPlayerScore = nCntScore;
			// フラッシュを設定
			bFlash = true;
		}

		// 生成
		m_pScore[nCntScore] = CScore::Create(m_scorePos[nCntScore], SIZE_SCORE, CScore::GetDefaultScore(nCntScore), bFlash);
	}

	C2DUi::SETING2DUI set;
	set.bDisp = true;
	set.col = MYLIB_D3DXCOR_SET;
	set.fRotation = 0.0f;
	set.nValue = 123456789;

	for (int nCntUI = 0; nCntUI < RANKUI_MAX; nCntUI++)
	{
		// UI
		set.nTextureID = CTexture::NAME_1ST + nCntUI;
		set.pos = m_UIPos[nCntUI];
		set.size = m_UISize[nCntUI];
		m_pUI[nCntUI] = C2DUi::Create(set);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CRanking::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMRANK; nCntScore++)
	{
		if (m_pScore[nCntScore])
		{
			m_pScore[nCntScore]->Uninit();
			delete m_pScore[nCntScore];
			m_pScore[nCntScore] = nullptr;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CRanking::Update(void)
{
	m_pScore[m_nPlayerScore];

	for (int nCntScore = 0; nCntScore < MAX_NUMRANK; nCntScore++)
	{
		if (m_pScore[nCntScore])
			m_pScore[nCntScore]->Update();
	}

	// キー入力でタイトルへ
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
		CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_TITLE);
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CRanking::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMRANK; nCntScore++)
	{
		if (m_pScore[nCntScore])
			m_pScore[nCntScore]->Draw();
	}
}
