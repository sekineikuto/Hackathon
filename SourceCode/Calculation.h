// ----------------------------------------------------------------------------------------------------
//
// Calculationヘッダー処理の説明[calculation.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _CALCULATION_H_
#define _CALCULATION_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include <fstream>
#include <sstream>

/*
#include <random>
#include <algorithm>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <limits>
#include <type_traits>

std::mt19937 create_rand_engine()
{
	std::random_device rnd;
	std::vector<std::uint_least32_t> v(10);// 初期化用ベクタ
	std::generate(v.begin(), v.end(), std::ref(rnd));// ベクタの初期化
	std::seed_seq seed(v.begin(), v.end());
	return std::mt19937(seed);// 乱数エンジン
}

namespace detail
{
	template<typename T> auto diff(T n1, T n2) -> typename std::make_unsigned<T>::type
	{
		static_assert(std::is_integral<T>::value, "T is not integral.");
		if (n1 < n2) std::swap(n1, n2);
		return static_cast<typename std::make_unsigned<T>::type>(n1 - n2);
	}
}

template<typename type>
std::vector<type> make_nonrepeat_rand_array_unique(const size_t size, type rand_min, type rand_max)
{
	if (rand_min > rand_max) std::swap(rand_min, rand_max);
	const auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
	if (max_min_diff < size) throw std::runtime_error("Invalid argument");

	std::vector<type> tmp;
	auto engine = create_rand_engine();
	std::uniform_int_distribution<type> distribution(rand_min, rand_max);

	const size_t make_size = size < std::numeric_limits<type>::max() - size / 5 ? size + size / 5 : std::numeric_limits<type>::max();

	tmp.reserve(make_size);
	while (tmp.size() < size) {
		while (tmp.size() < make_size) tmp.push_back(distribution(engine));
		std::sort(tmp.begin(), tmp.end());
		auto unique_end = std::unique(tmp.begin(), tmp.end());

		if (size < static_cast<size_t>(std::distance(tmp.begin(), unique_end))) {
			unique_end = std::next(tmp.begin(), size);
		}
		tmp.erase(unique_end, tmp.end());
	}

	std::shuffle(tmp.begin(), tmp.end(), engine);
	return tmp;
}

template<typename type>
std::vector<type> make_nonrepeat_rand_array_hash(const size_t size, type rand_min, type rand_max)
{
	if (rand_min > rand_max) std::swap(rand_min, rand_max);
	const auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
	if (max_min_diff < size) throw std::runtime_error("Invalid argument");

	std::unordered_set<type> tmp;
	auto engine = create_rand_engine();
	std::uniform_int_distribution<type> distribution(rand_min, rand_max);
	while (tmp.size() < size) tmp.insert(distribution(engine));
	return std::vector<type>(tmp.begin(), tmp.end());
}

template<typename type>
std::vector<type> make_nonrepeat_rand_array_unique(const size_t size, type rand_min, type rand_max)
{
	if (rand_min > rand_max) std::swap(rand_min, rand_max);
	const auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
	if (max_min_diff < size) throw std::runtime_error("Invalid argument");

	std::vector<type> tmp;
	auto engine = create_rand_engine();
	std::uniform_int_distribution<type> distribution(rand_min, rand_max);

	const size_t make_size = size < std::numeric_limits<type>::max() - size / 5 ? size + size / 5 : std::numeric_limits<type>::max();

	tmp.reserve(make_size);
	while (tmp.size() < size) {
		while (tmp.size() < make_size) tmp.push_back(distribution(engine));
		std::sort(tmp.begin(), tmp.end());
		auto unique_end = std::unique(tmp.begin(), tmp.end());

		if (size < static_cast<size_t>(std::distance(tmp.begin(), unique_end))) {
			unique_end = std::next(tmp.begin(), size);
		}
		tmp.erase(unique_end, tmp.end());
	}

	std::shuffle(tmp.begin(), tmp.end(), engine);
	return tmp;
}

template<typename type>
std::vector<type> make_nonrepeat_rand_array_shuffle(const size_t size, type rand_min, type rand_max) {
	if (rand_min > rand_max) std::swap(rand_min, rand_max);
	const auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
	if (max_min_diff < size) throw std::runtime_error("Invalid argument");

	std::vector<type> tmp;
	tmp.reserve(max_min_diff);

	for (auto i = rand_min; i <= rand_max; ++i)tmp.push_back(i);

	auto engine = create_rand_engine();
	std::shuffle(tmp.begin(), tmp.end(), engine);

	tmp.erase(std::next(tmp.begin(), size), tmp.end());

	return tmp;
}

template<typename type>
std::vector<type> make_nonrepeat_rand_array_select(const size_t size, type rand_min, type rand_max) {
	if (rand_min > rand_max) std::swap(rand_min, rand_max);
	const auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
	if (max_min_diff < size) throw std::runtime_error("Invalid argument");

	std::vector<type> tmp;
	tmp.reserve(max_min_diff);

	for (auto i = rand_min; i <= rand_max; ++i)tmp.push_back(i);

	auto engine = create_rand_engine();

	for (size_t cnt = 0; cnt < size; ++cnt) {
		size_t pos = std::uniform_int_distribution<size_t>(cnt, tmp.size() - 1)(engine);

		if (cnt != pos) std::swap(tmp[cnt], tmp[pos]);
	}
	tmp.erase(std::next(tmp.begin(), size), tmp.end());

	return tmp;
}

template<typename type>
std::vector<type> make_nonrepeat_rand_array_select_with_hash(const size_t size, type rand_min, type rand_max) {
	if (rand_min > rand_max) std::swap(rand_min, rand_max);
	auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
	//	if(max_min_diff < size) throw std::runtime_error("Invalid argument");
	using hash_map = std::unordered_map<type, type>;

	std::vector<type> tmp;
	tmp.reserve(size);

	hash_map Map;

	auto engine = create_rand_engine();
	for (size_t cnt = 0; cnt < size; ++cnt) {
		type val = std::uniform_int_distribution<type>(rand_min, rand_max)(engine);
		auto itr = Map.find(val);

		type replaced_val;
		auto replaced_itr = Map.find(rand_max);
		if (replaced_itr != Map.end()) replaced_val = replaced_itr->second;
		else replaced_val = rand_max;

		if (itr == Map.end()) {
			tmp.push_back(val);
			if (val != rand_max)Map.insert(std::make_pair(val, replaced_val));
		}
		else {
			tmp.push_back(itr->second);
			itr->second = replaced_val;
		}

		//Map.erase(replaced_val);

		--rand_max;
	}

	return tmp;
}

template<typename type>
std::vector<type> make_nonrepeat_rand_array_select_with_hash_no_itr(const size_t size, type rand_min, type rand_max) {
	if (rand_min > rand_max) std::swap(rand_min, rand_max);
	auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
	if (max_min_diff < size) throw std::runtime_error("引数が異常です");
	std::vector<type> re(size);
	std::unordered_map<type, type> conversion;

	auto engine = create_rand_engine();
	for (auto& r : re) {
		type key = std::uniform_int_distribution<type>(0, max_min_diff)(engine);
		if (!conversion.count(key)) conversion[key] = key;
		auto& conv_at_key = conversion[key];
		r = conv_at_key + rand_min;
		type conv_key = static_cast<type>(max_min_diff - 1);
		conv_at_key = (conversion.count(conv_key)) ? conversion[conv_key] : conv_key;
		--max_min_diff;
	}

	return re;
}

int* fn3_2_make_rand_array_select(const size_t size, int rand_min, int rand_max) {
	if (rand_min > rand_max) std::swap(rand_min, rand_max);
	const size_t max_min_diff = static_cast<size_t>(rand_max - rand_min + 1);
	if (max_min_diff < size) throw std::runtime_error("引数が異常です");

	int* tmp = (int*)malloc(sizeof(int) * max_min_diff);
	int* srcPtr = tmp;
	for (int i = rand_min; i <= rand_max; i++, srcPtr++)
	{
		*srcPtr = i;
	}

	auto engine = create_rand_engine();
	int distMax = max_min_diff - 1;
	//std::uniform_int_distribution<int> distribution(0, distMax); // こちらの方が適切(毎回、全数撹拌)

	// 先頭から戻り値を格納するループ
	int* dstPtr = tmp;
	for (size_t cnt = 0; cnt < size; cnt++, dstPtr++) {
		// sizeが0の場合は、0 > (max_min_diff - 1)の可能性がある (次行あたりでruntime_errorになるはず)
		std::uniform_int_distribution<int> distribution(cnt, distMax); // 元のロジック(未撹拌部分のみ操作)
		size_t pos = distribution(engine);
		if (cnt != pos) {
			// 上書きする前に退避
			int old = *dstPtr; // tmp[cnt]
			*dstPtr = tmp[pos]; // 戻り値を入れる
			tmp[pos] = old; // 使った値と使っていない値を入替る
		}
		// cntとposが同じ場合は、元の値のまま残す
	}

	// 先頭から長さsizeのデータを戻り値として使う
	// 使用後は、呼び出し側でfree(tmp)を呼ぶこと
	return tmp;
}

template<typename type>
std::vector<type> make_nonrepeat_rand_array_select2(const size_t size, type rand_min, type rand_max) {
	if (rand_min > rand_max) std::swap(rand_min, rand_max);
	const auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
	if (max_min_diff < size) throw std::runtime_error("Invalid argument");

	std::vector<type> tmp(max_min_diff);
	int i = 0;
	for (auto itr = std::begin(tmp); itr != std::end(tmp); ++itr) *itr = i++;

	auto engine = create_rand_engine();

	for (size_t cnt = 0; cnt < size; ++cnt) {
		size_t pos = std::uniform_int_distribution<size_t>(cnt, tmp.size() - 1)(engine);

		if (cnt != pos) std::swap(tmp[cnt], tmp[pos]);
	}
	tmp.erase(std::next(tmp.begin(), size), tmp.end());

	return tmp;

}

template<typename type>
std::vector<type> make_nonrepeat_rand_array(const size_t size, type rand_min, type rand_max) {
	if (rand_min > rand_max) std::swap(rand_min, rand_max);
	const auto max_min_diff = detail::diff(rand_max, rand_min) + 1;
	//	if (max_min_diff < size) throw std::runtime_error("Invalid argument");

	if (size < max_min_diff / 32) {
		return make_nonrepeat_rand_array_unique(size, rand_min, rand_max);
	}
	else {
		return make_nonrepeat_rand_array_select(size, rand_min, rand_max);
	}
}
*/

