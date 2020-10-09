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
#include "Scene2D.h"
#include "sound.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define SIZEORIGIN	(180)		// 元サイズ
#define ROTLIMIT	(0.75f)			// 角度の限界値
#define ROTCHANGE	(0.02f)			// 角度の限界値

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
	// タイトル背景の生成
	pC2dui[TITLEUI_BG] = C2DUi::Create(set[TITLEUI_BG]);

	// タイトルの設定
	set[TITLEUI_NAME].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	// タイトルの生成
	pC2dui[TITLEUI_NAME] = C2DUi::Create(set[TITLEUI_NAME]);
	pC2dui[TITLEUI_NAME]->GetFlashing()->m_nTiming = 3;
	pC2dui[TITLEUI_NAME]->GetFade()->bLoop = true;
	pC2dui[TITLEUI_NAME]->GetFade()->nTiming = 90;
	pC2dui[TITLEUI_NAME]->GetFade()->fChangeValue = 1.0f / pC2dui[TITLEUI_NAME]->GetFade()->nTiming;
	pC2dui[TITLEUI_NAME]->GetFade()->nAddSign = 1;

	// プレスエンターの設定
	set[TITLEUI_PRESSENTERNAME].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	// プレスエンターの生成
	pC2dui[TITLEUI_PRESSENTERNAME] = C2DUi::Create(set[TITLEUI_PRESSENTERNAME]);
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFlashing()->m_nTiming = 3;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->bLoop = true;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->nTiming = 30;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->fChangeValue = 1.0f / pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->nTiming;
	pC2dui[TITLEUI_PRESSENTERNAME]->GetFade()->nAddSign = 1;

	// プレスエンターアイコンの設定
	set[TITLEUI_PRESSENTERICON].mask.unMask = C2DUi::MASK_FADE | C2DUi::MASK_FLASHING;
	// プレスエンターのアイコン生成
	pC2dui[TITLEUI_PRESSENTERICON] = C2DUi::Create(set[TITLEUI_PRESSENTERICON]);
	pC2dui[TITLEUI_PRESSENTERICON]->GetFlashing()->m_nTiming = 3;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->bLoop = true;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->nTiming = 30;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->fChangeValue = 1.0f / pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->nTiming;
	pC2dui[TITLEUI_PRESSENTERICON]->GetFade()->nAddSign = 1;

	// プレイヤー1のアイコン生成
	// *GETSIZEデモって来る
	// 値を変える
	// 更新バーテクスで変える
	pC2dui[TITLEUI_PLAYER1ICON] = C2DUi::Create(set[TITLEUI_PLAYER1ICON]);

	// プレイヤー2のアイコン生成
	pC2dui[TITLEUI_PLAYER2ICON] = C2DUi::Create(set[TITLEUI_PLAYER2ICON]);

	// ピエン1のアイコン生成
	pC2dui[TITLEUI_PIEN1] = C2DUi::Create(set[TITLEUI_PIEN1]);

	// ピエン2のアイコン生成
	pC2dui[TITLEUI_PIEN2] = C2DUi::Create(set[TITLEUI_PIEN2]);

	// 拡大率の設定
	m_Scal.fScalChange = 1.2f;
	// タイムチェンジ
	m_Scal.nTimeChange = 30;
	// 拡大率差分
	m_Scal.fScalDiff = (m_Scal.fScalChange - m_Scal.fScal) / m_Scal.nTimeChange;
	// 初期サイズ
	m_Scal.OriginSize = D3DXVECTOR2(SIZEORIGIN, SIZEORIGIN);

	// 角度の設定
	m_fRotChange[0] = ROTCHANGE;
	m_fRotChange[1] = -ROTCHANGE;

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
	// サイズ変更処理
	Update_SizeChange();
	// 角度変更処理
	Update_RotChange();
	if (this->m_State == STATE_NORMAL)
	{
		// フェード更新処理
		for (int nCntUi = 0; nCntUi < TITLEUI_MAX; nCntUi++)
		{
			if (pC2dui[nCntUi]->GetFade() == NULL) continue;
			pC2dui[nCntUi]->GetFade()->Update(pC2dui[nCntUi]->GetImage());
		}
	}
	else if (this->m_State == STATE_OUT)
	{
		// フラッシュ更新処理
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
			// SEを入れる
			CManager::GetSound().PlaySound(CSound::SOUND_LABEL_SE_ENTER);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CTitle::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// サイズ変更の更新処理
//-------------------------------------------------------------------------------------------------------------
void CTitle::Update_SizeChange(void)
{
	// 変数宣言
	// UIの取得
	std::vector<CUi::UI_LOAD> Ui_Load = CUi::GetUi(CUi::UITYPE_TITLEUI);
	D3DXVECTOR2 * pSize;	// サイズ情報
	// サイズ情報の取得
	pSize = pC2dui[TITLEUI_PLAYER1ICON]->GetImage()->GetSize();
	// 切り替えOFF|切り替わるON/OFF
	if (m_Scal.nCntTimeChange == m_Scal.nTimeChange)
	{
		// 切り替わるON/OFF
		m_Scal.bChange = !m_Scal.bChange;
		// 切り替わるOFF時
		if (!m_Scal.bChange)
		{
			// 拡大率の差分
			m_Scal.fScalDiff = (m_Scal.fScalChange - m_Scal.fScal) / m_Scal.nTimeChange;
			// 目標の拡大率に強制変化
			m_Scal.fScal = 1.0f;
		}
		// 切り替わるON時
		else
		{
			// 目標の拡大率に強制変化
			m_Scal.fScal = m_Scal.fScalChange;
			// 拡大率の差分
			m_Scal.fScalDiff = (1.0f - m_Scal.fScalChange) / m_Scal.nTimeChange;
		}
		// カウント初期化
		m_Scal.nCntTimeChange = 0;
		// 関数を抜ける
		return;
	}
	// 切り替わる時間加算
	m_Scal.nCntTimeChange++;
	// 拡大率の変化
	m_Scal.fScal += m_Scal.fScalDiff;
	// サイズの変化
	*pSize = m_Scal.OriginSize * m_Scal.fScal;
	// 頂点カラーの設定
	pC2dui[TITLEUI_PLAYER1ICON]->GetImage()->UpdateVatexPosition();
	pSize = NULL;
	// サイズ情報の取得
	pSize = pC2dui[TITLEUI_PLAYER2ICON]->GetImage()->GetSize();
	// サイズの変化
	*pSize = m_Scal.OriginSize * m_Scal.fScal;
	// 頂点カラーの設定
	pC2dui[TITLEUI_PLAYER2ICON]->GetImage()->UpdateVatexPosition();

}

//-------------------------------------------------------------------------------------------------------------
// 角度変更の更新処理
//-------------------------------------------------------------------------------------------------------------
void CTitle::Update_RotChange(void)
{
	// 変数宣言
	// UIの取得
	std::vector<CUi::UI_LOAD> Ui_Load = CUi::GetUi(CUi::UITYPE_TITLEUI);
	float fRot;	// サイズ情報
	// サイズ情報の取得
	fRot = pC2dui[TITLEUI_PIEN1]->GetImage()->GetRotation();
	// 角度の加算
	fRot += m_fRotChange[0];
	// 角度の限度設定
	if (fRot < -ROTLIMIT)
	{
		m_fRotChange[0] *= -1;
	}
	else if (fRot > ROTLIMIT)
	{
		m_fRotChange[0] *= -1;
	}
	// 角度の設定
	pC2dui[TITLEUI_PIEN1]->GetImage()->SetRotation(fRot);
	pC2dui[TITLEUI_PIEN1]->GetImage()->UpdateVatexPosition();
	// サイズ情報の取得
	fRot = pC2dui[TITLEUI_PIEN2]->GetImage()->GetRotation();
	// 角度の加算
	fRot += m_fRotChange[1];
	// 角度の限度設定
	if (fRot < -ROTLIMIT)
	{
		m_fRotChange[1] *= -1;
	}
	else if (fRot > ROTLIMIT)
	{
		m_fRotChange[1] *= -1;
	}
	// 角度の設定
	pC2dui[TITLEUI_PIEN2]->GetImage()->SetRotation(fRot);
	pC2dui[TITLEUI_PIEN2]->GetImage()->UpdateVatexPosition();
}
