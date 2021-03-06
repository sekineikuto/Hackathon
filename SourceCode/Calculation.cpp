// ----------------------------------------------------------------------------------------------------
//
// Calculation処理の説明[calculation.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------
#include "Calculation.h"
#include "debugproc.h"
#include "renderer.h"

// ----------------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ----------------------------------------------------------------------------------------------------
bool CCalculation::m_bWire = false;

// ----------------------------------------------------------------------------------------------------
// 外積の計算
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::Cross_product(
	D3DXVECTOR3 &VecA,
	D3DXVECTOR3 &VecB)
{
	return D3DXVECTOR3(
		VecA.y * VecB.z - VecA.z * VecB.y,
		VecA.z * VecB.x - VecA.x * VecB.z,
		VecA.x * VecB.y - VecA.y * VecB.x);
}

// ----------------------------------------------------------------------------------------------------
// 内積の計算
// ----------------------------------------------------------------------------------------------------
float CCalculation::Dot_product(D3DXVECTOR3 &VecA, D3DXVECTOR3 &VecB)
{
	return (
		VecA.x * VecB.x +
		VecA.y * VecB.y +
		VecA.z * VecB.z);
}

// ----------------------------------------------------------------------------------------------------
// 底面の四角形の中に入っているかいないか
// ----------------------------------------------------------------------------------------------------
bool CCalculation::SquareBottom_Judg(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	int			nBlock_W,
	int			nBlock_H,
	D3DXVECTOR3 worldpos
)
{
	// 変数宣言
	D3DXVECTOR3 SquareDiff[4];
	D3DXVECTOR3 WorldDiff[4];
	//float		fCross_p[4];
	bool		bCross = false;

	// 左の距離
	SquareDiff[0] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H));

	// 上の距離
	SquareDiff[1] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H));

	// 右の距離
	SquareDiff[2] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H));

	// 下の距離
	SquareDiff[3] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H));

	// 左の距離
	WorldDiff[0] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		worldpos);

	// 上の距離
	WorldDiff[1] = Difference_Between(
		D3DXVECTOR3(
			pos.x - size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		worldpos);

	// 右の距離
	WorldDiff[2] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z + size.z * 0.5f * nBlock_H),
		worldpos);

	// 下の距離
	WorldDiff[3] = Difference_Between(
		D3DXVECTOR3(
			pos.x + size.x * 0.5f * nBlock_W,
			pos.y,
			pos.z - size.z * 0.5f * nBlock_H),
		worldpos);

	// 外積の中か外か
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		/*
		// 外積
		fCross_p[nCnt] = Cross_product(SquareDiff[nCnt], WorldDiff[nCnt]);

		// 外積の判定 //
		// 中
		if (fCross_p[nCnt] > 0)
		{
			bCross = true;
		}

		// 外
		else
		{
			return false;
		}
		*/
	}

	// 中か外かを返す
	return bCross;
}

// ----------------------------------------------------------------------------------------------------
// 側面の四角形の中に入っているかいないか
// ----------------------------------------------------------------------------------------------------
bool CCalculation::SquareSide_Judg(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	int nBlock_W,
	int nBlock_H,
	D3DXVECTOR3 worldpos)
{
	// 高さの範囲
	if (worldpos.y < pos.y + size.y * nBlock_H &&
		worldpos.y >= pos.y)
	{
		// 横の範囲
		if (worldpos.x > pos.x - size.x * 0.5f * nBlock_W * sinf(rot.y) &&
			worldpos.x < pos.x + size.x * 0.5f * nBlock_W * sinf(rot.y))
		{
			return true;
		}
		// 奥の範囲
		if (worldpos.z > pos.z - size.x * 0.5f * nBlock_W * cosf(rot.y) &&
			worldpos.z < pos.z + size.x * 0.5f * nBlock_W * cosf(rot.y))
		{
			return true;
		}
	}


	// 中か外かを返す
	return false;
}

