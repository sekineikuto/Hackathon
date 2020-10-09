//*************************************************************************************************************
//
// プレイヤー処理 [player.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CScene2D;
class CPlayer : public CScene
{
public:

	typedef enum
	{
		STATE_NOEN = -1,
		STATE_STANDBY,
		STATE_THROW,
		STATE_EXPOSURE,
		STATE_NOTEXPOSURE,
		STATE_MAX,
	}STATE;


	CPlayer():CScene(PRIORITY_PLAYER){};		// コンストラクタ
	~CPlayer() {};								// デストラクタ

	static CPlayer *Create(D3DXVECTOR3 &pos, D3DXVECTOR2 &size);				// 生成
	void Init(D3DXVECTOR3 &pos, D3DXVECTOR2 &size);							// 初期化
	void Init(void) {};							// 初期化
	void Uninit(void);							// 終了
	void Update(void);							// 更新
	void Draw(void);							// 描画

	void UpdateStandby(void);
	void UpdateThrow(void);

	void SetState(STATE state) { m_State = state; }
	STATE & GetState(void) { return m_State; }

	CScene2D *GetScene2D(void) { return m_pScene2D; }

	D3DXVECTOR3 &GetPos(void);
private:
	CScene2D *m_pScene2D;					// シーン２Dポインタ
	STATE     m_State;						// 状態
};

#endif