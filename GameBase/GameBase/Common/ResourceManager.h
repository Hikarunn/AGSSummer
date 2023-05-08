#pragma once
#include <filesystem>
#include <map>
#include <mutex>
#include <list>
#include <tuple>
#include "Shared.h"
#include "ScreenID.h"

class ResourceManager
{
	using HandleFlagPair = std::pair<std::shared_ptr<int>, bool>;
	using HandleMap = std::map<std::uint64_t, HandleFlagPair>;
	using HandleDivMap = std::map <std::uint64_t, std::tuple<std::vector<int>, std::shared_ptr<int>, bool>>;
	using ScreenMap = std::map<ScreenID, HandleFlagPair>;
	using ModelMap = std::map<std::uint64_t, std::tuple<std::shared_ptr<int>, bool>>;

public:
	ResourceManager();
	~ResourceManager();

	/// <summary>
	/// 画像んの読み込み
	/// </summary>
	/// <param name="out"></param>
	/// <param name=""></param>
	/// <param name="isNotRelese"></param>
	void LoadTexture(SharedGraphicHandle& out, const std::filesystem::path& path, 
		bool isNotRelese = false);
	
	/// <summary>
	/// 画像分割読み込み
	/// </summary>
	/// <param name="out"></param>
	/// <param name=""></param>
	/// <param name="divCount"></param>
	/// <param name="divSize"></param>
	/// <param name="isMotRelese"></param>
	void LoadDivTexture(SharedDivGraphicHandle& out, const std::filesystem::path& path, 
		const Vector2& divCount, const Vector2I& divSize, bool isNotRelese = false);

	/// <summary>
	/// モデル版
	/// </summary>
	/// <param name="out"></param>
	/// <param name=""></param>
	/// <param name="isNotRelese"></param>
	void LoadModelTexture(SharedGraphicHandle& out, const std::filesystem::path& path,
		bool isNotRelese = false);

	/// <summary>
	/// 一致するグラフィックハンドルを削除
	/// </summary>
	/// <param name="handle"></param>
	void Remove(SharedGraphicHandle& handle);

	/// <summary>
	/// 一致するグラフィックハンドルを削除
	/// </summary>
	/// <param name="handle"></param>
	void Remove(SharedDivGraphicHandle& handle);
	
	/// <summary>
	/// レンダーターゲットの作成
	/// </summary>
	/// <param name="out"></param>
	/// <param name="id"></param>
	/// <param name="size"></param>
	/// <param name="alpha"></param>
	/// <param name="isNotRelese"></param>
	void MakeRenderTarget(SharedRenderTargetHandle& out, const ScreenID id,
		const Vector2& size, bool alpha, bool isNotRelese = false);

	/// <summary>
	/// 削除
	/// </summary>
	/// <param name="id"></param>
	void Remove(const ScreenID id);

	/// <summary>
	/// モデルのロード
	/// </summary>
	/// <param name="out"></param>
	/// <param name=""></param>
	/// <param name="isNotRelese"></param>
	void LoadModel(SharedModelHandle& out, const std::filesystem::path& path, 
		bool isNotRelese = false);

	/// <summary>
	/// モデルのハンドル削除
	/// </summary>
	/// <param name="handle"></param>
	void RemoveModel(const int handle);

	/// <summary>
	/// ピクセルシェーダを読み込む
	/// </summary>
	/// <param name="path"></param>
	/// <param name="isNotRelese"></param>
	/// <returns></returns>
	void LoadPS(SharedShaderHandle& out, const std::filesystem::path& path, bool isNotRelese = false);

	/// <summary>
	/// 頂点シェーダを読み込む
	/// </summary>
	/// <param name="path"></param>
	/// <param name="isNotRelese"></param>
	/// <returns></returns>
	void LoadVS(SharedShaderHandle& out, const std::filesystem::path& path, bool isNotRelese = false);

	/// <summary>
	/// 一致するシェーダハンドルが管理から削除すべきか
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	bool IsRemove(SharedShaderHandle& handle);

	/// <summary>
	/// 一致するシェーダハンドルを管理から削除する
	/// </summary>
	/// <param name="handle"></param>
	void Remove(SharedShaderHandle& handle);


	/// <summary>
	/// サウンドのロード
	/// </summary>
	/// <param name="out"></param>
	/// <param name=""></param>
	/// <param name="isNotRelese"></param>
	void LoadSound(SharedSoundHandle&& out, const std::filesystem::path& path, bool isNotRelese);

	/// <summary>
	/// サウンドのハンドル削除
	/// </summary>
	/// <param name="handle"></param>
	void RemoveSound(const int handle);


	void Loaded(void);

private:
	std::mutex mutex_;


	HandleMap imageMap_;
	HandleDivMap imageDivMap_;
	ScreenMap screenMap_;
	HandleMap soundMap_;
	ModelMap modelMap_;
	HandleMap shaderMap_;

	std::list<std::function<void(void)>>loadedFunc_;
};

