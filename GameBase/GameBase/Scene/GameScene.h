#pragma once
#include <DxLib.h>
#include <vector>
#include "BaseScene.h"
#include "../Common/Shared.h"
#include "StageID.h"

struct DepthParameter
{
	float start;
	float end;
	float scope;
};

struct WEIGHT_NUM
{
	float weight[8];
};

// �G�Ƃ̋����Ɗp�x���m��
struct Rader
{
	float angle;
	float range;
	bool flag;
};
class GameScene :
	public BaseScene
{
public:
	GameScene(StageID stageID);
	~GameScene();

	// ���C�g�J�����̈ʒu

	// ���C�g�J���������_

	// ���[�_�[�ɓG��\������͈�

	void SetResult(Result result)
	{
		result_ = result;
	}

	/// <summary>
	/// �Q�[���ɖ߂�Ƃ��ɋ��ޏ���
	/// </summary>
	/// <param name=""></param>
	void SetUp(void);
	

private:
	const SceneID GetID(void)const
	{
		return SceneID::Game;
	}

	/// <summary>
	/// ���U���g�̐���
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeResultFunc(SceneUptr own);

	/// <summary>
	/// �|�[�Y�V�[���̐���
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakePauseFunc(SceneUptr own);

	/// <summary>
	/// �Z���N�g�V�[���̐���
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeSelectFunc(SceneUptr own);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="deltaa"></param>
	/// <param name="controller"></param>
	void Update(float deltaa, Controller& controller)final;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void)final;

	/// <summary>
	/// ���C���ȊO�̃X�N���[��
	/// </summary>
	/// <param name=""></param>
	void SetSubScreen(void);

	/// <summary>
	/// ���[�h���������Ă��邩
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsLoaded(void)final;

	/// <summary>
	/// ���[�h�������̏���
	/// </summary>
	/// <param name="controller"></param>
	void Loaded(Controller& controller);

	/// <summary>
	/// ���[�_�[�̃Z�b�g�A�b�v
	/// </summary>
	/// <param name=""></param>
	void RaderSetUp(void);

	/// <summary>
	/// ���[�_�[�̍X�V
	/// </summary>
	/// <param name="pPos"></param>
	void RaderUpdate(Vector2 pPos);

	/// <summary>
	/// ���[�_�[�̕`��
	/// </summary>
	/// <param name=""></param>
	void RaderDrow(void);

	// �I�t�X�N���[��
	int offScreen_;
	// �X�J�C�h�[���݂̂̃X�N���[��
	int skyScreen_;

	// �X�J�C�h�[���ƃX�e�[�W�݂̂̃X�N���[��
	int subScreen_;

	// ���[�_�[�p�̃X�N���[��
	int raderMap_;

	// �v���C���[�ƃG�l�~�[�̋����Ɗp�x���G�l�~�[�̐������擾
	std::map<int, Rader>rader_;

	// ���[�_�[�̃T�C�Y
	int raderSize_;

	Result result_;

	// ���݂̃X�e�[�WID
	StageID stageID_;

	int radarGraph_;

	friend class PauseScene;
};

