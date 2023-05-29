#include "../SceneManager.h"
#include "TitleScene.h"
#include "../Common/Camera.h"
#include "../Object/ObjectManager.h"
#include "../UI/UiManager.h"
#include "../Factory/StageFactory.h"
#include "PauseScene.h"
#include "../Common/ResourceManager.h"
#include "../Component/Transform/Transform.h"
#include "Transition/FadeLoading.h"
#include "ResultScene.h"
#include "../Common/Input/InputConfig/InputConfig.h"
#include "../Component/Behavior/CameraBehavior.h"
#include "../Component/Behavior/StageBehavior.h"
#include "GameScene.h"
#include "../Component/Render/Render.h"

#include "../Common/Debug.h"

GameScene::GameScene(StageID stageID) :
	BaseScene{ ScreenID::Game,SceneID::Game }, stageID_{ stageID }
{
	SetMakeSceneFunc(std::bind(&GameScene::MakeResultFunc, this, std::placeholders::_1), SceneID::Result);
	SetMakeSceneFunc(std::bind(&GameScene::MakePauseFunc, this, std::placeholders::_1), SceneID::Pause);
	objManager_ = std::make_unique<ObjectManager>(10);
//	peMng_ = std::make_unique<PEManager>();
	if (stageID_ == StageID::Tutorial)
	{
		// チュートリアル時
		uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/game.ui", true, true, false);
		objManager_->AddFactory(std::make_unique<StageFactory>(*objManager_));
		SetMakeSceneFunc(std::bind(&GameScene::MakeSelectFunc, this, std::placeholders::_1), SceneID::Select);
	}
	else
	{
		// 通常時のゲーム時
		objManager_->AddFactory(std::make_unique<StageFactory>("Resource/Other/Stage" + std::to_string(static_cast<int>(stageID)) + ".data", *objManager_));
		//objMng_->AddFactory(std::make_unique<StageFactory>("Resource/Other/dbg.data", *objMng_));
		uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/game.ui", true, false, false);
//		lpSceneManager.GetResourceManager().MakeRenderTarget(resultCapture_, ScreenID::ResultCapture, SceneManager::screenSize_<float>, true);
	}
	AddLoadedFunc(std::bind(&GameScene::Loaded, this, std::placeholders::_1));
	result_ = Result::Max;

	// ゲームシーンで使うシェーダをあらかじめロードしておく
	//useShaders_.resize(3);
	/*lpSceneManager.GetResourceManager().LoadVS(useShaders_[0], "Resource/resource/Shader/Vertex/Mesh.vso");
	lpSceneManager.GetResourceManager().LoadVS(useShaders_[1], "Resource/resource/Shader/Vertex/Mesh4.vso");
	lpSceneManager.GetResourceManager().LoadPS(useShaders_[2], "Resource/resource/Shader/Pixel/Tex.pso");*/



	SetLightDirection(VGet(1.f, -1.f, 1.f));
	SetLightDifColor(GetColorF(1.f, 1.f, 1.f, 1.f));
	SetLightSpcColor(GetColorF(1.f, 1.f, 1.f, 1.f));
	SetLightAmbColor(GetColorF(0.5f, 0.5f, 0.5f, 1.f));

	int x, y;
	GetDrawScreenSize(&x, &y);
	// シャドウマップを作成する際に高いフォーマット設定はいらないので設定を下げる
	// 次に作成する画像のフォーマットの設定をする
	SetDrawValidFloatTypeGraphCreateFlag(true);
	// チャネルを１にする
	SetCreateDrawValidGraphChannelNum(1);
	// ビット数を16にする
	SetCreateGraphColorBitDepth(16);
	// シャドウマップ
	shadowMap_ = MakeScreen(x, y, false);

	// 元に戻す
	SetDrawValidFloatTypeGraphCreateFlag(false);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphColorBitDepth(32);
	// 被写界深度
	depth_ = MakeScreen(x, y, false);

	// オフスクリーンの初期化
	offScreen_ = MakeScreen(x, y, false);
	// スカイドームのスクリーン
	skyScreen_ = MakeScreen(x, y, false);
	// スカイドームとステージのスクリーン
	subScreen_ = MakeScreen(x, y, false);

	radarGraph_ = LoadGraph(L"Resource/resource/Radar.png");

	// ミニマップ用のスクリーン
	radarSize_ = y / 4;
	radarMap_ = MakeScreen(radarSize_, radarSize_, false);
	count_ = 0;

	// シャドウマップ用カメラ情報の初期化
	SetUseASyncLoadFlag(false);
	shadowBuff_ = CreateShaderConstantBuffer(sizeof(LIGHT_MAT) * 4);
	lightMat_ = static_cast<LIGHT_MAT*>(GetBufferShaderConstantBuffer(shadowBuff_));
	lightMat.view = MGetIdent();
	lightMat.proj = MGetIdent();

	// 被写界深度用のカメラ情報の初期化
	depthbuffer_ = CreateShaderConstantBuffer(sizeof(DepthParameter) * 4);
	depthMat_ = static_cast<DepthParameter*>(GetBufferShaderConstantBuffer(depthbuffer_));
	depthMat.start = 0.0f;
	depthMat.end = 0.0f;
	depthMat.scope = 0.0f;
	SetUseASyncLoadFlag(true);

	// ゲームシーン用のBGMのロード
	//lpSooundPross.Init(SceneID::Game);

#ifdef _DEBUG
	SetMakeSceneFunc(std::bind(&GameScene::MakeSelectFunc, this, std::placeholders::_1), SceneID::Select);
#endif
}

