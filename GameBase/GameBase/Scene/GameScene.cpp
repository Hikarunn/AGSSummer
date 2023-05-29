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
		// �`���[�g���A����
		uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/game.ui", true, true, false);
		objManager_->AddFactory(std::make_unique<StageFactory>(*objManager_));
		SetMakeSceneFunc(std::bind(&GameScene::MakeSelectFunc, this, std::placeholders::_1), SceneID::Select);
	}
	else
	{
		// �ʏ펞�̃Q�[����
		objManager_->AddFactory(std::make_unique<StageFactory>("Resource/Other/Stage" + std::to_string(static_cast<int>(stageID)) + ".data", *objManager_));
		//objMng_->AddFactory(std::make_unique<StageFactory>("Resource/Other/dbg.data", *objMng_));
		uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/game.ui", true, false, false);
//		lpSceneManager.GetResourceManager().MakeRenderTarget(resultCapture_, ScreenID::ResultCapture, SceneManager::screenSize_<float>, true);
	}
	AddLoadedFunc(std::bind(&GameScene::Loaded, this, std::placeholders::_1));
	result_ = Result::Max;

	// �Q�[���V�[���Ŏg���V�F�[�_�����炩���߃��[�h���Ă���
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
	// �V���h�E�}�b�v���쐬����ۂɍ����t�H�[�}�b�g�ݒ�͂���Ȃ��̂Őݒ��������
	// ���ɍ쐬����摜�̃t�H�[�}�b�g�̐ݒ������
	SetDrawValidFloatTypeGraphCreateFlag(true);
	// �`���l�����P�ɂ���
	SetCreateDrawValidGraphChannelNum(1);
	// �r�b�g����16�ɂ���
	SetCreateGraphColorBitDepth(16);
	// �V���h�E�}�b�v
	shadowMap_ = MakeScreen(x, y, false);

	// ���ɖ߂�
	SetDrawValidFloatTypeGraphCreateFlag(false);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphColorBitDepth(32);
	// ��ʊE�[�x
	depth_ = MakeScreen(x, y, false);

	// �I�t�X�N���[���̏�����
	offScreen_ = MakeScreen(x, y, false);
	// �X�J�C�h�[���̃X�N���[��
	skyScreen_ = MakeScreen(x, y, false);
	// �X�J�C�h�[���ƃX�e�[�W�̃X�N���[��
	subScreen_ = MakeScreen(x, y, false);

	radarGraph_ = LoadGraph(L"Resource/resource/Radar.png");

	// �~�j�}�b�v�p�̃X�N���[��
	radarSize_ = y / 4;
	radarMap_ = MakeScreen(radarSize_, radarSize_, false);
	count_ = 0;

	// �V���h�E�}�b�v�p�J�������̏�����
	SetUseASyncLoadFlag(false);
	shadowBuff_ = CreateShaderConstantBuffer(sizeof(LIGHT_MAT) * 4);
	lightMat_ = static_cast<LIGHT_MAT*>(GetBufferShaderConstantBuffer(shadowBuff_));
	lightMat.view = MGetIdent();
	lightMat.proj = MGetIdent();

	// ��ʊE�[�x�p�̃J�������̏�����
	depthbuffer_ = CreateShaderConstantBuffer(sizeof(DepthParameter) * 4);
	depthMat_ = static_cast<DepthParameter*>(GetBufferShaderConstantBuffer(depthbuffer_));
	depthMat.start = 0.0f;
	depthMat.end = 0.0f;
	depthMat.scope = 0.0f;
	SetUseASyncLoadFlag(true);

	// �Q�[���V�[���p��BGM�̃��[�h
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
//	// ��xUi�ȊO��`�悷��
//	SetupShadowMap();
//	SetOffsetScreen();
//	SetSubScreen();
//	SetUpDepth();
//	depthMat_[0] = depthMat;
//	peMng_->SetBuffer(depthbuffer_);
//	peMng_->Draw(offScreen_, *screenHandle_, depth_, skyScreen_, subScreen_);
//
//	// �`�挋�ʂ��L���v�`������
//	SetDrawScreen(*screenHandle_);
//	GetDrawScreenGraph(0, 0, SceneManager::screenSize_<int>.x, SceneManager::screenSize_<int>.y, *resultCapture_);
//}

