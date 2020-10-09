// ===================================================================
//
// スコア処理 [ score.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"
#include "mode.h"

// ===================================================================
// マクロ定義
// ===================================================================
#define MAX_SCORE_DIGIT	(5)	// 最大桁数
#define MAX_NUM_SCORE	(6)	// スコアの最大数

// ===================================================================
// クラス定義
// ===================================================================
class CNumericString;
class C2DUi;

class CScore
{
public:
	CScore();
	~CScore();

	HRESULT Init(void);							// 初期化
	void Uninit(void);							// 終了
	void Update(void);							// 更新
	void Draw(void);							// 描画

	static CScore *Create(D3DXVECTOR3 & pos, D3DXVECTOR2 & size, float fValue);		// 生成

	static void SetPlayerScore(const int nPlayer, const float fScore) { m_fPlayerScore[nPlayer] = fScore; } // スコアの設定
	void SetScore(const float fScore);			// スコアの設定

	static float * GetPlayerScore(void)			{ return m_fPlayerScore; }			// プレイヤースコアの取得
	static float & GetPlayerScore(int nPlayer)	{ return m_fPlayerScore[nPlayer]; }	// プレイヤースコアの取得
	static float * GetDefaultScore(void)		{ return m_fDefaultScore; }			// デフォルトスコアの取得
	static float & GetDefaultScore(int nRank)	{ return m_fDefaultScore[nRank]; }	// デフォルトスコアの個別取得

	static HRESULT Load(void);					// スコアのロード
	static HRESULT SaveScore(void);					// スコアのセーブ

private:
	C2DUi *pC2dui;					// ２DUIポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture;
	static float m_fPlayerScore[MAX_PLAYER];		// プレイヤーのスコア
	static float m_fDefaultScore[MAX_NUM_SCORE];	// デフォルトのスコア
	CNumericString * m_pNumStrInt;					// 整数ナンバーのポインタ
	CNumericString * m_pNumStrFloat;				// 小数ナンバーのポインタ
};

#endif