// ----------------------------------------------------------------------------------------------------
// 座標同士の差計算
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::Difference_Between(
	D3DXVECTOR3 &posA,
	D3DXVECTOR3 &posB)
{
	// 差
	D3DXVECTOR3 Diff;

	//座標同士の差計算
	Diff = posA - posB;

	return Diff;
}

// ----------------------------------------------------------------------------------------------------
// 間の差の計算(3D用:位置、位置)
// ----------------------------------------------------------------------------------------------------
float CCalculation::DiffPoint(
	D3DXVECTOR3 const & posA,
	D3DXVECTOR3 const & posB
)
{
	// 変数宣言
	D3DXVECTOR3 diffpos;	// 差分の位置
	float diff,diff2;		// 差
	// 差分の位置の計算
	diffpos = posA - posB;
	// xとyの斜めの長さ
	diff = sqrt(diffpos.x * diffpos.x + diffpos.y * diffpos.y);
	// 先程出た斜めとzの長さ
	diff2 = sqrt(diff * diff + diffpos.z * diffpos.z);
	return diff2;
}

// ----------------------------------------------------------------------------------------------------
// 間の差の計算(2D用:位置、位置)
// ----------------------------------------------------------------------------------------------------
float CCalculation::DiffPoint(
	D3DXVECTOR2 const & posA,
	D3DXVECTOR2 const & posB
)
{
	// 変数宣言
	D3DXVECTOR2 diffpos;	// 差分の位置
	float diff;		// 差
	// 差分の位置の計算
	diffpos = posA - posB;
	// xとyの斜めの長さ
	diff = sqrt(diffpos.x * diffpos.x + diffpos.y * diffpos.y);
	return diff;
}

// ----------------------------------------------------------------------------------------------------
// 間の差の2乗計算(3D用:位置、位置)
// ----------------------------------------------------------------------------------------------------
float CCalculation::DiffPointSquare(D3DXVECTOR3 const & posA, D3DXVECTOR3 const & posB)
{
	// 変数宣言
	float PointSquare = DiffPoint(posA, posB);	// 距離
	return PointSquare * PointSquare;
}

// ----------------------------------------------------------------------------------------------------
// 間の差の2乗計算(2D用:位置、位置)
// ----------------------------------------------------------------------------------------------------
float CCalculation::DiffPointSquare(D3DXVECTOR2 const & posA, D3DXVECTOR2 const & posB)
{
	// 変数宣言
	float PointSquare = DiffPoint(posA, posB);	// 距離
	return PointSquare * PointSquare;
}

// ----------------------------------------------------------------------------------------------------
// 球の足し算計算
// ----------------------------------------------------------------------------------------------------
float CCalculation::Addition_Sphere(
	float Length_x,
	float Length_y,
	float Length_z)
{
	// 変数宣言
	float fLengthX = 0;
	float fLengthY = 0;
	float fLengthZ = 0;
	float Lenght = 0;

	// 円の計算
	fLengthX = Length_x * Length_x;
	fLengthY = Length_y * Length_y;
	fLengthZ = Length_z * Length_z;

	// 円同士の足し算
	Lenght = fLengthX + fLengthY + fLengthZ;

	return Lenght;
}

// ----------------------------------------------------------------------------------------------------
// 円の当たり判定
// ----------------------------------------------------------------------------------------------------
bool CCalculation::Collision_Circle(
	D3DXVECTOR2 &Me,
	float		fMeSize,
	D3DXVECTOR2 &Opponent,
	float		fOppSize
)
{
	// 変数宣言
	float fRange;		// 範囲
	float fDistance;	// 距離

	// 範囲の計算
	//fRange = fMeSize * fMeSize + fOppSize * fOppSize;
	fRange = (fMeSize + fOppSize) * (fMeSize + fOppSize);
	// 実際の距離の計算
	fDistance =
		(Me.x - Opponent.x) * (Me.x - Opponent.x) +
		(Me.y - Opponent.y) * (Me.y - Opponent.y);
	// 当たっているかいないかを返す
	return (fRange >= fDistance) ? true : false;
}