void GameScene::SetUp(void)
{
	//peMng_->SetFlag(PEID::Mono, lpConfigMng.GetPeConfig().at(PEID::Mono));
	//peMng_->SetFlag(PEID::VolFog, lpConfigMng.GetPeConfig().at(PEID::VolFog));
	//lpSooundPross.PlayBackSound(SOUNDNAME_BGM::GameSceneBGM, lpSooundPross.GetVolume(), true);	// �Q�[���V�[����BGM
	//lpSooundPross.PlayBackSound(SOUNDNAME_BGM::GameSceneBGM, lpSooundPross.GetVolume(), true);	// �Q�[���V�[����BGM

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
	// �V���h�E�}�b�v�쐬
	SetupShadowMap();
	// �I�t�X�N���[���̍쐬
	SetOffsetScreen();
	// ���C���ȊO�̃X�N���[���̍쐬
	SetSubScreen();

	// ��ʊE�[�x�p�̐[�x�e�N�X�`���̍쐬
	SetUpDepth();

	depthMat_[0] = depthMat;
	//peMng_->SetBuffer(depthbuffer_);
	// �|�X�g�G�t�F�N�g���ʏ�`��
	//peMng_->Draw(offScreen_, *screenHandle_, depth_, skyScreen_, subScreen_);

	// �~�j�}�b�v�̍쐬
	DrawGraph(10, 10, radarMap_, true);

	// UI�̕`��
	uiManager_->Draw();

}

void GameScene::SetupShadowMap(void)
{
	// �`�����e�p�[�x�L�^�摜�ɕύX����
	SetDrawScreen(shadowMap_);
	// �e�p�[�x�L�^�摜����x�^�����ɂ���
	SetBackgroundColor(255, 255, 255);
	ClsDrawScreen();
	SetBackgroundColor(0, 0, 0);

	// �V���h�E�}�b�v�p�ɃJ�������Z�b�g
	camera_->SetUpShadow(offsetOrtho, offsetNear, offsetFar, camTar);

	MV1SetUseOrigShader(true);
	objManager_->SetupDepthTex(*shadowPs_, -1);
	MV1SetUseOrigShader(false);

	// �V���h�E�}�b�v�쐬�Ɏg�����J���������擾
	lightMat.view = GetCameraViewMatrix();
	lightMat.proj = GetCameraProjectionMatrix();
	// �`��p�ɐ؂�ւ�
	SetDrawScreen(offScreen_);
	ClsDrawScreen();
	// �������擾�����J��������hlsl���ɓn��
	lightMat_[0] = lightMat;
}

void GameScene::SetUpDepth(void)
{
	// �`�����e�p�[�x�L�^�摜�ɕύX
	SetDrawScreen(depth_);
	// �e�p�[�x�L�^�摜����x�^�����ɂ���
	SetBackgroundColor(255, 255, 255);
	ClsDrawScreen();
	SetBackgroundColor(0, 0, 0);
	camera_->SetScreen();

	// ��ʊE�[�x�J�n�ʒu�̌v�Z
	depthMat.start = dofFocus - dofFocusSize / 2.0f - dofInterpSize;
	// ��ʊE�[�x�I���ʒu���v�Z
	depthMat.end = dofFocus + dofFocusSize / 2.0f + dofInterpSize;
	// ��ʊE�[�x�͈̔͂̋t�����v�Z
	depthMat.scope = 1.0f / (depthMat.end - depthMat.start);
	// ��Ԕ͈͂ƃt�H�[�J�X�������Ă���͈͂��܂߂����������Z�o
	dofTotalSize_ = dofInterpSize * 2.0f + dofFocusSize;

	// �擾�����f�[�^��hlsl���ɓn��
	depthMat_[0] = depthMat;
	MV1SetUseOrigShader(true);
	objManager_->SetupDepthTex(*depthPS_, depthbuffer_);
	MV1SetUseOrigShader(false);

	// �`��p�ɐ؂�ւ�
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	// ��ʊE�[�x�J�n�ʒu�̌v�Z
	depthMat.start = dofInterpSize / dofTotalSize_;
	// ��ʊE�[�x�I���ʒu�̌v�Z
	depthMat.end = (dofInterpSize + dofFocusSize) / dofTotalSize_;

}

