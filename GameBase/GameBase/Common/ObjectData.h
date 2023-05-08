#pragma once
#include <cstdint>

// データの種類分け
enum class DataType
{
	Transform,		// 座標回転拡縮
	Collider,		// 当たり判定
	Model,			// モデル
	Spawn,			// スポーン座標
	Other			// その他
};

// データファイルのヘッダー
struct ObjectDataHeader
{
	char sig[4];			// シグネチャ
	unsigned int check;		// チェック
	unsigned int version;	// バージョン情報
};

// データのヘッダー
struct DataHeader
{
	DataType type;			// タイプ
	std::uint32_t size;		// データサイズ
};

// 当たり判定データ種類
enum class ColiderDataType
{
	Box,
	Capsule,
	Sphere,
	Mesh
};