// ----------------------------------------------------------------------------------------------------
// 球の当たり判定
// ----------------------------------------------------------------------------------------------------
bool CCalculation::Collision_Sphere(
	D3DXVECTOR3 const &Me,
	float const &fMeSize,
	D3DXVECTOR3 const &Opponent,
	float const &fOppSize
)
{
	// 変数宣言
	float fRange;		// 範囲
	float fDistance;	// 距離

	// 範囲の計算
	fRange = fMeSize * fMeSize + fOppSize * fOppSize;

	// 実際の距離の計算
	fDistance =
		(Me.x - Opponent.x) * (Me.x - Opponent.x) +
		(Me.y - Opponent.y) * (Me.y - Opponent.y) +
		(Me.z - Opponent.z) * (Me.z - Opponent.z);

	// 当たっているかいないかを返す
	return (fRange >= fDistance) ? true : false;
}

// ----------------------------------------------------------------------------------------------------
// 回転の限界計算
// ----------------------------------------------------------------------------------------------------
float CCalculation::Rot_One_Limit(float &fRot)
{
	// -3.14〜3.14に回転差分を固定させる
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;		// -3.14未満
	}

	else if (fRot > D3DX_PI)
	{
		fRot += -D3DX_PI * 2;	// 3.14超過
	}

	return fRot;
}

// ----------------------------------------------------------------------------------------------------
// スクリーン座標をワールド座標に変換
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3* CCalculation::CalcScreenToWorld(
	D3DXVECTOR3* pout,	// マウスポイント
	float Sx,				// スクリーンX座標
	float Sy,				// スクリーンY座標
	float fZ,			// 射影空間でのZ値（0〜1）
	float Screen_w,		// スクリーンの横幅
	float Screen_h,		// スクリーンの高さ
	D3DXMATRIX* mtxView,// ビューマトリックス
	D3DXMATRIX* mtxPrj)	// プロジェクションマトリックス
{
	// 各行列の逆行列を算出 //
	// 変数宣言
	D3DXMATRIX mtxInvViex;		// 計算用逆行列ビューマトリックス
	D3DXMATRIX mtxInvPrj;		// 計算用逆行列プロジェクションマトリックス
	D3DXMATRIX mtxVP;			// 計算用ビューポット
	D3DXMATRIX mtxInvViexport;	// 計算用逆行列ビューポット
	D3DXMATRIX mtxChange;		// 座標変換用マトリックス

	// ビューポットの行列作成
	D3DXMatrixIdentity(&mtxVP);	// 初期化
	mtxVP._11 = Screen_w / 2.0f;
	mtxVP._22 = -Screen_h / 2.0f;
	mtxVP._41 = Screen_w / 2.0f;
	mtxVP._42 = Screen_h / 2.0f;

	// 逆行列の計算
	D3DXMatrixInverse(&mtxInvViex, NULL, mtxView);		// ビューマトリックス
	D3DXMatrixInverse(&mtxInvPrj, NULL, mtxPrj);		// プロジェクションマトリックス
	D3DXMatrixInverse(&mtxInvViexport, NULL, &mtxVP);	// ビューポットマトリックス

	// スクリーンからワールドへ
	mtxChange = mtxInvViexport * mtxInvPrj * mtxInvViex;		// 座標変換用
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &mtxChange);	// スクリーンからワールドへ

	// ワールドのポイントを返す
	return pout;
}

