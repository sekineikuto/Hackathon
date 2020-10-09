//*************************************************************************************************************
//
// リザルト処理 [result.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CScore;

class CResult : public CMode
{
public:
	CResult() {};					// コンストラクタ
	~CResult() {};					// デストラクタ

	static void SetWinPlayer(const int nPlayer) { m_nWinPlayer = nPlayer; }
	static int GetWinPlayer(void)				{ return m_nWinPlayer; }

	static CResult *Create(void);	// 生成
	void Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画

private:
	C2DUi *pPienDistance[MAX_PLAYER];					// ２DUIポインタ
	C2DUi *pPienIcon[MAX_PLAYER];						// ぴえんのアイコン
	C2DUi *pJudgeText[MAX_PLAYER];						// 勝敗の文章
	CScore *m_pScore[MAX_PLAYER];						// プレイヤーのスコア
	static int m_nWinPlayer;							// 勝ったプレイヤーのインデックス
	static D3DXVECTOR3 m_posScore[MAX_PLAYER];			// スコアの座標
	static D3DXVECTOR2 m_sizeScore[MAX_PLAYER];			// スコアのサイズ
	static D3DXVECTOR3 m_posPien[MAX_PLAYER];			// ぴえんの座標
	static D3DXVECTOR3 m_posText[MAX_PLAYER];			// 文章の座標
};

#endif