// ----------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------
// D3DVECTOR2型の初期化(1.0f)
#define D3DVECTOR2_INI			D3DXVECTOR2(1.0f,1.0f)
// D3DVECTOR2型の初期化(0.0f)
#define D3DVECTOR2_ZERO			D3DXVECTOR2(0.0f,0.0f)
// D3DVECTOR3型の初期化(0.0f)
#define D3DVECTOR3_ZERO			D3DXVECTOR3(0.0f,0.0f,0.0f)
// D3DVECTOR3型の初期化(1.0f)
#define D3DVECTOR3_ONE			D3DXVECTOR3(1.0f,1.0f,1.0f)
// D3DVECTOR4型の初期化(0.0f)
#define D3DVECTOR4_ZERO			D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f)
// D3DXCOLOR型の初期化
#define D3DXCOLOR_INI			D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)
// D3DXCOLOR型の初期化(基本色)
#define D3DXCOLOR_C(col)		D3DXCOLOR(col,col,col,1.0f)
// D3DXCOLOR型の初期化(基本色,透明度)
#define D3DXCOLOR_CA(col,alpha)	D3DXCOLOR(col,col,col,alpha)
// ファイル読み込み最大行数
#define FILELINE_ERROW (1048576)

// ----------------------------------------------------------------------------------------------------
//
// Calculation種類
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 列挙型定義
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 構造体定義
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 名前定義
//
// ----------------------------------------------------------------------------------------------------
namespace MYLIBLARY
{
	void in(void);
	// 書き換え不可の文字ポインター
	typedef const char * P_CST_CHAR;

}