// ----------------------------------------------------------------------------------------------------
// スクリーン座標をXZ平面のワールド座標交点算出
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3* CCalculation::CalcScreenToXZ(
	D3DXVECTOR3* pout,		// ワールド座標の入れ物
	float Sx,				// スクリーンX座標
	float Sy,				// スクリーンY座標
	float Screen_w,			// スクリーンの横幅
	float Screen_h,			// スクリーンの高さ
	D3DXMATRIX* mtxView,	// ビューマトリックス
	D3DXMATRIX* mtxPrj,		// プロジェクションマトリックス
	D3DXVECTOR3 obj			// オブジェクトの位置
)
{
	// 変数宣言
	D3DXVECTOR3 Mearpos;	// 最近値
	D3DXVECTOR3 Farpos;		// 最遠値
	D3DXVECTOR3 Ray;		// マウスポイント方向

	// スクリーン座標をワールド座標に変換
	CalcScreenToWorld(		// 最近値
		&Mearpos,
		Sx, Sy,
		0.0f,
		Screen_w, Screen_h,
		mtxView,
		mtxPrj);

	CalcScreenToWorld(		// 最遠値
		&Farpos,
		Sx, Sy,
		1.0f,
		Screen_w, Screen_h,
		mtxView,
		mtxPrj);

	// マウスポイント方向
	Ray = Farpos - Mearpos;
	D3DXVec3Normalize(&Ray, &Ray);	// 正規化

	D3DXVECTOR3 diff;

	// 改善する必要がある
	diff = Mearpos - obj;
	diff = -(*D3DXVec3Normalize(&diff, &diff));

	// 下床との交点 //
	if (Ray.y <= 0)
	{
		// 床交点
		float LRay = D3DXVec3Dot(&Ray, &D3DXVECTOR3(0, 1, 0));		// マウスの方向の内積
		float LP0 = D3DXVec3Dot(&(obj - Mearpos), &D3DXVECTOR3(0, 1, 0));	// 原点の内積
		*pout = Mearpos + (LP0 / LRay)*Ray;
	}

	/* 起きていない場合は遠くの壁との交点を出力	*/
	// 上床との交点 //
	else
	{
		// 床交点
		float LRay = D3DXVec3Dot(&Ray, &D3DXVECTOR3(0, -1, 0));		// マウスの方向の内積
		float LP0 = D3DXVec3Dot(&(obj - Mearpos), &D3DXVECTOR3(0, -1, 0));	// 原点の内積
		*pout = Mearpos + (LP0 / LRay)*Ray;

		//*pout = Farpos;
	}

	return pout;
}

// ----------------------------------------------------------------------------------------------------
// ワールド座標をスクリーン座標に変換(多分)
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3* CCalculation::CalcWorldToScreen(
	D3DXVECTOR3* Obj,	// オブジェクトの位置
	float Sx,			// スクリーンX座標
	float Sy,			// スクリーンY座標
	float fZ,			// 射影空間でのZ値（0〜1）
	float Screen_w,		// スクリーンの横幅
	float Screen_h,		// スクリーンの高さ
	D3DXMATRIX* mtxView,// ビューマトリックス
	D3DXMATRIX* mtxPrj)	// プロジェクションマトリックス
{
	// 各行列の逆行列を算出 //
	// 変数宣言
	D3DXMATRIX mtxVP;			// 計算用ビューポット
	D3DXMATRIX mtxChange;				//

	// ビューポットの行列作成
	D3DXMatrixIdentity(&mtxVP);			// 初期化
	mtxVP._11 = SCREEN_WIDTH / 2.0f;
	mtxVP._22 = -SCREEN_HEIGHT / 2.0f;
	mtxVP._41 = SCREEN_WIDTH / 2.0f;
	mtxVP._42 = SCREEN_HEIGHT / 2.0f;

	mtxChange = mtxVP * *mtxPrj * *mtxView;
	D3DXVec3TransformCoord(Obj, &D3DXVECTOR3(Sx, Sy, fZ), &mtxChange);

	return Obj;
}

// ----------------------------------------------------------------------------------------------------
// 一次関数
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR2 CCalculation::Linear_function(D3DXVECTOR3 Apos, D3DXVECTOR3 Bpos)
{
	// 変数宣言
	float fCons;	// 比例定数
	float fSection;	// 切片

	// 比例定数計算
	fCons = (Apos.y - Bpos.y) / (Apos.x - Bpos.x);
	// 切片計算
	fSection = Apos.y - Apos.x * fCons;
	// 比例定数と切片の計算結果を返す
	return D3DXVECTOR2(fCons, fSection);
}

