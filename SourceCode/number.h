//*************************************************************************************************************
//
// 数字の処理[number.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "main.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define SCORE_NUMBER_SIZE_X (35.0f)
#define SCORE_NUMBER_SIZE_Y (55.0f)

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CNumber
{
public:
	CNumber(int nNumOrder);																							// コンストラクタ
	CNumber();																										// コンストラクタ
	~CNumber();																										// デストラクタ
	static HRESULT Load(void);																						// 読み込み
	static void Unload(void);																						// 開放
	static CNumber *Create(float fSizeX, float fSizeY, D3DXVECTOR3 pos, float fRot, int nNumber, D3DXCOLOR col);	// 生成
	HRESULT Init(float fSizeX, float fSizeY, int nNumber, float fRot, D3DXVECTOR3 pos, D3DXCOLOR col);				// 初期化
	void Unint(void);																								// 終了
	void Update(void);																								// 更新
	void Draw(void);																								// 描画
	void SetNumber(int nNumber);																					// 数字の設定
	void SetVaetex(int *nNumber = NULL, D3DXVECTOR3 *pos = NULL, D3DXCOLOR *col = NULL);							// 頂点の更新
	void SetVeatexColor(void);																						// 頂点カラー設定
	void SetSize(D3DXVECTOR2 *size);																				// 大きさの設定
	void SetRotation(float fRot);																					// 回転量の設定
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);																	// テクスチャ情報の設定

	inline D3DXCOLOR *GetColor(void) { return &m_col; }
	inline D3DXVECTOR2 *GetSize(void) { return &m_size; }
	inline D3DXVECTOR3 *GetPos(void) { return &m_pos; }
	inline float *GetRot(void) { return &m_fRot; }
	inline int *GetNumber(void) { return&m_nNumber; }

private:
	LPDIRECT3DTEXTURE9			m_pTexture;		// テクスチャ情報のポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// バッファ情報のポインタ
	int							m_nNumber;		// 数字
	float						m_fRot;			// 回転量
	D3DXVECTOR2					m_size;			// 大きさ
	D3DXVECTOR3					m_pos;			// 位置
	D3DXCOLOR					m_col;			// 色
};

#endif
