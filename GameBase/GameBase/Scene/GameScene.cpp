#include "../SceneManager.h"
#include "TitleScene.h"
#include "../Common/Camera.h"
#include "../Object/ObjectManager.h"
#include "../UI/UiManager.h"
//#include "../Shader/PEManager.h"
//#include "../Factory/StageFactory.h"
#include "PauseScene.h"
#include "../Common/ResourceManager.h"
//#include "../Component/Transform/Transform.h"
//#include "Transition/FadeLoading.h"
//#include "../Common/SoundPross.h"
#include "ResultScene.h"
//#include "../Component/Behavior/ThrusterBehavior.h"
//#include "../Common/Input/InputConfig/InputConfig.h"
//#include "../Component/Behavior/CameraBehavior.h"
//#include "../Component/Behavior/StageBehavior.h"
#include "GameScene.h"

#include "../Common/Debug.h"

GameScene::GameScene(StageID stageID) :
	BaseScene{ScreenID::Game,SceneID::Game},stageID_{stageID}
{
	//SetMakeSceneFunc(std::bind(&GameScene::MakeResultFunc, true, std::placeholders::_1), SceneID::Result);
}

GameScene::~GameScene()
{
}

void GameScene::SetUp(void)
{
}

BaseScene::SceneUptr GameScene::MakeResultFunc(SceneUptr own)
{
	return SceneUptr();
}

BaseScene::SceneUptr GameScene::MakePauseFunc(SceneUptr own)
{
	return SceneUptr();
}

BaseScene::SceneUptr GameScene::MakeSelectFunc(SceneUptr own)
{
	return SceneUptr();
}

void GameScene::Update(float deltaa, Controller& controller)
{
}

void GameScene::DrawScene(void)
{
}

void GameScene::SetSubScreen(void)
{
}

bool GameScene::IsLoaded(void)
{
	return false;
}

void GameScene::Loaded(Controller& controller)
{
}

void GameScene::RaderSetUp(void)
{
}

void GameScene::RaderUpdate(Vector2 pPos)
{
}

void GameScene::RaderDrow(void)
{
}