// ----------------------------------------------------------------------------------------------------
// 2線分の交点(連立方程式)
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::TwoLine_Inse(D3DXVECTOR2 ALinear, D3DXVECTOR2 BLinear)
{
	// 変数宣言
	D3DXVECTOR3 Insecsion;	// 交点

	/* 交点計算 */
	// x
	Insecsion.x = (BLinear.y - ALinear.y) / (ALinear.x - BLinear.x);
	// y
	Insecsion.y = ALinear.x * Insecsion.x + ALinear.y;
	// 交点を返す
	return Insecsion;
}

// ----------------------------------------------------------------------------------------------------
// 途中の計算
// ----------------------------------------------------------------------------------------------------
bool CCalculation::TlyCollision(
	D3DXVECTOR3 ObjectPos,
	D3DXVECTOR3 pos,
	...
)
{
	// 変数宣言
	D3DXVECTOR3 posA,posB;
	va_list va;	// 可変変数
	int nCntMax;
	// 可変引数を1個にまとめる
	va_start(va, pos);
	nCntMax = (sizeof(va) / sizeof(D3DXVECTOR3));
	for (int nCnt = 0; nCnt < (sizeof(va) / sizeof(D3DXVECTOR3)); nCnt++)
	{
		posA = va_arg(va, D3DXVECTOR3);
		posB = va_arg(va, D3DXVECTOR3);

	}
	va_end(va);

	return false;
}

// ----------------------------------------------------------------------------------------------------
// 外積の当たり判定
// ----------------------------------------------------------------------------------------------------
bool CCalculation::CrossCollision(
	D3DXVECTOR3 *ObjectPos,
	D3DXVECTOR3 *PosA,
	D3DXVECTOR3 *PosB,
	D3DXVECTOR3 *PosC
)
{
	// 変数宣言
	D3DXVECTOR3 VecCompOri, VecAhe;		// ベクトル方向
	D3DXVECTOR3 Cross1,Cross2,Cross3;	// 外積の計算結果
	// 一つ目
	VecCompOri = *PosB - *PosA;
	VecAhe = *ObjectPos - *PosA;
	D3DXVec3Cross(&Cross1,&VecCompOri, &VecAhe);
	// 二つ目
	VecCompOri = *PosC - *PosB;
	VecAhe = *ObjectPos - *PosB;
	D3DXVec3Cross(&Cross2, &VecCompOri, &VecAhe);
	// 三つ目
	VecCompOri = *PosA - *PosC;
	VecAhe = *ObjectPos - *PosC;
	D3DXVec3Cross(&Cross3, &VecCompOri, &VecAhe);
	// 範囲に入っている
	if (Cross1.y >= 0 && Cross2.y >= 0 && Cross3.y >= 0)
	{
		// デバッグ表示
		//CDebugproc::Print("外積[%.3f][%.3f][%.3f]\n", Cross1.y, Cross2.y, Cross3.y);
		return true;
	}
	// 入っていない
	return false;
}

// ----------------------------------------------------------------------------------------------------
// 行列から位置情報取得
// 1:位置情報,2:行列情報
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::Vector3ToMatrix(
	D3DXVECTOR3 &vector3Out,
	D3DXMATRIX const & mtx
)
{
	return vector3Out = D3DXVECTOR3(mtx._41,mtx._42,mtx._43);
}

// ----------------------------------------------------------------------------------------------------
// 行列から位置情報取得
// 1:行列情報
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCalculation::Vector3ToMatrix(
	D3DXMATRIX const & mtx
)
{
	return D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
}

// ----------------------------------------------------------------------------------------------------
// 桁数を測る(10で割る方法:数が少ない時に利用)
// ----------------------------------------------------------------------------------------------------
unsigned CCalculation::GetDigit_Division(unsigned nNum)
{
	// 変数宣言
	unsigned digit = 0;	// 桁数
	// nNumが0になるまでループ
	while (nNum != 0)
	{
		nNum /= 10;
		// 桁数アップ
		digit++;
	}
	// 桁数を返す
	return digit;
}

