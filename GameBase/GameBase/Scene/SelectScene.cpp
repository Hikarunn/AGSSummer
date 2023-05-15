#include "SelectScene.h"
#include "Transition/Loading.h"
#include "GameScene.h"
#include "OptionScene.h"
#include "../UI/UiManager.h"
#include "Transition/FadeLoading.h"
#include "DialogScene.h"
#include "../Common/ResourceManager.h"
//#include "../Common/SoundPross.h"
#include "../Object/ObjectManager.h"
#include "../Component/Render/ModelRender.h"
#include "../Component/Transform/Transform.h"
#include "../Component/Info/ObjectInfo.h"
#include "../Common/Camera.h"

#ifdef _DEBUG
#include "TitleScene.h"
#endif

SelectScene::SelectScene()
	: WindowScene{ nullptr, 0.25f , ScreenID::SelectUi,ScreenID::Select,SceneID::Select }
{
	Init();
	objManager_ = std::make_unique<ObjectManager>(1ull);

	auto id = objManager_->MakeObjectID();

	std::unique_ptr<Render> render = std::make_unique<ModelRender>();
	render->Load("Resource/resource/Title/Title.mv1");
	objManager_->AddComponent(std::move(render), id);

	auto transform = std::make_unique<Transform>();
	transform->Scale() = Vector3{ 1.0f,1.0f,1.0f };
	objManager_->AddComponent(std::move(transform), id);

	objManager_->AddComponent(std::make_unique<ObjectInfo>(), id);

	AddLoadedFunc(std::bind(&SelectScene::Loaded, this, std::placeholders::_1));

	//camera_->SetPos(Vector3(0.0f, 50.0f, -300.0f));
	//camera_->SetRotation(Quaternion(0.0f, Deg2Rad(180.0f), 0.0f));
	//SetLightDirection(VGet(0.0f, -1.0f, 0.0f));
	//SetLightDifColor(GetColorF(1.f, 1.f, 1.f, 1.f));
	//SetLightSpcColor(GetColorF(1.f, 1.f, 1.f, 1.f));
	//SetLightAmbColor(GetColorF(0.5f, 0.5f, 0.5f, 1.f));

	//// シャドウマップ
	//int x, y;
	//GetDrawScreenSize(&x, &y);
	//shadowMap_ = MakeScreen(x, y, false);
	//// シャドウマップ用カメラ情報の初期化
	//SetUseASyncLoadFlag(false);
	//shadowBuff_ = CreateShaderConstantBuffer(sizeof(LIGHT_MAT) * 4);
	//SetUseASyncLoadFlag(true);
	//lightMat_ = static_cast<LIGHT_MAT*>(GetBufferShaderConstantBuffer(shadowBuff_));
	//lightMat.view = MGetIdent();
	//lightMat.proj = MGetIdent();

}

SelectScene::SelectScene(std::unique_ptr<ObjectManager>&& objectManager) :
	WindowScene{ nullptr, 0.25f , ScreenID::SelectUi,ScreenID::Select,SceneID::Select }
{
	objManager_ = std::move(objectManager);
	Init();
}

void SelectScene::Dialog(void)
{
	choiceNextID_ = SceneID::Dialog;
	//lpSooundPross.PlayBackSound(SOUNDNAME_SE::cursorMove, lpSooundPross.GetVolume(), false);
	Close();
}

void SelectScene::Option(void)
{
	choiceNextID_ = SceneID::Option;
	//lpSooundPross.PlayBackSound(SOUNDNAME_SE::cursorMove, lpSooundPross.GetVolume(), false);
	Close();
}

void SelectScene::Play(void)
{
	choiceNextID_ = SceneID::Game;
	//lpSooundPross.PlayBackSound(SOUNDNAME_SE::cursorMove, lpSooundPross.GetVolume(), false);
	Close();
}

void SelectScene::Tutorial(void)
{
	staegID_ = StageID::Tutorial;
	choiceNextID_ = SceneID::Game;
	//lpSooundPross.PlayBackSound(SOUNDNAME_SE::cursorMove, lpSooundPross.GetVolume(), false);
	Close();
}

BaseScene::SceneUptr SelectScene::MakeGameFunc(SceneUptr own)
{
	return std::make_unique<FadeLoading>(std::move(own), std::make_unique<GameScene>(staegID_), 2.0f);
}

BaseScene::SceneUptr SelectScene::MakeOptionFunc(SceneUptr own)
{
	return std::make_unique<OptionScene>(std::move(own));
}

BaseScene::SceneUptr SelectScene::MakeDialogFunc(SceneUptr own)
{
	return std::make_unique<DialogScene>(std::move(own));
}

