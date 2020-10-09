//*************************************************************************************************************
//
// 数字文字列処理[NumericString.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _NUMERICSTRING_H_
#define _NUMERICSTRING_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CNumber;
class CNumericString
{
public:
	CNumericString();				// コンストラクタ
	~CNumericString();				// デストラクタ

	static CNumericString *Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size, float fRotation ,int nValue);

	void Init(D3DXVECTOR3 &pos, D3DXCOLOR &col, D3DXVECTOR2 &size, float &fRotation, int &nValue);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddValue(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);				// テクスチャを関連付ける

	int GetDigit(void) { return m_nDigits; }		// 桁数の取得

	int							m_nValue;			// 値
	int							m_nDigits;			// 桁数(描画数)
	int							m_nNumNumber;		// 使う個数
	int							m_nAddSign;			// 加算する符号
	CNumber						*m_pNumber;			// 数字情報
	D3DXCOLOR					m_col;				// 色
	D3DXVECTOR3					m_pos;				// 位置
	D3DXVECTOR2					m_size;				// 大きさ
	float						m_fRotation;		// 回転量
	int							m_nNumDiff;			// 差分
};

#endif