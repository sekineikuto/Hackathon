//*************************************************************************************************************
//
// タイトル処理 [title.h]
// Author:KOKI NISHIYAMA
//
//*************************************************************************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CTitle : public CMode
{
public:
	/* 列挙型 */
	// タイトルUI
	typedef enum
	{
		TITLEUI_BG = 0,			// 背景
		TITLEUI_NAME,			// 名前
		TITLEUI_PRESSENTERNAME,	// プレスエンター名
		TITLEUI_PRESSENTERICON,	// プレスエンターアイコン
		TITLEUI_PLAYER1ICON,	// プレイヤー1アイコン
		TITLEUI_PLAYER2ICON,	// プレイヤー2アイコン
		TITLEUI_PIEN1,			// ピエン1
		TITLEUI_PIEN2,			// ピエン2
		TITLEUI_MAX
	} TITLEUI;

	// 拡大率情報
	typedef struct SCALING
	{
		SCALING()
		{
			OriginSize = D3DXVECTOR2(0.0f,0.0f);
			fScal = 1.0f;
			fScalChange = 0.0f;
			fScalDiff = 0.0f;
			nTimeChange = 0;
			nCntTimeChange = 0;
			bChange = false;
		}
		D3DXVECTOR2 OriginSize;			// 元のサイズ
		float fScal;					// 拡大率
		float fScalChange;				// 目標拡大率
		float fScalDiff;				// 拡大率上昇率
		int nTimeChange;				// 切り替わる時間
		int nCntTimeChange;				// 切り替わる時間カウント
		bool bChange;					// 切り替わり中
	} SCALING, *P_SCALING;

	/* 関数 */
	CTitle() {};					// コンストラクタ
	~CTitle() {};					// デストラクタ

	static CTitle *Create(void);	// 生成
	void Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画

private:
	/* 関数 */
	// サイズ変更の更新処理
	void Update_SizeChange(void);
	// 角度変更の更新処理
	void Update_RotChange(void);
	/* 変数 */
	C2DUi *pC2dui[TITLEUI_MAX];					// ２DUIポインタ
	SCALING m_Scal;								// 拡大率
	float m_fRotChange[2];						// 角度変更値
};

#endif