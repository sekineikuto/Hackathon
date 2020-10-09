//=============================================================================
//
// スコア処理 [ score.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "score.h"
#include "NumericString.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "2DUI.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FILENAME_RANKING	("data/TEXT/Ranking.txt")	// ランキングのテキストファイル
#define HEIGHT_DOT			(25.0f)
#define TIME_FLASH			(10)

//=============================================================================
// 静的メンバ変数
//=============================================================================
float CScore::m_fPlayerScore[MAX_PLAYER]		= {};
float CScore::m_fDefaultScore[MAX_NUM_SCORE]	= {};
LPDIRECT3DTEXTURE9 CScore::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CScore::Init(void)
{
	m_bFlash = false;
	m_nCntFlash = 0;
	// 成功
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CScore::Uninit(void)
{
	if (m_pNumStrInt)
	{
		m_pNumStrInt->Uninit();
		delete m_pNumStrInt;
		m_pNumStrInt = nullptr;
	}

	if (m_pNumStrFloat)
	{
		m_pNumStrFloat->Uninit();
		delete m_pNumStrFloat;
		m_pNumStrFloat = nullptr;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CScore::Update(void)
{
	if (m_bFlash)
	{
		m_nCntFlash++;
		if (m_nCntFlash >= TIME_FLASH)
		{
			m_nCntFlash = 0;
		}
		pC2dui->GetFlashing()->Update(pC2dui);
	}

	if (m_pNumStrInt)
		m_pNumStrInt->Update();
	if (m_pNumStrFloat)
		m_pNumStrFloat->Update();
}

//=============================================================================
// 描画
//=============================================================================
void CScore::Draw(void)
{
	if (m_bFlash && m_nCntFlash % 5 == 0)
		return;

	if (m_pNumStrInt)
		m_pNumStrInt->Draw();
	if (m_pNumStrFloat)
		m_pNumStrFloat->Draw();
}

//=============================================================================
// 生成
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 & pos, D3DXVECTOR2 & size, float fValue, bool bFlash)
{
	// 整数の値を取得
	int IntValue = (int)fValue;
	// 小数の値を取得
	float FloatValue = (fValue - (float)IntValue) * 1000.0f;

	// メモリ確保
	CScore *pScore = new CScore;
	// 初期化
	pScore->Init();
	// 整数の生成
	pScore->m_pNumStrInt = CNumericString::Create(pos, MYLIB_D3DXCOR_SET, size, 0.0f, IntValue);
	pScore->m_pNumStrInt->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_NUMBER));
	pScore->m_bFlash = bFlash;

	// 小数の座標を計算
	D3DXVECTOR3 posDot = D3DXVECTOR3(pos.x + size.x * (pScore->m_pNumStrInt->GetDigit() * 2), pos.y + HEIGHT_DOT, pos.z);

	C2DUi::SETING2DUI set;
	set.bDisp = true;
	set.col = MYLIB_D3DXCOR_SET;
	set.fRotation = 0.0f;
	set.nTextureID = CTexture::NAME_DOT;
	set.nValue = 123456789;
	set.pos = posDot;
	set.size = D3DXVECTOR2(60.0f, 120.0f);
	if (bFlash)
		set.mask.unMask = C2DUi::MASK_FLASHING;
	pScore->pC2dui = C2DUi::Create(set);
	if (bFlash)
		pScore->pC2dui->GetFlashing()->m_nTiming = TIME_FLASH / 2;

	// 小数の座標を計算
	D3DXVECTOR3 posFloat = D3DXVECTOR3(posDot.x + size.x, pos.y, pos.z);

	// 小数の生成
	pScore->m_pNumStrFloat = CNumericString::Create(posFloat, MYLIB_D3DXCOR_SET, size, 0.0f, (int)FloatValue);
	pScore->m_pNumStrFloat->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_NUMBER));
	// 値を返す
	return pScore;
}

//=============================================================================
// スコアの設定
//=============================================================================
void CScore::SetScore(const float fScore)
{
	// 整数の値を取得
	int IntValue = (int)fScore;
	// 小数の値を取得
	int FloatValue = (int)(fScore - (float)IntValue) * 100;
	// 数字を変更
	m_pNumStrInt->m_nValue = IntValue;
	m_pNumStrFloat->m_nValue = FloatValue;
}