void GameScene::SetOffsetScreen(void)
{
	camera_->SetScreen();

	// �`��
	objManager_->ShadowDraw(shadowMap_, shadowBuff_);
	MV1SetUseOrigShader(false);

	// 3D�n�̃f�o�b�O�`��
	DebugDraw3DScreen();

	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
}

void GameScene::SetSubScreen(void)
{
	// �X�J�C�h�[���݂̂̃X�N���[��
	SetDrawScreen(skyScreen_);
	ClsDrawScreen();
	camera_->SetScreen();
	auto [result, id] = objManager_->Find(ObjectAttribute::Sky);
	objManager_->GetComponent<Render>(id)->Draw();

	// �X�J�C�h�[���ƃX�e�[�W�݂̂̃X�N���[��
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
	// ���[�h�������ɌĂ΂��
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
	// �v���C���[�̏����擾
	auto player = objManager_->GetComponent<Transform>(objManager_->GetPlayerID());
	Vector2 pPos = Vector2(player->GetPos().x, player->GetPos().z);
	// �G�l�~�[�̏����擾
	auto stage = objManager_->GetComponent<StageBehavior>(objManager_->GetStageID());
	auto& list = stage->GetEnemyPosList();
	// �v���C���[�Ƃ̋����Ɗp�x���擾
	int i = 0;
	for (auto ePos : list)
	{
		Radar radar;
		// �v���C���[�Ƃ̋������擾
		Vector2 tmp = ePos.second - pPos;
		radar.range = tmp.Magnitude();
		// �v���C���[�Ƃ̊p�x���擾
		// �J�����̏����擾
		auto camera = objManager_->GetComponent<Transform>(objManager_->GetCameraID());
		// �J�����̌����Ă����������̊p�x�����߂�
		float angle = GetAngle2Vector(Vector2(camera->GetForward().x, camera->GetForward().z), tmp);
		// �o�����̂��i�[
		radar.angle = angle;

		// �f�t�H���g�͕\�����Ȃ�
		radar.flag = false;
		// �������\���\�͈͓��ł���Ε\������
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
	// �v���C���[�̏����擾
	auto player = objManager_->GetComponent<Transform>(objManager_->GetPlayerID());
	// �G�l�~�[�̏����擾
	auto stage = objManager_->GetComponent<StageBehavior>(objManager_->GetStageID());
	auto& list = stage->GetEnemyPosList();
	// �v���C���[�Ƃ̋����Ɗp�x���擾
	int i = 0;
	for (auto ePos : list)
	{
		// �v���C���[�Ƃ̋������擾
		Vector2 tmp = ePos.second - pPos;
		float range = tmp.Magnitude();
		// �J�����̏����擾
		auto camera = objManager_->GetComponent<Transform>(objManager_->GetCameraID());
		// �J�����̌����Ă����������̊p�x�����߂�
		float angle = GetAngle2Vector(Vector2(camera->GetForward().x, camera->GetForward().z), tmp);
		// �f�t�H���g�͕\�����Ȃ�
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

	// ���S�ʒu�̎擾
	int SizeHalf = radarSize_ / 2;
	// �v���C���[�̈ʒu
	DrawCircle(SizeHalf, SizeHalf, 5, 0x0000ff);

	for (const auto& map : radar_)
	{
		if (map.second.flag)
		{
			Vector3 pos = Vector3(0, -1, 0);
			Vector3 tmp;
			// �󂯎�����p�x��sin��cos�̒l������
			float sinParam = sin(-map.second.angle);
			float cosParam = cos(-map.second.angle);
			// sinParam��cosParam�œG�̈ʒu���擾����
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
