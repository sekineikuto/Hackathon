//*************************************************************************************************************
//
// 2DUI処理[2DUI.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _2DUI_H_
#define _2DUI_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CNumericString;
class CScene2D;
class C2DUi : public CScene
{
public:
	enum
	{
		MASK_NONE = 0b00000000,
		MASK_NUMBER = 0b00000001,
		MASK_FLASHING = 0b00000010,
		MASK_FADE = 0b00000100,
		MASK_MAX = 0b00000111,
	};
	enum
	{
		FLAG_NONE = -1,
		FLAG_NUMBER,
		FLAG_FLASHING,
		FLAG_FADE,
		FLAG_MAX
	};
	typedef struct
	{
		unsigned int bNumber : 1;
		unsigned int bFlashing : 1;
		unsigned int bFade : 1;
	} _MASK;

	typedef union MASK
	{
		MASK(int n = 0) : unMask(n) {}
		unsigned int unMask;
		_MASK field;
	} MASK;
	typedef struct FLASHING
	{
		UINT m_nTiming;															// 点滅
		bool IsEqual(UINT &nTiming) { return (nTiming >= m_nTiming); }
		void Update(C2DUi *pThis);

	} FLASHING;

	typedef struct FADE
	{
		bool  bLoop;															// ループ
		float fChangeValue;														// アルファ値の1フレの変更値
		int   nTiming;															// タイミング
		int   nAddSign;															// 加算する符号

		inline void   SetChangeValue(float fChangeValue) { fChangeValue = fChangeValue; }											// 1フレの変更値の設定
		inline float& GetChangeValue(void) { return fChangeValue; }																	// 1フレの変更値の取得
		inline void   SetAddSign(int nAddSign) { if (nAddSign != 0)nAddSign = (nAddSign / nAddSign) * (nAddSign > 0) ? 1 : -1; }	// 加算する符号の設定
		inline int&   GetAddSign(void) { return nAddSign; }																			// 加算する符号の取得

		void Update(CScene2D *pImage);
		void Update(CNumericString *m_pNumber);
	} FADE;

	typedef struct SETING2DUI
	{
		SETING2DUI() {}
		SETING2DUI(MASK mask, int  nTextureID, bool bDisp, D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size, float fRotation, int nValue)
		{
			this->mask = mask;
			this->nTextureID = nTextureID;
			this->bDisp = bDisp;
			this->pos = pos;
			this->col = col;
			this->size = size;
			this->fRotation = fRotation;
			this->nValue =nValue;
		}
		MASK        mask;			// マスク
		int         nTextureID;		// テクスチャID
		bool        bDisp;			// 描画フラグ
		D3DXVECTOR3 pos;			// 位置
		D3DXCOLOR   col;			// 色
		D3DXVECTOR2 size;			// サイズ
		float       fRotation;		// 回転量
		int         nValue;			// 値
	} SETING2DUI;

	C2DUi(PRIORITY pri) : CScene(pri) {}																							// コンストラクタ
	~C2DUi() {}																													// デストラクタ

	static C2DUi * Create(SETING2DUI &seting, PRIORITY pri = PRIORITY_2DUI);																					// 生成
	void Init(SETING2DUI &seting, PRIORITY pri);																								// 初期化
	void Init(void);																											// 初期化
	void Uninit(void);																											// 終了
	void Update(void);																											// 更新
	void Draw(void);																											// 描画

	static bool   AlphaLimitRange(D3DXCOLOR &col);																						// アルファ値を範囲内に制限する
	inline void   ChangeDispFlag(void) { this->m_bDisp ^= 1; }																	// 描画フラグの切り替え

	inline void   SetTime(UINT nChangeTime) { m_nTime = nChangeTime; }													// 時間の設定
	inline UINT&  GetTime(void) { return m_nTime; }																		// 時間の取得
	inline void   SetTextureID(int nTextureID) { m_nTextureID = nTextureID; }													// テクスチャIDの設定
	inline int&   GetTextureID(void) { return m_nTextureID; }																	// テクスチャIDの取得
	inline void   SetDisp(bool bDisp) { m_bDisp = bDisp; }																		// 描画フラグの設定
	inline bool&  GetDisp(void) { return m_bDisp; }																				// 描画フラグの取得

	inline FLASHING *GetFlashing(void) { return m_pFlashing; }
	inline FADE *GetFade(void) { return m_pFade; }
	inline CNumericString *GetNumericString(void) { return m_pNumber; }
	inline CScene2D *GetImage(void) { return m_pImage; }


protected:
	// 共通
	int   m_nTextureID;															// テクスチャID
	UINT  m_nTime;																// 時間
	bool  m_bDisp;																// 描画フラグ

	MASK m_Mask;																// マスク

	FLASHING *m_pFlashing;														// 点滅
	FADE* m_pFade;																// フェード
	CNumericString *m_pNumber;													// 数字
	CScene2D *m_pImage;															// 画像
};


#endif