//*************************************************************************************************************
//
// 数字文字列処理[NumericString.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "NumericString.h"
#include "number.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CNumericString::CNumericString()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CNumericString::~CNumericString()
{
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CNumericString * CNumericString::Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size, float fRotation, int nValue)
{
	CNumericString *pNumericString = new CNumericString;
	pNumericString->Init(pos, col, size, fRotation, nValue);
	return pNumericString;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Init(D3DXVECTOR3 & pos, D3DXCOLOR & col, D3DXVECTOR2 & size, float & fRotation, int & nValue)
{
	m_col = col;			// 色
	m_pos = pos;			// 位置
	m_size = size;			// 大きさ
	m_nValue = nValue;		// 時間の値
	m_nAddSign = -1;		// 加算する符号
	m_fRotation = fRotation;

	// 桁数から使う個数を算出
	m_nDigits = 0;
	CMylibrary::GetDigits(m_nDigits, m_nValue);
	m_nNumNumber = m_nDigits;

	// メモリを確保
	m_pNumber = new CNumber[m_nNumNumber];

	// 使う数字分ループ
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; nCntNumber++)
	{// 桁の数字を算出
		int nDigits =
			m_nValue %
			(int)powf(10.0f, (float)m_nNumNumber - nCntNumber) /
			(int)powf(10.0f, (float)m_nNumNumber - 1.0f - nCntNumber);
		// 位置の設定
		D3DXVECTOR3 pos;
		pos.x = m_pos.x + (sinf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x * 2.0f));
		pos.y = m_pos.y + (cosf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x * 2.0f));
		pos.z = 0.0f;
		// 初期化
		m_pNumber[nCntNumber].Init(m_size.x, m_size.y, nDigits, m_fRotation, pos, m_col);
	}
	// 差分を出す
	m_nNumDiff = m_nNumNumber - m_nDigits;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Uninit(void)
{
	if (m_pNumber != nullptr)
	{
		m_pNumber->Unint();
		delete[]m_pNumber;
		m_pNumber = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Update(void)
{
	// 桁数を初期化値にする
	m_nDigits = 0;
	// 桁数を求める
	CMylibrary::GetDigits(m_nDigits, m_nValue);
	// 桁数分ループ
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; nCntNumber++)
	{// 桁の数字を算出
		int nDigits =
			m_nValue %
			(int)powf(10.0f, (float)m_nNumNumber - nCntNumber) /
			(int)powf(10.0f, (float)m_nNumNumber - 1.0f - nCntNumber);
		// 頂点情報の設定
		m_pNumber[nCntNumber].SetVaetex(&nDigits);
	}

	// 差分を出す
	m_nNumDiff = m_nNumNumber - m_nDigits;
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Draw(void)
{
	// 使う個数分ループ
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; nCntNumber++)
	{//差分がカウント以下だった時
		//if (m_nNumDiff <= nCntNumber)
		{// 描画
			m_pNumber[nCntNumber].Draw();
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 値の加算
//-------------------------------------------------------------------------------------------------------------
void CNumericString::AddValue(void)
{
	// 開始値に符号を加算
	m_nValue += m_nAddSign;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャの連結
//-------------------------------------------------------------------------------------------------------------
void CNumericString::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; nCntNumber++)
	{
		m_pNumber[nCntNumber].BindTexture(pTexture);
	}
}
