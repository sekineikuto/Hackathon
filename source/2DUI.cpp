//*************************************************************************************************************
//
// 2DUI処理[2DUI.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "2DUI.h"
#include "Scene2D.h"
#include "number.h"
#include "NumericString.h"
#include "texture.h"

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
C2DUi * C2DUi::Create(SETING2DUI &seting)
{
	C2DUi * pC2dui = new C2DUi;
	pC2dui->Init(seting);
	return pC2dui;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Init(SETING2DUI & seting)
{
	m_Mask = seting.mask;

	if (Mybfunc_bit_comp(m_Mask.unMask, FLAG_NUMBER))
	{
		m_pNumber = CNumericString::Create(seting.pos, seting.col, seting.size, seting.fRotation, seting.nValue);
		m_pNumber->BindTexture(CTexture::GetTextureInfo(seting.nTextureID));
	}
	else
	{
		m_pImage = CScene2D::Create(PRIORITY::PRIORITY_2DUI, seting.pos, seting.col, ORIGINVERTEXTYPE_ROTCENTER, seting.size, 0.0f, seting.fRotation);
		m_pImage->BindTexture(CTexture::GetTextureInfo(seting.nTextureID));
	}

	if (Mybfunc_bit_comp(m_Mask.unMask, FLAG_FLASHING))
	{
		m_pFlashing = new C2DUi::FLASHING;
	}
	if (Mybfunc_bit_comp(m_Mask.unMask, FLAG_FADE))
	{
		m_pFade = new C2DUi::FADE;
	}
	m_nTextureID = 0;
	m_bDisp = true;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Init(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Uninit(void)
{
	if (m_pFlashing != nullptr)
	{
		delete m_pFlashing;
		m_pFlashing = nullptr;
	}
	if (m_pFade != nullptr)
	{
		delete m_pFade;
		m_pFade = nullptr;
	}
	if (m_pNumber != nullptr)
	{
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = nullptr;
	}
	if (m_pImage != nullptr)
	{
		m_pImage->Release();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Update(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Draw(void)
{
	if (m_bDisp == true)
	{
		if (m_pNumber != nullptr)
		{
			m_pNumber->Draw();
		}
		if (m_pImage != nullptr)
		{
			m_pImage->Draw();
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// アルファ値を範囲内に制限する
//-------------------------------------------------------------------------------------------------------------
bool C2DUi::AlphaLimitRange(D3DXCOLOR &col)
{
	if (col.a <= 0.0f)
	{
		col.a = 0.0f;
		return true;
	}
	else if (col.a >= 1.0f)
	{
		col.a = 1.0f;
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// フェードの更新
//-------------------------------------------------------------------------------------------------------------
void C2DUi::FADE::Update(CScene2D *pImage)
{
	// 色の取得
	D3DXCOLOR *pCol = pImage->GetColor();

	// 色の変化
	pCol->a += fChangeValue * nAddSign;

	// アルファ値を範囲内に制限する
	if (C2DUi::AlphaLimitRange(*pCol) == true)
	{// 直されたとき
		nAddSign *= -1;
	}

	// 頂点カラーを更新する
	pImage->UpdateVatexColor();
}

//-------------------------------------------------------------------------------------------------------------
// フェードの更新
//-------------------------------------------------------------------------------------------------------------
void C2DUi::FADE::Update(CNumericString * pNumber)
{
	if (pNumber == nullptr)
	{
		return;
	}

	for (int nCntNumber = 0; nCntNumber < pNumber->m_nNumNumber; nCntNumber++)
	{
		// 数字クラスの取得
		CNumber *pNumClass = &pNumber->m_pNumber[nCntNumber];
		if (pNumClass == nullptr)
		{
			continue;
		}
		// 色の取得
		D3DXCOLOR *pCol = pNumClass->GetColor();

		// 色の変化
		pCol->a += fChangeValue * nAddSign;

		// アルファ値を範囲内に制限する
		if (C2DUi::AlphaLimitRange(*pCol) == true)
		{// 直されたとき
			nAddSign *= -1;
		}

		// 頂点カラーを更新する
		pNumClass->SetVeatexColor();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 点滅の更新
//-------------------------------------------------------------------------------------------------------------
void C2DUi::FLASHING::Update(C2DUi * pThis)
{
	UINT *pTime = &pThis->GetTime();

	*pTime += 1;

	if (pThis->m_pFlashing->IsEqual(*pTime))
	{
		pThis->ChangeDispFlag();
		pTime = 0;
	}
}