GameScene::~GameScene()
{
	DeleteShaderConstantBuffer(shadowBuff_);
	DeleteShaderConstantBuffer(depthbuffer_);
	radar_.erase(count_);
}

//void GameScene::Capture(void)
//{
//	// 一度Ui以外を描画する
//	SetupShadowMap();
//	SetOffsetScreen();
//	SetSubScreen();
//	SetUpDepth();
//	depthMat_[0] = depthMat;
//	peMng_->SetBuffer(depthbuffer_);
//	peMng_->Draw(offScreen_, *screenHandle_, depth_, skyScreen_, subScreen_);
//
//	// 描画結果をキャプチャする
//	SetDrawScreen(*screenHandle_);
//	GetDrawScreenGraph(0, 0, SceneManager::screenSize_<int>.x, SceneManager::screenSize_<int>.y, *resultCapture_);
//}

void GameScene::SetUp(void)
{
	//peMng_->SetFlag(PEID::Mono, lpConfigMng.GetPeConfig().at(PEID::Mono));
	//peMng_->SetFlag(PEID::VolFog, lpConfigMng.GetPeConfig().at(PEID::VolFog));
	//lpSooundPross.PlayBackSound(SOUNDNAME_BGM::GameSceneBGM, lpSooundPross.GetVolume(), true);	// ゲームシーンのBGM
	//lpSooundPross.PlayBackSound(SOUNDNAME_BGM::GameSceneBGM, lpSooundPross.GetVolume(), true);	// ゲームシーンのBGM

	auto cam = objManager_->GetComponent<CameraBehavior>(objManager_->GetCameraID());
	if (cam.IsActive())
	{
		cam->SetSpeed(lpConfigManager.GetCameraSpeed());
	}
}

BaseScene::SceneUptr GameScene::MakeResultFunc(SceneUptr own)
{
	//return;
//	Capture();
	//lpSooundPross.SoundStop(SOUNDNAME_BGM::GameSceneBGM);
//	lpSooundPross.SoundStop(SOUNDNAME_SE::playerMove);
	return std::make_unique<FadeLoading>(std::move(own), std::make_unique<ResultScene>(stageID_, result_, resultCapture_), 1.0f);
}

BaseScene::SceneUptr GameScene::MakePauseFunc(SceneUptr own)
{
	//return;
	//lpSooundPross.SoundStop(SOUNDNAME_BGM::GameSceneBGM);
	//lpSooundPross.SoundStop(SOUNDNAME_SE::playerMove);
	return std::make_unique<PauseScene>(std::move(own));
}

BaseScene::SceneUptr GameScene::MakeSelectFunc(SceneUptr own)
{
	return std::make_unique<FadeLoading>(std::move(own), std::make_unique<TitleScene>(), 0.5f);
	//lpSooundPross.SoundStop(SOUNDNAME_BGM::GameSceneBGM);
	//lpSooundPross.SoundStop(SOUNDNAME_SE::playerMove);
	//return std::make_unique<FadeLoading>(std::move(own), std::make_unique<TitleScene>(), 0.5f);
}

void GameScene::Update(float delta, Controller& controller)
{
	if (controller.PressdCancel())
	{
		ChangeSceneID(SceneID::Pause);
		return;
	}

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_1))
	{
		ChangeSceneID(SceneID::Select);
		return;
	}
	if (CheckHitKey(KEY_INPUT_2))
	{
		SetResult(Result::Clear);
		ChangeSceneID(SceneID::Result);
		return;
	}
	if (CheckHitKey(KEY_INPUT_3))
	{
		/*auto id = objMng_->GetFactory(FactoryID::HorizontalEffect).Create(ObjectID{}, {0.0f, 100.0f, 0.0f});
		objMng_->Begin(id);*/
	}
