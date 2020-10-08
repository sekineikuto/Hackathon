//*************************************************************************************************************
//
// 数字処理 [number.cpp]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "number.h"
#include "manager.h"
#include "Scene2D.h"

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CNumber::CNumber(int nNumOrder)
{

}

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CNumber::CNumber()
{

}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CNumber::~CNumber()
{

}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT CNumber::Load(void)
{
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CNumber::Unload(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 作成
//-------------------------------------------------------------------------------------------------------------
CNumber * CNumber::Create(float fSizeX, float fSizeY, D3DXVECTOR3 pos, float fRot, int nNumber, D3DXCOLOR col)
{
	// ナンバーの生成
	CNumber *pNumber = new CNumber;
	// 初期化
	pNumber->Init(fSizeX, fSizeY, nNumber, fRot, pos ,col);
	return pNumber;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CNumber::Init(float fSizeX, float fSizeY, int nNumber, float fRot, D3DXVECTOR3 pos, D3DXCOLOR col)
{
	m_nNumber	= nNumber;		// 数字
	m_size.x	= fSizeX;		// 大きさX
	m_size.y	= fSizeY;		// 大きさY
	m_fRot		= fRot;			// 回転量
	m_pos		= pos;			// 位置 
	m_col		= col;
	// 辺巣宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();	// デバイスのポインタ

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

	// 頂点の設定
	pVtx[0].pos.x = (pos.x + sinf(D3DX_PI + fRot)*fSizeY) + sinf(D3DX_PI*-0.5f + fRot)*fSizeX;
	pVtx[0].pos.y = (pos.y + cosf(D3DX_PI + fRot)*fSizeY) + cosf(D3DX_PI*-0.5f + fRot)*fSizeX;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = (pos.x + sinf(D3DX_PI + fRot)*fSizeY) + sinf(D3DX_PI*0.5f + fRot)*fSizeX;
	pVtx[1].pos.y = (pos.y + cosf(D3DX_PI + fRot)*fSizeY) + cosf(D3DX_PI*0.5f + fRot)*fSizeX;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = (pos.x + sinf(0.0f + fRot)*fSizeY) + sinf(D3DX_PI*-0.5f + fRot)*fSizeX;
	pVtx[2].pos.y = (pos.y + cosf(0.0f + fRot)*fSizeY) + cosf(D3DX_PI*-0.5f + fRot)*fSizeX;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = (pos.x + sinf(0.0f + fRot)*fSizeY) + sinf(D3DX_PI*0.5f + fRot)*fSizeX;
	pVtx[3].pos.y = (pos.y + cosf(0.0f + fRot)*fSizeY) + cosf(D3DX_PI*0.5f + fRot)*fSizeX;
	pVtx[3].pos.z = 0.0f;
	// 同次座標の設定
	pVtx[0].rhw = 1.0f;		//  1.0fで固定
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 1.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CNumber::Unint(void)
{
	// 頂点バッファがNULLじゃない時
	if (m_pVtxBuff != NULL)
	{// 頂点バッファの開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CNumber::Update(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CNumber::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// 頂点バッファをストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CRenderer::VERTEX_2D));

	// 頂点フォーマット設定
	pDevice->SetTexture(0, m_pTexture);

	// テクスチャの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-------------------------------------------------------------------------------------------------------------
// 数字の設定
//-------------------------------------------------------------------------------------------------------------
void CNumber::SetNumber(int nNumber)
{
	m_nNumber = nNumber;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の更新
//-------------------------------------------------------------------------------------------------------------
void CNumber::SetVaetex(int * nNumber, D3DXVECTOR3 * pos, D3DXCOLOR * col)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 数字ポインタがNULLじゃない時
	if (nNumber != NULL)
	{// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2((*nNumber*0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (*nNumber*0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((*nNumber*0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (*nNumber*0.1f), 1.0f);
	}
	// 位置ポインタがNULLじゃない時
	if (pos != NULL)
	{// 位置の設定
		m_pos = *pos;
		pVtx[0].pos.x = (pos->x + sinf(D3DX_PI + m_fRot)*m_size.y) + sinf(D3DX_PI*-0.5f + m_fRot)*m_size.x;
		pVtx[0].pos.y = (pos->y + cosf(D3DX_PI + m_fRot)*m_size.y) + cosf(D3DX_PI*-0.5f + m_fRot)*m_size.x;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = (pos->x + sinf(D3DX_PI + m_fRot)*m_size.y) + sinf(D3DX_PI*0.5f + m_fRot)*m_size.x;
		pVtx[1].pos.y = (pos->y + cosf(D3DX_PI + m_fRot)*m_size.y) + cosf(D3DX_PI*0.5f + m_fRot)*m_size.x;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = (pos->x + sinf(0.0f + m_fRot)*m_size.y) + sinf(D3DX_PI*-0.5f + m_fRot)*m_size.x;
		pVtx[2].pos.y = (pos->y + cosf(0.0f + m_fRot)*m_size.y) + cosf(D3DX_PI*-0.5f + m_fRot)*m_size.x;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = (pos->x + sinf(0.0f + m_fRot)*m_size.y) + sinf(D3DX_PI*0.5f + m_fRot)*m_size.x;
		pVtx[3].pos.y = (pos->y + cosf(0.0f + m_fRot)*m_size.y) + cosf(D3DX_PI*0.5f + m_fRot)*m_size.x;
		pVtx[3].pos.z = 0.0f;
	}
	// カラーポインタがNULLじゃない時
	if (col != NULL)
	{// 色の更新
		m_col = *col;
		pVtx[0].col = *col;
		pVtx[1].col = *col;
		pVtx[2].col = *col;
		pVtx[3].col = *col;
	}
	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}
//-------------------------------------------------------------------------------------------------------------
// 頂点カラー設定
//-------------------------------------------------------------------------------------------------------------
void CNumber::SetVeatexColor(void)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 大きさの設定
//-------------------------------------------------------------------------------------------------------------
void CNumber::SetSize(D3DXVECTOR2 * size)
{
	m_size = *size;
}

//-------------------------------------------------------------------------------------------------------------
// 回転量の設定
//-------------------------------------------------------------------------------------------------------------
void CNumber::SetRotation(float fRot)
{
	m_fRot = fRot;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャ情報の設定
//-------------------------------------------------------------------------------------------------------------
void CNumber::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