//=============================================================================
// スコアのロード
//=============================================================================
HRESULT CScore::Load(void)
{
	// 変数宣言
	FILE *pFile = nullptr;

	char cHeadText[64];
	char cReadText[64];
	char cDieText[64];
	int nCntScore = 0;

	// ファイルを開く
	pFile = fopen(FILENAME_RANKING, "r");

	// nullcheck
	if (pFile)
	{
		printf("==================================================\n");
		printf("ランキングファイルの読み込み開始\n");

		// スクリプトがくるまで繰り返す
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}
		// スクリプトが来たら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまで繰り返す
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);
				// 改行
				if (strcmp(cHeadText, "\n") == 0)
				{
				}
				// スコアが来たら
				else if (strcmp(cHeadText, "PLAYERSCORE_1") == 0)
				{
					// モデルタイプ数の取得
					sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_fPlayerScore[0]);
				}
				// スコアが来たら
				else if (strcmp(cHeadText, "PLAYERSCORE_2") == 0)
				{
					// モデルタイプ数の取得
					sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_fPlayerScore[1]);
				}
				// スコアが来たら
				else if (strcmp(cHeadText, "SCORE") == 0)
				{
					// モデルタイプ数の取得
					sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_fDefaultScore[nCntScore]);
					// スコア番号の加算
					nCntScore++;
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}

		printf("ランキングファイルの読み込み終了\n");
		printf("==================================================\n");
	}
	// ファイルを開けなかったとき
	else
	{
		printf("ランキングファイルを開けませんでした。\n");
		printf("==================================================\n");
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// スコアのセーブ
//=============================================================================
HRESULT CScore::SaveScore(void)
{
	float fWinScore = m_fPlayerScore[0];
	if (fWinScore < m_fPlayerScore[1])
		fWinScore = m_fPlayerScore[1];

	// 最低スコアをプレイヤーのものに書き換え
	m_fDefaultScore[5] = fWinScore;

	// ランキングの入れ替え
	for (int nCount = 0; nCount < MAX_NUM_SCORE - 1; nCount++)
	{
		for (int nCount2 = nCount + 1; nCount2 < MAX_NUM_SCORE; nCount2++)
		{
			// スコアが上回っている
			if (m_fDefaultScore[nCount] < m_fDefaultScore[nCount2])
			{
				// 順位の入れ替え
				float fData = m_fDefaultScore[nCount2];
				m_fDefaultScore[nCount2] = m_fDefaultScore[nCount];
				m_fDefaultScore[nCount] = fData;
			}
		}
	}

	// 変数宣言
	FILE *pFile;

	// テキストファイルを見やすくするコメント
	char cComment0[8] = "\n";

	// 書き込み用の変数
	char cWriteText[64];

	// 位置・回転の取得用
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	// ファイルを開く
	pFile = fopen(FILENAME_RANKING, "w");

	// nullcheck
	if (pFile)
	{
		int nCntScore = 0;

		// コメント設定
		strcpy(cWriteText, "SCRIPT\n");
		fputs(cWriteText, pFile);															// SCRIPT
		fputs(cComment0, pFile);															// \n

		// コメント設定
		sprintf(cWriteText, "PLAYERSCORE_1 : %.3f\n",
			m_fPlayerScore[0]);
		fputs(cWriteText, pFile);														// PLAYERSCORE = m_fPlayerScore

		// コメント設定
		sprintf(cWriteText, "PLAYERSCORE_2 : %.3f\n",
			m_fPlayerScore[1]);
		fputs(cWriteText, pFile);														// PLAYERSCORE = m_fPlayerScore

		for (int nCntScore = 0; nCntScore < MAX_NUM_SCORE; nCntScore++)
		{
			// コメント設定
			sprintf(cWriteText, "SCORE : %.3f\n",
				m_fDefaultScore[nCntScore]);
			fputs(cWriteText, pFile);														// SCORE = m_fDefaultScore
		}

		fputs(cComment0, pFile);															// \n

		// コメント設定
		strcpy(cWriteText, "END_SCRIPT\n");
		fputs(cWriteText, pFile);															// END_SCRIPT

		// ファイルを閉じる
		fclose(pFile);
	}
	// ファイルを開けなかった時
	else
	{
		// コンソールに表示
		printf("ランキングファイルを開けませんでした。\n");
		// 失敗
		return E_FAIL;
	}

	// 成功
	return S_OK;
}
