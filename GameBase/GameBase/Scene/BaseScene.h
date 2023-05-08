#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include <list>
#include "SceneID.h"
#include "../Component/ComponentID.h"
#include "../Common/Shared.h"
#include "../Common/ScreenID.h"
#include "../Common/Input/Controller.h"

class BaseScene;
class Camera;
class ObjectManager;
class UiManager;
class Controller;

struct LIGHT_MAT
{
	MATRIX view;
	MATRIX proj;
	
};

class BaseScene
{
public:
	using SceneUptr = std::unique_ptr<BaseScene>;
	using LoadedFunc = std::function<void(Controller&)>;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="id">シーン</param>
	/// <param name="sceneID">初期シーン</param>
	BaseScene(ScreenID id, SceneID sceneID);

	/// <summary>
	/// コンストラクタ(オブジェクトマネージャー引継ぎ用)
	/// </summary>
	/// <param name="objectManager">オブジェクトマネージャー</param>
	/// <param name="id">スクリーンの種類</param>
	/// <param name="sceneID">シーンの種類</param>
	BaseScene(std::unique_ptr<ObjectManager>&& objectManager, ScreenID id, SceneID sceneID);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseScene();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta">デルタタイム</param>
	/// <param name="controller"></param>
	virtual void Update(float delta, Controller& controller) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary>
	/// シーンの描画
	/// </summary>
	/// <param name=""></param>
	virtual void DrawScene(void) = 0;

	/// <summary>
	/// シーンIDの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual const SceneID GetID(void) const = 0;

	/// <summary>
	/// シーン内の開放処理
	/// </summary>
	/// <param name=""></param>
	virtual void Relese(void);

	/// <summary>
	/// ロード完了の確認
	/// </summary>
	/// <param name=""></param>
	/// <returns>trueの場合ロード完了</returns>
	virtual bool IsLoaded(void);

	/// <summary>
	/// シーンのポインタ更新処理
	/// </summary>
	/// <param name="own">現在のシーンのポインタ</param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	/// <returns>次のフレームで使うポインタ</returns>
	SceneUptr Update(SceneUptr own, float delta, Controller& controller);

	/// <summary>
	/// シーン遷移
	/// </summary>
	/// <param name="id">変更後のシーンID</param>
	void ChangeSceneID(SceneID id);

	/// <summary>
	/// シーン生成のためのfunctionセット用
	/// </summary>
	/// <param name="sceneMakeFunc">生成に必要なunction</param>
	/// <param name="id">生成するシーンID</param>
	void SetMakeSceneFunc(std::function<SceneUptr(SceneUptr)>&& sceneMakeFunc, const SceneID id);

	/// <summary>
	/// ロード処理が完了したときの処理
	/// </summary>
	/// <param name="controller"></param>
	void Loaded(Controller& controller);

	/// <summary>
	/// ロード完了時の追加処理
	/// </summary>
	/// <param name="loadFunc"></param>
	void AddLoadedFunc(LoadedFunc&& loadFunc);

	/// <summary>
	/// カメラの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カメラの参照</returns>
	Camera& GetCamera(void)
	{
		return *camera_;
	}

	/// <summary>
	/// オブジェクトマネージャーの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>オブジェクトマネージャーの参照</returns>
	ObjectManager& GetObjectManager(void)
	{
		return *objManager_;
	}

protected:

	// 移行先のID
	SceneID nextID_;

	// スクリーンのID
	ScreenID screenID_;

	// スクリーンハンドル
	SharedRenderTargetHandle screenHandle_;

	// カメラ
	std::unique_ptr<Camera> camera_;

	// UIマネージャー
	std::unique_ptr<UiManager> uiMneger_;

	// オブジェクトマネージャー
	std::unique_ptr<ObjectManager> objManager_;

private:
	
	// シーン作成のfunctionをまとめたマップ
	std::unordered_map<SceneID, std::function<SceneUptr(SceneUptr)>> seneMakeFuncMap_;

	// ロードが完了したときに実行するfunction
	std::list<LoadedFunc> loadedFunc_;
};


