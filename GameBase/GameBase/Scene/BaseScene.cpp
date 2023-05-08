#include <DxLib.h>
#include "BaseScene.h"
#include "../SceneManager.h"
#include "../Object/ObjectManager.h"
#include "../Common/Camera.h"
#include "../Common/ResourceManager.h"
#include "../UI/UiManager.h"
#include "../Common/Input/InputType/Keybord.h"
#include "../Common/Input/InputType/Pad.h"

#include "../Common/Debug.h"

BaseScene::BaseScene(ScreenID id, SceneID sceneID):
	screenID_{ id }, nextID_{ sceneID }
{
	// スクリーン作成
	lpSceneManager.GetResourceManager().MakeRenderTarget(screenHandle_, screenID_, lpSceneManager.screenSize_<float>, true);
	
	// カメラ作成
	camera_ = std::make_unique<Camera>();
}

BaseScene::BaseScene(std::unique_ptr<ObjectManager>&& objectManager, ScreenID id,SceneID sceneID):
	screenID_{ id }, nextID_{ sceneID }, objManager_{ std::move(objectManager) }
{
	// スクリーン作成
	lpSceneManager.GetResourceManager().MakeRenderTarget(screenHandle_, screenID_, lpSceneManager.screenSize_<float>, true);

	// カメラ作成
	camera_ = std::make_unique<Camera>();
}

BaseScene::~BaseScene()
{
}

void BaseScene::Draw(void)
{
	DrawGraph(0, 0, *screenHandle_, true);
}

void BaseScene::Relese(void)
{
}

bool BaseScene::IsLoaded(void)
{
	return GetASyncLoadNum() <= 0;
}

BaseScene::SceneUptr BaseScene::Update(SceneUptr own, float delta, Controller& controller)
{
	Update(delta, controller);
	if (nextID_ != GetID())
	{
		// 次のシーンのIDと一致しないとき
		if (seneMakeFuncMap_.contains(nextID_))
		{
			return seneMakeFuncMap_[nextID_](std::move(own));
		}
	}
	return own;
}

void BaseScene::ChangeSceneID(SceneID id)
{
	nextID_ = id;
}

void BaseScene::SetMakeSceneFunc(std::function<SceneUptr(SceneUptr)>&& sceneMakeFunc, const SceneID id)
{
	seneMakeFuncMap_.emplace(id, std::move(sceneMakeFunc));
}

void BaseScene::Loaded(Controller& controller)
{
	// ロード終了したのでリストの中身をすべて実行する
	std::for_each(loadedFunc_.begin(), loadedFunc_.end(), [&controller](LoadedFunc& f) { f(controller); });

	// もう使わないのでクリアしとく
	loadedFunc_.clear();
}

void BaseScene::AddLoadedFunc(LoadedFunc&& loadFunc)
{
	loadedFunc_.emplace_back(std::move(loadFunc));
}
