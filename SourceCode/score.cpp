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

//=============================================================================
// マクロ定義
//=============================================================================
#define FILENAME_RANKING	("data/TEXT/Ranking.txt")	// ランキングのテキストファイル

//=============================================================================
// 静的メンバ変数
//=============================================================================
float CScore::m_fPlayerScore					= 0;
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
	if (m_pNumStrInt)
		m_pNumStrInt->Draw();
	if (m_pNumStrFloat)
		m_pNumStrFloat->Draw();
}

//=============================================================================
// 生成
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 & pos, D3DXVECTOR2 & size, float fValue)
{
	// 整数の値を取得
	int IntValue = (int)fValue;
	// 小数の値を取得
	int FloatValue = (int)(fValue - (float)IntValue) * 100;

	// メモリ確保
	CScore *pScore = new CScore;
	// 初期化
	pScore->Init();
	// ナンバーの生成
	pScore->m_pNumStrInt = CNumericString::Create(pos, MYLIB_D3DXCOR_SET, size, 0.0f, IntValue);
	pScore->m_pNumStrInt->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_NUMBER));
	// ナンバーの生成
	pScore->m_pNumStrFloat = CNumericString::Create(pos, MYLIB_D3DXCOR_SET, size, 0.0f, FloatValue);
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
				else if (strcmp(cHeadText, "PLAYERSCORE") == 0)
				{
					// モデルタイプ数の取得
					sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_fPlayerScore);
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
	// 最低スコアをプレイヤーのものに書き換え
	m_fDefaultScore[5] = m_fPlayerScore;

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
		sprintf(cWriteText, "PLAYERSCORE : %.3f\n",
			m_fPlayerScore);
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