#endif

	auto player = (objManager_->GetComponent<Transform>(objManager_->GetPlayerID()));
	objManager_->Update(delta, controller, *this);
	uiManager_->Update(delta, *this, *objManager_, controller);
	if (player.IsActive())
	{
		RadarUpdate(Vector2(player->GetPos().x, player->GetPos().z));
	}
	//peManager_->Update(delta);

}

void GameScene::DrawScene(void)
{
	RadarDraw();
	// シャドウマップ作成
	SetupShadowMap();
	// オフスクリーンの作成
	SetOffsetScreen();
	// メイン以外のスクリーンの作成
	SetSubScreen();

	// 被写界深度用の深度テクスチャの作成
	SetUpDepth();

	depthMat_[0] = depthMat;
	//peMng_->SetBuffer(depthbuffer_);
	// ポストエフェクトか通常描画
	//peMng_->Draw(offScreen_, *screenHandle_, depth_, skyScreen_, subScreen_);

	// ミニマップの作成
	DrawGraph(10, 10, radarMap_, true);

	// UIの描画
	uiManager_->Draw();

}

void GameScene::SetupShadowMap(void)
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

	// シャドウマップ作成に使ったカメラ情報を取得
	lightMat.view = GetCameraViewMatrix();
	lightMat.proj = GetCameraProjectionMatrix();
	// 描画用に切り替え
	SetDrawScreen(offScreen_);
	ClsDrawScreen();
	// さっき取得したカメラ情報をhlsl側に渡す
	lightMat_[0] = lightMat;
}

void GameScene::SetUpDepth(void)
{
	// 描画先を影用深度記録画像に変更
	SetDrawScreen(depth_);
	// 影用深度記録画像を一度真っ白にする
	SetBackgroundColor(255, 255, 255);
	ClsDrawScreen();
	SetBackgroundColor(0, 0, 0);
	camera_->SetScreen();

	// 被写界深度開始位置の計算
	depthMat.start = dofFocus - dofFocusSize / 2.0f - dofInterpSize;
	// 被写界深度終了位置を計算
	depthMat.end = dofFocus + dofFocusSize / 2.0f + dofInterpSize;
	// 被写界深度の範囲の逆数を計算
	depthMat.scope = 1.0f / (depthMat.end - depthMat.start);
	// 補間範囲とフォーカスがあっている範囲を含めた総距離を算出
	dofTotalSize_ = dofInterpSize * 2.0f + dofFocusSize;

	// 取得したデータをhlsl側に渡す
	depthMat_[0] = depthMat;
	MV1SetUseOrigShader(true);
	objManager_->SetupDepthTex(*depthPS_, depthbuffer_);
	MV1SetUseOrigShader(false);

	// 描画用に切り替え
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	// 被写界深度開始位置の計算
	depthMat.start = dofInterpSize / dofTotalSize_;
	// 被写界深度終了位置の計算
	depthMat.end = (dofInterpSize + dofFocusSize) / dofTotalSize_;

}

void GameScene::SetOffsetScreen(void)
{
	camera_->SetScreen();

	// 描画
	objManager_->ShadowDraw(shadowMap_, shadowBuff_);
	MV1SetUseOrigShader(false);

	// 3D系のデバッグ描画
	DebugDraw3DScreen();

	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
}

void GameScene::SetSubScreen(void)
{
	// スカイドームのみのスクリーン
	SetDrawScreen(skyScreen_);
	ClsDrawScreen();
	camera_->SetScreen();
	auto [result, id] = objManager_->Find(ObjectAttribute::Sky);
	objManager_->GetComponent<Render>(id)->Draw();

	// スカイドームとステージのみのスクリーン
	SetDrawScreen(subScreen_);
	ClsDrawScreen();
	camera_->SetScreen();
	auto [result1, id1] = objManager_->Find(ObjectAttribute::Stage);
	objManager_->GetComponent<Render>(id)->Draw();
	objManager_->GetComponent<Render>(id1)->Draw();
}

bool GameScene::IsLoaded(void)
{
	return BaseScene::IsLoaded() && objManager_->IsLoaded() && uiManager_->IsLoaded();
}

