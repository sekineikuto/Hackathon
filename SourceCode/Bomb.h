//*************************************************************************************************************
//
// 爆弾処理 [bomb.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _BOMB_H_
#define _BOMB_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CScene2D;
class CBomb : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = -1,
		STATE_STANDBY,
		STATE_MOVEING,
		STATE_LANDING,
		STATE_MAX
	}STATE;


	CBomb() : CScene(PRIORITY_PLAYER) {};		// コンストラクタ
	~CBomb() {};								// デストラクタ

	static CBomb *Create(D3DXVECTOR3 &pos, D3DXVECTOR2 &size, int nPlayer_ID);				// 生成
	void Init(D3DXVECTOR3 &pos, D3DXVECTOR2 &size, int &nPlayer_ID);							// 初期化
	void Init(void) {};									// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	void UpdateStandby(void);							// 更新
	void UpdateMoveing(void);							// 更新
	void UpdateLanding(void);							// 更新

	void Fire(float& fForce,float& fForceY);

	void SetState(STATE state) { m_State = state; }
	STATE & GetState(void) { return m_State; }
private:
	CScene2D *m_pScene2D;					// シーン２Dポインタ
	STATE     m_State;
	D3DXVECTOR3 m_move;
	int       m_nPlayer_ID;
};

#endif