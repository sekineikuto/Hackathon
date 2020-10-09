//*************************************************************************************************************
//
// ゲーム処理 [game.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _GAME_H_
#define _GAME_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CPlayer;
class CBomb;
class CGame : public CMode
{
public:
	enum
	{
		UI_BG = 0,
		UI_TIMER,

		UI_P1_GAGE_X,
		UI_P1_GAGE_Y,
		UI_P2_GAGE_X,
		UI_P2_GAGE_Y,

		UI_P1_FRAME_X,
		UI_P1_FRAME_Y,
		UI_P2_FRAME_X,
		UI_P2_FRAME_Y,

		UI_MAX,
	};

	enum
	{
		SCAL_P1_GAGE_X,
		SCAL_P1_GAGE_Y,
		SCAL_P2_GAGE_X,
		SCAL_P2_GAGE_Y,
		SCAL_P2_GAGE_MAX,
	};

	enum
	{
		PLAYER_NONE = -1,
		PLAYER_1,
		PLAYER_2,
		PLAYER_MAX,
	};


	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 size;
	} PLAYEROFFSET;

	CGame() {};					// コンストラクタ
	~CGame() {};					// デストラクタ

	static CGame *Create(void);	// 生成
	void Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画

private:
	void UpdateNormal(void);		// 通常更新
	void UpdateOut(void);			// アウト更新

	void GageUpdate(void);
	void GageScaForceShuffle(float *pGageScalForce);
	bool GageScalClamp(float *pGageScal,int *pSign);

	C2DUi *pC2dui[UI_MAX];					// ２DUIポインタ
	CPlayer *m_pPlayer[PLAYER_MAX];				// プレイヤポインタ
	CBomb *m_pBomb[PLAYER_MAX];				// 爆弾ポインタ
	static PLAYEROFFSET m_offset[PLAYER_MAX];

	float m_fGageScal[SCAL_P2_GAGE_MAX];
	float m_fGageScaForce[SCAL_P2_GAGE_MAX];
	int m_fGageScaSign[SCAL_P2_GAGE_MAX];
	bool m_bMoveGage[SCAL_P2_GAGE_MAX];
};

#endif