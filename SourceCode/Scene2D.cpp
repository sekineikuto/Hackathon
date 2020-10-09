//*************************************************************************************************************
//
// シーン2D処理[Scene2D.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene2D.h"
#include "manager.h"
#include "keyboard.h"
#include "DebugProc.h"

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CScene2D::CScene2D(CScene::PRIORITY priority) : CScene(priority)
{
	m_OriginType = ORIGINVERTEXTYPE_CENTER;
	m_pos = { 0.0f,0.0f,0.0f };
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_size = { 0.0f,0.0f };
	m_TexUVInfo = { 0,1,0,1 };
	m_TexAnimInfo = { 0,0,0 };
	m_fRadius = { 0.0f };
	m_fRotation = { 0.0f };
	m_bDisp = true;
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CScene2D::~CScene2D()
{
}

//-------------------------------------------------------------------------------------------------------------
// 細かい作成
//-------------------------------------------------------------------------------------------------------------
CScene2D * CScene2D::Create(CScene::PRIORITY priority, D3DXVECTOR3 pos, D3DXCOLOR col, ORIGINVERTEXTYPE OriginType, D3DXVECTOR2 size, float fRadius, float m_fRotation, SETING_UV nTexVari)
{
	CScene2D *pScene2D = new CScene2D(priority);
	pScene2D->SetColor(col);
	pScene2D->SetOriginType(OriginType);
	pScene2D->SetPosition(pos);
	pScene2D->SetRotation(m_fRotation);
	pScene2D->SetRadius(fRadius);
	pScene2D->SetSize(size);
	pScene2D->SetSetingUV(nTexVari);
	pScene2D->Init();
	return pScene2D;
}

//-------------------------------------------------------------------------------------------------------------
// 作成
//-------------------------------------------------------------------------------------------------------------
CScene2D * CScene2D::Create(CScene::PRIORITY priority, D3DXVECTOR3 pos, ORIGINVERTEXTYPE OriginType, D3DXVECTOR2 size, float fRadius)
{
	CScene2D *pScene2D = new CScene2D(priority);
	pScene2D->SetOriginType(OriginType);
	pScene2D->SetPosition(pos);
	pScene2D->SetRadius(fRadius);
	pScene2D->SetSize(size);
	pScene2D->Init();
	return pScene2D;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CScene2D::Init(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();
	
	m_bDisp = true;

	// 頂点の作成
	this->MakeVatex(pDevice);
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CScene2D::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CScene2D::Update(void)
{
	if (CManager::GetKeyboard().GetTrigger(DIK_DELETE))
	{
		Release();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CScene2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = CManager::GetRenderer().GetDevice();

	// 頂点バッファをストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CRenderer::VERTEX_2D));

	// 頂点フォーマット設定
	pDevice->SetTexture(0, m_pTexture);

	// テクスチャの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	if (m_bDisp)
	{
		// ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の更新
//-------------------------------------------------------------------------------------------------------------
void CScene2D::UpdateVertex(D3DXVECTOR3 * pos, D3DXVECTOR2 * size, float * fRadius, D3DXCOLOR * col, SETING_UV * TexVari)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pos != NULL) {
		m_pos = *pos;
	}
	if (size != NULL) {
		m_size = *size;
	}
	if (fRadius != NULL) {
		m_fRadius = *fRadius;
	}
	if (pos != NULL || size != NULL || fRadius != NULL)
	{
		this->SetVatexPosition(pVtx);
	}

	if (col != NULL)
	{
		m_col = *col;
		this->SetVatexColor(pVtx);
	}

	if (TexVari != NULL)
	{
		m_TexUVInfo = *TexVari;
		this->SetVatexTexture(pVtx);
	}

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の更新
//-------------------------------------------------------------------------------------------------------------
void CScene2D::UpdateVertex(bool bUpdatePos, bool bUpdateCol, bool bUpdateUVTex)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (bUpdatePos)
	{
		this->SetVatexPosition(pVtx);
	}
	if (bUpdateCol)
	{
		this->SetVatexColor(pVtx);
	}
	if (bUpdateUVTex)
	{
		this->SetVatexTexture(pVtx);
	}
	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 頂点位置の更新
//-------------------------------------------------------------------------------------------------------------
void CScene2D::UpdateVatexPosition(void)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;
	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	this->SetVatexPosition(pVtx);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 頂点カラーの更新
//-------------------------------------------------------------------------------------------------------------
void CScene2D::UpdateVatexColor(void)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;
	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	this->SetVatexColor(pVtx);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 頂点テクスチャの更新
//-------------------------------------------------------------------------------------------------------------
void CScene2D::UpdateVatexTexture(void)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;
	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点テクスチャの設定
	this->SetVatexTexture(pVtx);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の原点の変更
//-------------------------------------------------------------------------------------------------------------
void CScene2D::ChangeVertexOrigin(ORIGINVERTEXTYPE OriginType)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_OriginType != OriginType)
	{
		m_OriginType = OriginType;
		this->SetVatexPosition(pVtx);
	}

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// バインドテクスチャ
//-------------------------------------------------------------------------------------------------------------
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//-------------------------------------------------------------------------------------------------------------
// バッファの取得
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetpVtxBuff(void)
{
	return m_pVtxBuff;
}

//-------------------------------------------------------------------------------------------------------------
// 原点種類の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetOriginType(ORIGINVERTEXTYPE OriginType)
{
	m_OriginType = OriginType;
}

//-------------------------------------------------------------------------------------------------------------
// 原点種類の取得
//-------------------------------------------------------------------------------------------------------------
ORIGINVERTEXTYPE *CScene2D::GetOriginType(void)
{
	return &m_OriginType;
}

//-------------------------------------------------------------------------------------------------------------
// 位置の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetPosition(D3DXVECTOR3& pos)
{
	m_pos = pos;
}

//-------------------------------------------------------------------------------------------------------------
// 位置の取得
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 *CScene2D::GetPosition(void)
{
	return &m_pos;
}

//-------------------------------------------------------------------------------------------------------------
// 色の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetColor(D3DXCOLOR& col)
{
	m_col = col;
}

//-------------------------------------------------------------------------------------------------------------
// 色の取得
//-------------------------------------------------------------------------------------------------------------
D3DXCOLOR *CScene2D::GetColor(void)
{
	return &m_col;
}

//-------------------------------------------------------------------------------------------------------------
// 大きさの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSize(D3DXVECTOR2& size)
{
	m_size = size;
}

//-------------------------------------------------------------------------------------------------------------
// 大きさXの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSizeX(float size)
{
	m_size.x = size;
}

//-------------------------------------------------------------------------------------------------------------
// 大きさYの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSizeY(float size)
{
	m_size.y = size;
}

//-------------------------------------------------------------------------------------------------------------
// 大きさの取得
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR2 *CScene2D::GetSize(void)
{
	return &m_size;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャのUV座標の設定用の情報
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSetingUV(SETING_UV& SetingUV)
{
	m_TexUVInfo = SetingUV;
}

//-------------------------------------------------------------------------------------------------------------
// // テクスチャのUV座標の設定用の情報横のカウントの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSetingUVCountU(int nCntU)
{
	m_TexUVInfo.nCntU = nCntU;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャのUV座標の設定用の情報縦のカウントの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSetingUVCountV(int nCntV)
{
	m_TexUVInfo.nCntV = nCntV;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャのUV座標の設定用の情報の取得
//-------------------------------------------------------------------------------------------------------------
SETING_UV *CScene2D::GetSetingUV(void)
{
	return &m_TexUVInfo;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャアニメーション用変数の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetAnimTexVariables(TEXTUREANIMEINFO& AnimTexVari)
{
	m_TexAnimInfo = AnimTexVari;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャアニメーション用フレーム変数の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetAnimTexVariFrame(int nFrame)
{
	m_TexAnimInfo.nFrame = nFrame;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャアニメーション用最大フレーム変数の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetAnimTexVariFrameMax(int nFrameMax)
{
	m_TexAnimInfo.nFrameMax = nFrameMax;
}
//-------------------------------------------------------------------------------------------------------------
// テクスチャアニメーション用変数の取得
//-------------------------------------------------------------------------------------------------------------
TEXTUREANIMEINFO * CScene2D::GetAnimTexVariables(void)
{
	return &m_TexAnimInfo;
}

//-------------------------------------------------------------------------------------------------------------
// 半径の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

//-------------------------------------------------------------------------------------------------------------
// 半径の取得
//-------------------------------------------------------------------------------------------------------------
float CScene2D::GetRadius(void)
{
	return m_fRadius;
}

//-------------------------------------------------------------------------------------------------------------
// 回転量の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetRotation(float fRotation)
{
	m_fRotation = fRotation;
}

//-------------------------------------------------------------------------------------------------------------
// 回転量の取得
//-------------------------------------------------------------------------------------------------------------
float CScene2D::GetRotation(void)
{
	return m_fRotation;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点テクスチャ座表計算
//-------------------------------------------------------------------------------------------------------------
int CScene2D::CalculatVatexUVMap(bool bLoop, int nFrameMax, int & nFrame, int nNumWidht, int nNumHight, int & nCountWidht, int & nCountHight)
{
	// 変数宣言
	int nState = -1;

	// フレーム数が最大値に到達した時
	if (nFrame >= nFrameMax)
	{	// フレームを初期化
		nFrame = 0;

		// x軸のカウントをクリアメント
		nCountWidht++;
		nState = true;

		// x軸のカウントが既定値に到達し時
		if (nNumWidht <= nCountWidht)
		{	// x軸のカウントを初期化
			nCountWidht = 0;

			// y軸のカウントをクリアメント
			nCountHight++;
			nState = true;

			// y軸のカウントが既定値に到達し時
			if (nNumHight <= nCountHight)
			{	// y軸のカウントを初期化
				nCountHight = 0;
				if (bLoop = false)
				{
					nState = false;
				}
			}
		}
	}
	else
	{	// フレーム数をクリアメント
		nFrame++;
	}

	return nState;
}

//-------------------------------------------------------------------------------------------------------------
// 選択可能な頂点テクスチャ座表計算
//-------------------------------------------------------------------------------------------------------------
void CScene2D::CalSelectionUVMap(bool bIteration, int nFrameMax, int & nFrame, int nNumWidht, int & nCountWidht, int nCountHight, int & m_nIteration)
{
	// フレーム数が最大値に到達した時
	if (nFrame >= nFrameMax)
	{
		// フレームを初期化
		nFrame = 0;

		// x軸のカウントをクリアメント
		nCountWidht += m_nIteration;

		// 反復する場合
		if (bIteration == true)
		{// x軸のカウントが既定値に到達し時
			if (nNumWidht - 1 <= nCountWidht)
			{
				m_nIteration = -1;
			}
			else if (0 >= nCountWidht)
			{
				m_nIteration = 1;
			}
		}
		else
		{
			if (nNumWidht - 1 <= nCountWidht)
			{
				nCountWidht = nNumWidht - 1;
			}
			else if (0 >= nCountWidht)
			{
				nCountWidht = 0;
			}
		}
	}
	else
	{	// フレーム数をクリアメント
		nFrame++;
	}

}

//-------------------------------------------------------------------------------------------------------------
// 反復頂点テクスチャ座表計算
//-------------------------------------------------------------------------------------------------------------
void CScene2D::CalTurnBackUVMap(int nFrameMax, int & nFrame, int nNumWidht, int & nCountWidht, int nCountHight, int & m_nIteration)
{
	// フレーム数が最大値に到達した時
	if (nFrame >= nFrameMax)
	{	// フレームを初期化
		nFrame = 0;
		// x軸のカウントが既定値に到達し時
		if (nNumWidht - 1 <= nCountWidht)
		{
			m_nIteration = -1;
		}
		else if (0 >= nCountWidht)
		{
			m_nIteration = 1;
		}
		// x軸のカウントをクリアメント
		nCountWidht += m_nIteration;
	}
	else
	{	// フレーム数をクリアメント
		nFrame++;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の作成
//-------------------------------------------------------------------------------------------------------------
HRESULT CScene2D::MakeVatex(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(CRenderer::VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_OriginType)
	{
	case ORIGINVERTEXTYPE_LOWERLEFT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_LOWERCENTER:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x*0.5f, m_pos.y - m_size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x*0.5f, m_pos.y - m_size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x*0.5f, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x*0.5f, m_pos.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_LOWERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_CENTERLEFT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y*0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y*0.5f, 0.0f);
		break;
	case ORIGINVERTEXTYPE_CENTER:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x*0.5f, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x*0.5f, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x*0.5f, m_pos.y + m_size.y*0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x*0.5f, m_pos.y + m_size.y*0.5f, 0.0f);
		break;
	case ORIGINVERTEXTYPE_CENTERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y*0.5f, 0.0f);
		break;
	case ORIGINVERTEXTYPE_UPPERLEFT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_UPPERCENTER:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_UPPERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_ROTATION:
		// 頂点の設定
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI*-0.75f)*m_fRadius;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI*-0.75f)*m_fRadius;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI*0.75f)*m_fRadius;
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI*0.75f)*m_fRadius;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(D3DX_PI*-0.25f)*m_fRadius;
		pVtx[2].pos.y = m_pos.y + cosf(D3DX_PI*-0.25f)*m_fRadius;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(D3DX_PI*0.25f)*m_fRadius;
		pVtx[3].pos.y = m_pos.y + cosf(D3DX_PI*0.25f)*m_fRadius;
		pVtx[3].pos.z = 0.0f;
		break;
	case ORIGINVERTEXTYPE_ROTCENTER:
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[2].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[3].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[3].pos.z = 0.0f;
		break;
	case ORIGINVERTEXTYPE_ROTCENTERLEFT:
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y + sinf(D3DX_PI*0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y + cosf(D3DX_PI*0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y;
		pVtx[2].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y + sinf(D3DX_PI*0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[3].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y + cosf(D3DX_PI*0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[3].pos.z = 0.0f;
		break;

	case ORIGINVERTEXTYPE_ROTLOWERCENTER:
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x + sinf(D3DX_PI + m_fRotation)*(m_size.y*2.0f);
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x + cosf(D3DX_PI + m_fRotation)*(m_size.y*2.0f);
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x + sinf(D3DX_PI + m_fRotation)*(m_size.y*2.0f);
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x + cosf(D3DX_PI + m_fRotation)*(m_size.y*2.0f);
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[2].pos.y = m_pos.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[3].pos.y = m_pos.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[3].pos.z = 0.0f;

		break;
	case ORIGINVERTEXTYPE_ROTCENTERRIGHT:
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y + sinf(D3DX_PI*-0.5f + m_fRotation)*(m_size.x)*2.0f;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y + cosf(D3DX_PI*-0.5f + m_fRotation)*(m_size.x)*2.0f;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y;
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y + sinf(D3DX_PI*-0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[2].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y + cosf(D3DX_PI*-0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y;
		pVtx[3].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y;
		pVtx[3].pos.z = 0.0f;
		break;
	case ORIGINVERTEXTYPE_ROTUPPERCENTER:
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x + sinf(0.0f + m_fRotation)*(m_size.y*2.0f);;
		pVtx[2].pos.y = m_pos.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x + cosf(0.0f + m_fRotation)*(m_size.y*2.0f);;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x + sinf(0.0f + m_fRotation)*(m_size.y*2.0f);;
		pVtx[3].pos.y = m_pos.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x + cosf(0.0f + m_fRotation)*(m_size.y*2.0f);;
		pVtx[3].pos.z = 0.0f;
		break;
	}

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標
	float fNumU = 1.0f / m_TexUVInfo.nNumU;
	float fNumV = 1.0f / m_TexUVInfo.nNumV;

	pVtx[0].tex = D3DXVECTOR2(0.0f + (fNumU*m_TexUVInfo.nCntU), 0.0f + (fNumV*m_TexUVInfo.nCntV));
	pVtx[1].tex = D3DXVECTOR2(fNumU + (fNumU*m_TexUVInfo.nCntU), 0.0f + (fNumV*m_TexUVInfo.nCntV));
	pVtx[2].tex = D3DXVECTOR2(0.0f + (fNumU*m_TexUVInfo.nCntU), fNumV + (fNumV*m_TexUVInfo.nCntV));
	pVtx[3].tex = D3DXVECTOR2(fNumU + (fNumU*m_TexUVInfo.nCntU), fNumV + (fNumV*m_TexUVInfo.nCntV));

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点位置の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetVatexPosition(CRenderer::VERTEX_2D * pVtx)
{
	switch (m_OriginType)
	{

	case ORIGINVERTEXTYPE_LOWERLEFT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_LOWERCENTER:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x*0.5f, m_pos.y - m_size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x*0.5f, m_pos.y - m_size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x*0.5f, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x*0.5f, m_pos.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_LOWERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_CENTERLEFT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y*0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y*0.5f, 0.0f);
		break;
	case ORIGINVERTEXTYPE_CENTER:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x*0.5f, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x*0.5f, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x*0.5f, m_pos.y + m_size.y*0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x*0.5f, m_pos.y + m_size.y*0.5f, 0.0f);
		break;
	case ORIGINVERTEXTYPE_CENTERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y*0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y*0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y*0.5f, 0.0f);
		break;
	case ORIGINVERTEXTYPE_UPPERLEFT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_UPPERCENTER:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_UPPERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
		break;
	case ORIGINVERTEXTYPE_ROTATION:
		// 頂点の設定
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI*-0.75f)*m_fRadius;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI*-0.75f)*m_fRadius;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI*0.75f)*m_fRadius;
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI*0.75f)*m_fRadius;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(D3DX_PI*-0.25f)*m_fRadius;
		pVtx[2].pos.y = m_pos.y + cosf(D3DX_PI*-0.25f)*m_fRadius;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(D3DX_PI*0.25f)*m_fRadius;
		pVtx[3].pos.y = m_pos.y + cosf(D3DX_PI*0.25f)*m_fRadius;
		pVtx[3].pos.z = 0.0f;
		break;
	case ORIGINVERTEXTYPE_ROTCENTER:
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[2].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[3].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[3].pos.z = 0.0f;
		break;
	case ORIGINVERTEXTYPE_ROTCENTERLEFT:
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y + sinf(D3DX_PI*0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y + cosf(D3DX_PI*0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y;
		pVtx[2].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y + sinf(D3DX_PI*0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[3].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y + cosf(D3DX_PI*0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[3].pos.z = 0.0f;
		break;

	case ORIGINVERTEXTYPE_ROTLOWERCENTER:
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x + sinf(D3DX_PI + m_fRotation)*(m_size.y*2.0f);
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x + cosf(D3DX_PI + m_fRotation)*(m_size.y*2.0f);
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x + sinf(D3DX_PI + m_fRotation)*(m_size.y*2.0f);
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x + cosf(D3DX_PI + m_fRotation)*(m_size.y*2.0f);
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[2].pos.y = m_pos.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[3].pos.y = m_pos.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[3].pos.z = 0.0f;

		break;
	case ORIGINVERTEXTYPE_ROTCENTERRIGHT:
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y + sinf(D3DX_PI*-0.5f + m_fRotation)*(m_size.x)*2.0f;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y + cosf(D3DX_PI*-0.5f + m_fRotation)*(m_size.x)*2.0f;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI + m_fRotation)*m_size.y;
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI + m_fRotation)*m_size.y;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y + sinf(D3DX_PI*-0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[2].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y + cosf(D3DX_PI*-0.5f + m_fRotation)*(m_size.x*2.0f);
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(0.0f + m_fRotation)*m_size.y;
		pVtx[3].pos.y = m_pos.y + cosf(0.0f + m_fRotation)*m_size.y;
		pVtx[3].pos.z = 0.0f;
		break;
	case ORIGINVERTEXTYPE_ROTUPPERCENTER:
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_pos.x + sinf(D3DX_PI*-0.5f + m_fRotation)*m_size.x + sinf(0.0f + m_fRotation)*(m_size.y*2.0f);;
		pVtx[2].pos.y = m_pos.y + cosf(D3DX_PI*-0.5f + m_fRotation)*m_size.x + cosf(0.0f + m_fRotation)*(m_size.y*2.0f);;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_pos.x + sinf(D3DX_PI*0.5f + m_fRotation)*m_size.x + sinf(0.0f + m_fRotation)*(m_size.y*2.0f);;
		pVtx[3].pos.y = m_pos.y + cosf(D3DX_PI*0.5f + m_fRotation)*m_size.x + cosf(0.0f + m_fRotation)*(m_size.y*2.0f);;
		pVtx[3].pos.z = 0.0f;
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点カラーの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetVatexColor(CRenderer::VERTEX_2D * pVtx)
{
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点テクスチャ座標の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetVatexTexture(CRenderer::VERTEX_2D * pVtx)
{
	// テクスチャ座標
	float fNumU = 1.0f / m_TexUVInfo.nNumU;
	float fNumV = 1.0f / m_TexUVInfo.nNumV;

	pVtx[0].tex = D3DXVECTOR2(0.0f + (fNumU*m_TexUVInfo.nCntU), 0.0f + (fNumV*m_TexUVInfo.nCntV));
	pVtx[1].tex = D3DXVECTOR2(fNumU + (fNumU*m_TexUVInfo.nCntU), 0.0f + (fNumV*m_TexUVInfo.nCntV));
	pVtx[2].tex = D3DXVECTOR2(0.0f + (fNumU*m_TexUVInfo.nCntU), fNumV + (fNumV*m_TexUVInfo.nCntV));
	pVtx[3].tex = D3DXVECTOR2(fNumU + (fNumU*m_TexUVInfo.nCntU), fNumV + (fNumV*m_TexUVInfo.nCntV));

}