// ----------------------------------------------------------------------------------------------------
// 桁数を測る(常用対数方法:数が不特定で大きい時に利用)
// ----------------------------------------------------------------------------------------------------
unsigned CCalculation::GetDigit_CommonLogarithm(unsigned nNum)
{
	return (int)log10(nNum) + 1;
}

// ----------------------------------------------------------------------------------------------------
// ビルボードの設定
// ----------------------------------------------------------------------------------------------------
void CCalculation::SetBillboard(
	D3DXMATRIX *Originmtx,	// 元になる行列情報
	D3DXMATRIX *Viewmtx		// ビュー行列情報
)
{
	//逆行列
	Originmtx->_11 = Viewmtx->_11;
	Originmtx->_12 = Viewmtx->_21;
	Originmtx->_13 = Viewmtx->_31;
	Originmtx->_21 = Viewmtx->_12;
	Originmtx->_22 = Viewmtx->_22;
	Originmtx->_23 = Viewmtx->_32;
	Originmtx->_31 = Viewmtx->_13;
	Originmtx->_32 = Viewmtx->_23;
	Originmtx->_33 = Viewmtx->_33;

}

// ----------------------------------------------------------------------------------------------------
// メッセージ表示(警告文)
// ----------------------------------------------------------------------------------------------------
void CCalculation::Messanger(
	const char * cMessa,	// メッセージ内容
	HWND hWnd				// ウィンドウ情報
)
{
	// 変数宣言
	MessageBox(hWnd, cMessa, "警告！", MB_ICONWARNING);
}

// ----------------------------------------------------------------------------------------------------
// メッセージ表示(自由メッセージ)
// ----------------------------------------------------------------------------------------------------
void CCalculation::Messanger(
	const char * cMessa,	// メッセージ内容
	const long &ID,			// 番号
	HWND hWnd				// ウィンドウ情報
)
{
	// 変数宣言
	MessageBox(hWnd, cMessa, "メッセージ", ID);
}

// ----------------------------------------------------------------------------------------------------
// メッセージ表示(YES or NO)
// ----------------------------------------------------------------------------------------------------
bool const CCalculation::QuestionMessanger(
	const char * cMessa,	// メッセージ内容
	HWND hWnd				// ウィンドウ情報
)
{
	// 変数宣言
	int nYesOrNo;
	nYesOrNo = MessageBox(hWnd, cMessa, "どうしますか?", MB_YESNO | MB_ICONQUESTION);
	// Yesの場合
	// ->trueを返す
	if (nYesOrNo == IDYES) return true;
	// Noの場合
	// ->falseを返す
	else return false;
}

