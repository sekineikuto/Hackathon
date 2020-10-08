//*************************************************************************************************************
//
// シーン2D処理[scene2D.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CScene2D :public CScene
{
public:
	CScene2D(CScene::PRIORITY priority);											// コンストラクタ
	~CScene2D();																	// デストラクタ

	static CScene2D *Create(														// 生成
		CScene::PRIORITY priority,													// 描画優先順位
		D3DXVECTOR3      pos,														// 位置
		D3DXCOLOR        col,														// 色
		ORIGINVERTEXTYPE OriginType,												// 原点タイプ
		D3DXVECTOR2      size,														// 大きさ
		float            fRadius = 0.0f,											// 半径
		float            fRotation = 0.0f,											// 回転量
		SETING_UV        TexUVInfo = SETING_UV(0, 1, 0, 1));						// テクスチャUV設定

	static CScene2D *Create(														// 生成
		CScene::PRIORITY priority,													// 描画優先順位
		D3DXVECTOR3      pos,														// 位置
		ORIGINVERTEXTYPE OriginType,												// 色
		D3DXVECTOR2      size,														// サイズ
		float            fRadius = 0.0f);											// 半径
	
	void Init(void);																// 初期化
	void Uninit(void);																// 終了
	void Update(void);																// 更新
	void Draw(void);																// 描画

	void UpdateVertex(																// 頂点の更新
		D3DXVECTOR3 *pPos = NULL,													// 位置のポインタ
		D3DXVECTOR2 *pSize = NULL,													// 大きさのポインタ
		float       *pRadius = NULL,												// 半径のポインタ
		D3DXCOLOR   *pCol = NULL,													// 色のポインタ
		SETING_UV   *pTexVari = NULL);												// UV座標設定のポインタ

	void UpdateVertex(																// 頂点の更新
		bool bUpdatePos = false,													// 位置更新フラグ
		bool bUpdateCol = false,													// 色更新フラグ
		bool bUpdateUVTex = false);													// UV座標更新フラグ

	void                    UpdateVatexPosition(void);								// 頂点位置の更新
	void                    UpdateVatexColor(void);									// 頂点カラーの更新
	void                    UpdateVatexTexture(void);								// 頂点テクスチャの更新

	void                    ChangeVertexOrigin(ORIGINVERTEXTYPE OriginType);		// 原点の変更
	void                    BindTexture(LPDIRECT3DTEXTURE9 pTexture);				// テクスチャを関連付ける
	LPDIRECT3DVERTEXBUFFER9 GetpVtxBuff(void);										// 頂点バッファの取得
	void                    SetOriginType(ORIGINVERTEXTYPE OriginType);				// 原点タイプの設定
	ORIGINVERTEXTYPE*       GetOriginType(void);									// 原点タイプの取得
	void                    SetPosition(D3DXVECTOR3& pos);							// 位置の設定
	D3DXVECTOR3*            GetPosition(void);										// 位置の取得
	void                    SetColor(D3DXCOLOR& col);								// 色の設定
	D3DXCOLOR*              GetColor(void);											// 色の取得
	void                    SetSize(D3DXVECTOR2& size);								// 大きさの設定
	void                    SetSizeX(float fSize);									// 大きさXの設定
	void                    SetSizeY(float fSize);									// 大きさYの設定
	D3DXVECTOR2*            GetSize(void);											// 大きさの取得
	void                    SetSetingUV(SETING_UV& SetingUV);						// テクスチャのUV座標の設定用の情報の設定
	void                    SetSetingUVCountU(int nCntU);							// テクスチャのUV座標の設定用の情報横のカウントの設定
	void                    SetSetingUVCountV(int nCntV);							// テクスチャのUV座標の設定用の情報縦のカウントの設定
	SETING_UV*              GetSetingUV(void);										// テクスチャのUV座標の設定用の情報の取得
	void                    SetAnimTexVariables(TEXTUREANIMEINFO& TextureAnimeInfo);	// テクスチャアニメーション情報の設定
	void                    SetAnimTexVariFrame(int nFrame);						// テクスチャアニメーションフレーム情報の設定
	void                    SetAnimTexVariFrameMax(int nFrameMax);					// テクスチャアニメーションの最大フレーム情報の設定
	TEXTUREANIMEINFO*       GetAnimTexVariables(void);								// テクスチャアニメーション情報の取得
	void                    SetRadius(float fRadius);								// 半径の設定
	float                   GetRadius(void);										// 半径の取得
	void                    SetRotation(float fRotation);							// 回転量の設定
	float                   GetRotation(void);										// 回転量の取得
	int                     CalculatVatexUVMap(bool bLoop, int nFrameMax, int &nFrame, int nNumWidht, int nNumHight, int &nCountWidht, int &nCountHight);
	void                    CalSelectionUVMap(bool bIteration, int nFrameMax, int &nFrame, int nNumWidht, int &nCountWidht, int nCountHight, int &m_nIteration);
	void                    CalTurnBackUVMap(int nFrameMax, int &nFrame, int nNumWidht, int &nCountWidht, int nCountHight, int &m_nIteration);
	inline void             SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	inline bool &           GetDisp(void) { return m_bDisp; }
protected:
	/* -- メンバ関数 -- */
	HRESULT                 MakeVatex(LPDIRECT3DDEVICE9 pDevice);					// 頂点の作成
	void                    SetVatexPosition(CRenderer::VERTEX_2D *pVtx);			// 頂点位置の設定
	void                    SetVatexColor(CRenderer::VERTEX_2D *pVtx);				// 頂点カラーの設定
	void                    SetVatexTexture(CRenderer::VERTEX_2D *pVtx);			// 頂点テクスチャの設定
	/* -- メンバ変数 -- */
	LPDIRECT3DTEXTURE9      m_pTexture;												// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;												// 頂点バッファへのポインタ
	ORIGINVERTEXTYPE        m_OriginType;											// 原点の種類
	D3DXVECTOR3             m_pos;													// 位置
	D3DXCOLOR               m_col;													// 色
	D3DXVECTOR2             m_size;													// 大きさ
	SETING_UV               m_TexUVInfo;											// テクスチャのUV座標の設定用の情報
	TEXTUREANIMEINFO        m_TexAnimInfo;											// テクスチャアニメーション情報
	float                   m_fRadius;												// 半径
	float                   m_fRotation;											// 回転量
	bool                    m_bDisp;												// 描画フラグ
};

#endif