#ifdef _DEBUG

BaseScene::SceneUptr SelectScene::MakeTitleFunc(SceneUptr own)
{
	return std::make_unique<FadeLoading>(std::move(own), std::make_unique<TitleScene>(), 1.0f);
}

#endif

void SelectScene::UpdateOpend(float delta, Controller& controller)
{
	uiManager_->Update(delta, *this, *objManager_, controller);

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_1))
	{
		ChangeSceneID(SceneID::Title);
	}
#endif
}

void SelectScene::DrawWindow(void)
{
	DrawRotaGraph(SceneManager::screenSize_<int>.x / 2, SceneManager::screenSize_<int>.y / 2, 1.0, 0.0f, *frame_, true);
	uiManager_->Draw();
}

void SelectScene::Closed(void)
{
	WindowScene::Closed();
	DrawScene();
	Open();
	ChangeSceneID(choiceNextID_);

	//lpSooundPross.Release(SOUNDNAME_BGM::TitleSceneBGM);
}

bool SelectScene::IsLoaded(void)
{
	return BaseScene::IsLoaded() && uiManager_->IsLoaded();
}

void SelectScene::Init(void)
{
	SetMakeSceneFunc(std::bind(&SelectScene::MakeGameFunc, this, std::placeholders::_1), SceneID::Game);
	SetMakeSceneFunc(std::bind(&SelectScene::MakeDialogFunc, this, std::placeholders::_1), SceneID::Dialog);
	SetMakeSceneFunc(std::bind(&SelectScene::MakeOptionFunc, this, std::placeholders::_1), SceneID::Option);

#ifdef _DEBUG
	SetMakeSceneFunc(std::bind(&SelectScene::MakeTitleFunc, this, std::placeholders::_1), SceneID::Title);
#endif

	uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/select.ui");
	AddLoadedFunc([this](auto&) { uiManager_->Begin(); });
	lpSceneManager.GetResourceManager().LoadTexture(frame_, "Resource/resource/SelectFrame.png");
	staegID_ = StageID::Stage1;
	//lpSooundPross.Init(SceneID::Select);
	//lpSooundPross.PlayBackSound(SOUNDNAME_SE::openMenu, lpSooundPross.GetVolume(), false);
	camera_->SetRotation(Quaternion(0.0f, Deg2Rad(180.0f), 0.0f));
	camera_->SetPos(Vector3(0.0f, 200.0f, 200.0f));
	SetLightDirection(VGet(0.0f, -1.0f, 0.0f));
	SetLightDifColor(GetColorF(1.f, 1.f, 1.f, 1.f));
	SetLightSpcColor(GetColorF(1.f, 1.f, 1.f, 1.f));
	SetLightAmbColor(GetColorF(0.5f, 0.5f, 0.5f, 1.f));

	// シャドウマップ
	int x, y;
	GetDrawScreenSize(&x, &y);
	shadowMap_ = MakeScreen(x, y, false);
	// シャドウマップ用カメラ情報の初期化
	SetUseASyncLoadFlag(false);
	shadowBuff_ = CreateShaderConstantBuffer(sizeof(LIGHT_MAT) * 4);
	SetUseASyncLoadFlag(true);
	lightMat_ = static_cast<LIGHT_MAT*>(GetBufferShaderConstantBuffer(shadowBuff_));
	lightMat.view = MGetIdent();
	lightMat.proj = MGetIdent();

}

void SelectScene::DrawBackGround(void)
{
	camera_->SetScreen();
	objManager_->ShadowDraw(shadowMap_, shadowBuff_);
}

void SelectScene::Loaded(Controller& controller)
{
	lpSceneManager.GetResourceManager().Loaded();
	objManager_->Begin();
	objManager_->Update(0.0f, controller, *this);
}

void SelectScene::SetupShadowMap(void)
{
	// 描画先を影用深度記録画像に変更する
	SetDrawScreen(shadowMap_);
	// 影用深度記録画像を一度真っ白にする
	SetBackgroundColor(255, 255, 255);
	ClsDrawScreen();
	SetBackgroundColor(0, 0, 0);

	// シャドウマップ用にカメラをセット
	camera_->SetUpShadow(offsetOrtho, offsetNear, offsetFar, camTar);

	MV1SetUseOrigShader(true);
	objManager_->SetupDepthTex(*shadowPs_, -1);
	MV1SetUseOrigShader(false);

	lightMat.view = GetCameraViewMatrix();
	lightMat.proj = GetCameraProjectionMatrix();
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	lightMat_[0] = lightMat;

}