// ----------------------------------------------------------------------------------------------------
// キー文字の反映
// ----------------------------------------------------------------------------------------------------
void CCalculation::KeyString(
	int nKeyID,					// キーID
	char *KeyName				// 文字
)
{
	if (KeyName != NULL)
	{
		switch (nKeyID)
		{
		case DIK_0:
			strcpy(KeyName, "0");
			break;
		case DIK_1:
			strcpy(KeyName, "1");
			break;
		case DIK_2:
			strcpy(KeyName, "2");
			break;
		case DIK_3:
			strcpy(KeyName, "3");
			break;
		case DIK_4:
			strcpy(KeyName, "4");
			break;
		case DIK_5:
			strcpy(KeyName, "5");

			break;
		case DIK_6:
			strcpy(KeyName, "6");
			break;
		case DIK_7:
			strcpy(KeyName, "7");
			break;
		case DIK_8:
			strcpy(KeyName, "8");
			break;
		case DIK_9:
			strcpy(KeyName, "9");
			break;

		case DIK_Q:
			strcpy(KeyName, "Q");
			break;
		case DIK_W:
			strcpy(KeyName, "W");
			break;
		case DIK_E:
			strcpy(KeyName, "E");
			break;
		case DIK_R:
			strcpy(KeyName, "R");
			break;
		case DIK_T:
			strcpy(KeyName, "T");
			break;
		case DIK_Y:
			strcpy(KeyName, "Y");
			break;
		case DIK_U:
			strcpy(KeyName, "U");
			break;
		case DIK_I:
			strcpy(KeyName, "I");
			break;
		case DIK_O:
			strcpy(KeyName, "O");
			break;
		case DIK_P:
			strcpy(KeyName, "P");
			break;

		case DIK_A:
			strcpy(KeyName, "A");
			break;
		case DIK_S:
			strcpy(KeyName, "S");
			break;
		case DIK_D:
			strcpy(KeyName, "D");
			break;
		case DIK_F:
			strcpy(KeyName, "F");
			break;
		case DIK_G:
			strcpy(KeyName, "G");
			break;
		case DIK_H:
			strcpy(KeyName, "H");
			break;
		case DIK_J:
			strcpy(KeyName, "J");
			break;
		case DIK_K:
			strcpy(KeyName, "K");
			break;
		case DIK_L:
			strcpy(KeyName, "L");
			break;

		case DIK_Z:
			strcpy(KeyName, "Z");
			break;
		case DIK_X:
			strcpy(KeyName, "X");
			break;
		case DIK_C:
			strcpy(KeyName, "C");
			break;
		case DIK_V:
			strcpy(KeyName, "V");
			break;
		case DIK_B:
			strcpy(KeyName, "B");
			break;
		case DIK_N:
			strcpy(KeyName, "N");
			break;
		case DIK_M:
			strcpy(KeyName, "M");
			break;
		}
	}
}

// ------------------------------------------
// スクリプトが存在するか判定
// ------------------------------------------
bool CCalculation::Existenceofscript(FILE * pFile)
{
	// 変数宣言
	char cRead[128];	// 読み込み用
	char cComp[128];	// 比較用
	int nCntError = 0;	// エラー用
	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
			return false;
		}
	}
	return true;
}

// ------------------------------------------
// テクスチャーのサイズを取得(2D)
// ------------------------------------------
bool CCalculation::GetTexSize(
	LPDIRECT3DTEXTURE9 pTex,
	D3DXVECTOR2 * pSize
)
{
	// テクスチャーが設定されていなければ関数を抜ける
	if (pTex == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("ライブラリ:テクスチャー取得できませんでした。");
#endif // _DEBUG
		return false;
	}
	// テクスチャサイズの取得
	D3DSURFACE_DESC desc;
	if (pTex->GetLevelDesc(0, &desc) != S_OK)
	{
		return false;
	}
	// テクスチャーのサイズを取得
	pSize->x = (float)desc.Width;
	pSize->y = (float)desc.Height;

	return true;
}

// ------------------------------------------
// テクスチャーのサイズを取得(3D)
// ------------------------------------------
bool CCalculation::GetTexSize(
	LPDIRECT3DTEXTURE9 pTex,
	D3DXVECTOR3 * pSize
)
{
	// テクスチャサイズの取得
	D3DSURFACE_DESC desc;

	if (pTex->GetLevelDesc(0, &desc) != S_OK)
	{
		return false;
	}
	// テクスチャーのサイズを取得
	pSize->x = (float)desc.Width;
	pSize->y = (float)desc.Height;
	pSize->z = 0.0f;
	return true;
}

// ------------------------------------------
// 文字を特定の区切りごとに取得する(1行ごと)
// ------------------------------------------
vector<string> CCalculation::split(
	string& input,		// 1行のストリーム
	char delimiter		// 区切り文字
)
{
	// 変数宣言
	istringstream iss_Line(input);	// 文字列ストリーム
	string sRead;					// 文字列読み込み用
	vector<string> vec_Result;		// 1行読み込み用

	// 指定した区切りが来る限りループする
	while (getline(iss_Line,sRead,delimiter))
	{
		// 読み取った文字列を格納する
		vec_Result.push_back(sRead);
	}
	// 確保した文字列分返す
	return vec_Result;
}