// 配置オブジェクト情報(1:タイプ,2:位置,3:回転)
typedef struct ARRANGEMENTOBJ
{
	ARRANGEMENTOBJ() {}
	ARRANGEMENTOBJ(
		int d_nType,
		D3DXVECTOR3 d_pos,
		D3DXVECTOR3 d_rot
	)
	{
		nType = d_nType;
		pos = d_pos;
		rot = d_rot;
	}
	int			nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
} ARRANGEMENTOBJ,*pARRANGEMENTOBJ;

// 配置ポリゴン情報(1:タイプ,2:位置,3:回転,4:サイズA,5:サイズB)
typedef struct ARRANGEMENTPOLYGON
{
	ARRANGEMENTPOLYGON() {}
	ARRANGEMENTPOLYGON(
		int d_nType,
		D3DXVECTOR3 d_pos,
		D3DXVECTOR3 d_rot,
		float		d_fSizeA,
		float		d_fSizeB
	)
	{
		nType = d_nType;
		pos = d_pos;
		rot = d_rot;
		fSizeA = d_fSizeA;
		fSizeB = d_fSizeB;
	}
	int			nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	float		fSizeA;
	float		fSizeB;
} ARRANGEMENTPOLYGON, *pARRANGEMENTPOLYGON;

// 配置メッシュ情報(1:タイプ,2:位置,3:回転4:縦ブロック,5:横ブロック,6:サイズA,7:サイズB)
typedef struct ARRANGEMENTMESH
{
	ARRANGEMENTMESH() {}
	ARRANGEMENTMESH(
		int d_nType,
		D3DXVECTOR3 d_pos,
		D3DXVECTOR3 d_rot,
		int			d_nBlockDepth,
		int			d_nBlockWidth,
		float		d_fSizeA,
		float		d_fSizeB
	)
	{
		nType = d_nType;
		pos = d_pos;
		rot = d_rot;
		nBlockDepth = d_nBlockDepth;
		nBlockWidth = d_nBlockWidth;
		fSizeA = d_fSizeA;
		fSizeB = d_fSizeB;
	}
	int			nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int			nBlockDepth;
	int			nBlockWidth;
	float		fSizeA;
	float		fSizeB;
} ARRANGEMENTMESH, *pARRANGEMENTMESH;