void GameScene::Loaded(Controller& controller)
{
	// ロード完了時に呼ばれる
	lpSceneManager.GetResourceManager().Loaded();
	objManager_->Begin();
	objManager_->Update(0.0f, controller, *this);
	uiManager_->Begin();


	lpSceneManager.GetResourceManager().LoadPS(shadowPs_, "Resource/resource/Shader/ShadowMap/ShadowMap.pso");
	lpSceneManager.GetResourceManager().LoadPS(depthPS_, "Resource/resource/Shader/PostEffect/Dof/depth.pso");
	SetUp();
	RadarSetUp();
}

void GameScene::RadarSetUp(void)
{
	// プレイヤーの情報を取得
	auto player = objManager_->GetComponent<Transform>(objManager_->GetPlayerID());
	Vector2 pPos = Vector2(player->GetPos().x, player->GetPos().z);
	// エネミーの情報を取得
	auto stage = objManager_->GetComponent<StageBehavior>(objManager_->GetStageID());
	auto& list = stage->GetEnemyPosList();
	// プレイヤーとの距離と角度を取得
	int i = 0;
	for (auto ePos : list)
	{
		Radar radar;
		// プレイヤーとの距離を取得
		Vector2 tmp = ePos.second - pPos;
		radar.range = tmp.Magnitude();
		// プレイヤーとの角度を取得
		// カメラの情報を取得
		auto camera = objManager_->GetComponent<Transform>(objManager_->GetCameraID());
		// カメラの向いている方向からの角度を求める
		float angle = GetAngle2Vector(Vector2(camera->GetForward().x, camera->GetForward().z), tmp);
		// 出たものを格納
		radar.angle = angle;

		// デフォルトは表示しない
		radar.flag = false;
		// 距離が表示可能範囲内であれば表示する
		if (radar.range <= (RADAR_RANGE * RADAR_RANGE))
		{
			radar.flag = true;
		}
		radar_.emplace(i, radar);
		i++;
		count_++;
	}
}

void GameScene::RadarUpdate(Vector2 pPos)
{
	// プレイヤーの情報を取得
	auto player = objManager_->GetComponent<Transform>(objManager_->GetPlayerID());
	// エネミーの情報を取得
	auto stage = objManager_->GetComponent<StageBehavior>(objManager_->GetStageID());
	auto& list = stage->GetEnemyPosList();
	// プレイヤーとの距離と角度を取得
	int i = 0;
	for (auto ePos : list)
	{
		// プレイヤーとの距離を取得
		Vector2 tmp = ePos.second - pPos;
		float range = tmp.Magnitude();
		// カメラの情報を取得
		auto camera = objManager_->GetComponent<Transform>(objManager_->GetCameraID());
		// カメラの向いている方向からの角度を求める
		float angle = GetAngle2Vector(Vector2(camera->GetForward().x, camera->GetForward().z), tmp);
		// デフォルトは表示しない
		bool flag = false;
		if (range <= (RADAR_RANGE * RADAR_RANGE))
		{
			flag = true;
		}
		radar_.at(i).range = range;
		radar_.at(i).angle = angle;
		radar_.at(i).flag = flag;
		i++;
	}
	for (int n = i; n < count_; n++)
	{
		radar_.at(n).flag = false;
	}
}

void GameScene::RadarDraw()
{
	SetDrawScreen(radarMap_);
	ClsDrawScreen();
	DrawGraph(0, 0, radarGraph_, true);

	// 中心位置の取得
	int SizeHalf = radarSize_ / 2;
	// プレイヤーの位置
	DrawCircle(SizeHalf, SizeHalf, 5, 0x0000ff);

	for (const auto& map : radar_)
	{
		if (map.second.flag)
		{
			Vector3 pos = Vector3(0, -1, 0);
			Vector3 tmp;
			// 受け取った角度でsinとcosの値をだす
			float sinParam = sin(-map.second.angle);
			float cosParam = cos(-map.second.angle);
			// sinParamとcosParamで敵の位置を取得する
			tmp.x = (pos.x * cosParam) - (pos.y * sinParam);
			tmp.y = (pos.x * sinParam) + (pos.y * cosParam);
			tmp.z = 0.0f;
			pos.x = tmp.x * (map.second.range / 50);
			pos.y = tmp.y * (map.second.range / 50);
			pos.z = tmp.z * (map.second.range / 50);
			DrawCircle(static_cast<int>(pos.x) + SizeHalf, static_cast<int>(pos.y) + SizeHalf, 2, 0xff0000);
		}
	}

}