// ------------------------------------------
// ファイルの中身を取得
// ------------------------------------------
vector<vector<string>> CCalculation::FileContens(
	char const * cns_cFile,
	char delimiter
)
{
	// 変数宣言
	ifstream				ifs_file;	// ファイル用ストリーム
	string					s_Line;		// 1時的に1行読み込む
	vector<vector<string>>	svec_Char;	// ファイルの中身格納用

	// ファイルを開く
	ifs_file.open(cns_cFile);
	// 読み込みに失敗したらエラー文を出し関数を抜ける
	if (ifs_file.fail())
	{
#ifdef _DEBUG
		Messanger("CCalculation::FileContensの関数のファイル読み取りエラー");
#endif // _DEBUG
		return svec_Char;
	}
	// 文字列がなくなるまでループ
	while (getline(ifs_file, s_Line))
	{
		// １行ごと文字列を取得する
		svec_Char.push_back(split(s_Line, delimiter));
	}
	// ファイルを閉じる
	ifs_file.close();

	// ファイルの中身を返す
	return svec_Char;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ファイルのメインコメント関数
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
string CCalculation::FileMainComment(
	string const & EditerName,	// エディター名
	string const & FileName,	// ファイル名
	string const & Author		// 著者名
)
{
	// 変数宣言
	string MainComment;	// メインコメント
	// メインコメント設定
	MainComment = "#==============================================================================\n";
	MainComment += "#\n";
	MainComment += "# [" + EditerName + "] スクリプトファイル [" + FileName + "]\n";
	MainComment += "# Author : " + Author + "\n";
	MainComment += "#\n";
	MainComment += "#==============================================================================\n";

	return MainComment;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ファイルのサブコメント関数
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
string CCalculation::FileSubComment(
	int const &nId,				// ID
	int const &nTab				// 水平タブの個数
)
{
	// 変数宣言
	string MainComment;			// メインコメント
	string TabString = "\0";	// 水平タブ
	// タブの個数分追加
	for (int nCntTab = 0; nCntTab < nTab; nCntTab++)
	{
		TabString += "\t";
	}
	// メインコメント設定
	MainComment = TabString + "#==============================================================================\n";
	MainComment += TabString + "# [" + to_string(nId) + "]\n";
	MainComment += TabString + "#==============================================================================\n";

	return MainComment;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ファイルのスモールコメント関数
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
string CCalculation::FileSmallComment(
	string const & SmallComment,	// スモールコメント名
	int const &nTab					// 水平タブの個数
)
{
	// 変数宣言
	string L_SmallComment = "\0";	// メインコメント
	// タブの個数分追加
	for (int nCntTab = 0; nCntTab < nTab; nCntTab++)
	{
		L_SmallComment += "\t";
	}
	// メインコメント設定
	L_SmallComment += "# ----------[" + SmallComment + "]---------- #\n";

	return L_SmallComment;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ファイルの通常コメント関数
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
string CCalculation::FileNormalComment(
	string const & NormalComment,
	int const & nTab
)
{
	// 変数宣言
	string L_NormalComment = "\0";	// メインコメント
	// タブの個数分追加
	for (int nCntTab = 0; nCntTab < nTab; nCntTab++)
	{
		L_NormalComment += "\t";
	}
	// メインコメント設定
	L_NormalComment += "# " + NormalComment + "\n";

	return L_NormalComment;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ファイルの名前情報の書き込み関数
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
string CCalculation::FileInfoWriting(
	char const *cFmt,				// フォーマット
	...
)
{
	// 変数宣言
	char cStr[200];
	va_list va;	// 可変変数
	// 可変引数を1個にまとめる
	va_start(va, cFmt);
	// まとめて表示
	vsprintf(&cStr[0], cFmt, va);
	va_end(va);
	return cStr;
}

void MYLIBLARY::in(void)
{

}