// ファイル格納用変数
typedef struct CHARFILE
{
	char		cFile
		[64];		// モデルのファイル名
} CHARFILE;

// ----------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------
#define INTEGER4_INI			INTEGER4(1,1,1,1)
//#define ARRAYSIZE(Array)          ((int)(sizeof(Array) / sizeof(*Array)))       // Size of a static C-style array. Don't use on pointers!

// ----------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------
class CCalculation
{
public:
	/* 関数 */
	// 距離の計算
	static D3DXVECTOR3 Difference_Between(
		D3DXVECTOR3 &posA,
		D3DXVECTOR3 &posB
	);
	// 間の差の計算(3D用:位置、位置)
	static float DiffPoint(
		D3DXVECTOR3 const & posA,
		D3DXVECTOR3 const & posB
	);
	// 間の差の計算(2D用:位置、位置)
	static float DiffPoint(
		D3DXVECTOR2 const & posA,
		D3DXVECTOR2 const & posB
	);
	// 間の差の2乗計算(3D用:位置、位置)
	static float DiffPointSquare(
		D3DXVECTOR3 const & posA,
		D3DXVECTOR3 const & posB
	);
	// 間の差の2乗計算(2D用:位置、位置)
	static float DiffPointSquare(
		D3DXVECTOR2 const & posA,
		D3DXVECTOR2 const & posB
	);

	// 外積の計算
	static D3DXVECTOR3 Cross_product(
		D3DXVECTOR3 &VecA,
		D3DXVECTOR3 &VecB
	);
	// 内積の計算
	static float Dot_product(
		D3DXVECTOR3 &VecA,
		D3DXVECTOR3 &VecB
	);
	// 四角形の中に入っているかいないか
	static bool SquareBottom_Judg(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int			nBlock_W,
		int			nBlock_H,
		D3DXVECTOR3 worldpos
	);

	// 四角形の中に入っているかいないか
	static bool SquareSide_Judg(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int			nBlock_W,
		int			nBlock_H,
		D3DXVECTOR3 worldpos
	);

	// 球の足し算
	static float Addition_Sphere(
		float Length_x,
		float Length_y,
		float Length_z
	);
	// 円の当たり判定
	static bool Collision_Circle(
		D3DXVECTOR2 &Me,
		float		fMeSize,
		D3DXVECTOR2 &Opponent,
		float		fOppSize
	);
	// 球の当たり判定
	static bool Collision_Sphere(
		D3DXVECTOR3 const &Me,
		float const &fMeSize,
		D3DXVECTOR3 const &Opponent,
		float const &fOppSize
	);
	// 回転量の上限 //
	static float Rot_One_Limit(
		float &fRot
	);
	// スクリーン座標をワールド座標に変換
	static D3DXVECTOR3* CalcScreenToWorld(
		D3DXVECTOR3* pout,		// マウスポイント
		float Sx,				// スクリーンX座標
		float Sy,				// スクリーンY座標
		float fZ,				// 射影空間でのZ値（0～1）
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj	// プロジェクションマトリックス
	);
	// スクリーン座標をXZ平面のワールド座標交点算出
	static D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,		// マウスポイント
		float Sx,				// スクリーンX座標
		float Sy,				// スクリーンY座標
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj,		// プロジェクションマトリックス
		D3DXVECTOR3 obj			// オブジェクトの位置
	);
	// ワールド座標をスクリーン座標に変換(多分)
	static D3DXVECTOR3* CalcWorldToScreen(
		D3DXVECTOR3* Obj,		// オブジェクトの位置
		float Sx,					// スクリーンX座標
		float Sy,					// スクリーンY座標
		float fZ,				// 射影空間でのZ値（0～1）
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj	// プロジェクションマトリックス
	);
	// 一次関数
	static D3DXVECTOR2 Linear_function(
		D3DXVECTOR3 Apos,			// 始点
		D3DXVECTOR3 Bpos			// 終点
	);
	// 2線分の交点(連立方程式)
	static D3DXVECTOR3 TwoLine_Inse(
		D3DXVECTOR2 ALinear,	// 線①
		D3DXVECTOR2 BLinear	// 線②
	);
	// 途中の計算
	static bool TlyCollision(
		D3DXVECTOR3 ObjectPos,
		D3DXVECTOR3,
		...
	);
	// 外積の当たり判定
	static bool CrossCollision(
		D3DXVECTOR3 *ObjectPos,
		D3DXVECTOR3 *PosA,
		D3DXVECTOR3 *PosB,
		D3DXVECTOR3 *PosC
	);
	// 行列の位置情報取得
	// 1:位置情報,2:行列
	static D3DXVECTOR3 Vector3ToMatrix(
		D3DXVECTOR3 &vector3Out,
		D3DXMATRIX const &mtx
	);
	// 行列の位置情報取得
	// 1:行列
	static D3DXVECTOR3 Vector3ToMatrix(
		D3DXMATRIX const &mtx
	);
	// 桁数を測る(10で割る方法:数が少ない時に利用)
	static unsigned GetDigit_Division(unsigned nNum);
	// 桁数を測る(常用対数方法:数が不特定で大きい時に利用)
	static unsigned GetDigit_CommonLogarithm(unsigned nNum);
	// ビルボードの設定
	static void SetBillboard(
		D3DXMATRIX *Originmtx,	// 元になる行列情報
		D3DXMATRIX *Viewmtx		// ビュー行列情報
	);
	// メッセージ発生(警告)
	static void Messanger(
		const char * cMessa,	// メッセージ内容
		HWND hWnd = NULL
	);
	// メッセージ発生
	static void Messanger(
		const char * cMessa,	// メッセージ内容
		const long &ID,			// 番号
		HWND hWnd = NULL
	);
	// メッセージ発生(YES or NO)
	static bool const QuestionMessanger(
		const char * cMessa,
		HWND hWnd = NULL
	);
	// キー文字の反映
	static void KeyString(
		int nKeyID,				// キーID
		char *KeyName = NULL	// 文字
	);
	// ファイルが存在しているか
	static bool Existenceofscript(FILE * pFile);
	// テクスチャー情報からテクスチャーサイズの取得(2D)
	static bool GetTexSize(
		LPDIRECT3DTEXTURE9 pTex,
		D3DXVECTOR2 *pSize
	);
	// テクスチャー情報からテクスチャーサイズの取得(3D)
	static bool GetTexSize(
		LPDIRECT3DTEXTURE9 pTex,
		D3DXVECTOR3 * pSize
	);
	// 文字を特定の区切りごとに取得する(1行ごと)
	static vector<string> split(
		string& input,		// 1行のストリーム
		char delimiter		// 区切り文字
	);
	// 文字を特定の区切りごとに取得する(ファイル全体)
	static vector<vector<string>> FileContens(
		char const * cns_cFile,	// ファイル名
		char delimiter			// 区切り文字
	);
	// ファイルメインコメントの書き込み
	static string FileMainComment(
		string const & EditerName,	// エディター名
		string const & FileName,	// ファイル名
		string const & Author		// 著者名
	);
	// サブコメントの書き込み
	static string FileSubComment(
		int const &nId,				// ID
		int const &nTab				// 水平タブの個数
	);
	// スモールコメントの書き込み
	static string FileSmallComment(
		string const & SmallComment,	// スモールコメント名
		int const &nTab					// 水平タブの個数
	);
	// 通常コメントの書き込み
	static string FileNormalComment(
		string const & NormalComment,	// 通常コメント名
		int const &nTab					// 水平タブの個数
	);
	// float型の情報書き込み
	static string FileInfoWriting(
		char const *cFmt,				// フォーマット
		...
	);
protected:

private:
	/* 変数 */
	static bool m_bWire;
};

// ----------------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ----------------------------------------------------------------------------------------------------

#endif