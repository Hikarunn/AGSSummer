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
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="id">�V�[��</param>
	/// <param name="sceneID">�����V�[��</param>
	BaseScene(ScreenID id, SceneID sceneID);

	/// <summary>
	/// �R���X�g���N�^(�I�u�W�F�N�g�}�l�[�W���[���p���p)
	/// </summary>
	/// <param name="objectManager">�I�u�W�F�N�g�}�l�[�W���[</param>
	/// <param name="id">�X�N���[���̎��</param>
	/// <param name="sceneID">�V�[���̎��</param>
	BaseScene(std::unique_ptr<ObjectManager>&& objectManager, ScreenID id, SceneID sceneID);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BaseScene();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta">�f���^�^�C��</param>
	/// <param name="controller"></param>
	virtual void Update(float delta, Controller& controller) = 0;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	/// <param name=""></param>
	virtual void DrawScene(void) = 0;

	/// <summary>
	/// �V�[��ID�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual const SceneID GetID(void) const = 0;

	/// <summary>
	/// �V�[�����̊J������
	/// </summary>
	/// <param name=""></param>
	virtual void Relese(void);

	/// <summary>
	/// ���[�h�����̊m�F
	/// </summary>
	/// <param name=""></param>
	/// <returns>true�̏ꍇ���[�h����</returns>
	virtual bool IsLoaded(void);

	/// <summary>
	/// �V�[���̃|�C���^�X�V����
	/// </summary>
	/// <param name="own">���݂̃V�[���̃|�C���^</param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	/// <returns>���̃t���[���Ŏg���|�C���^</returns>
	SceneUptr Update(SceneUptr own, float delta, Controller& controller);

	/// <summary>
	/// �V�[���J��
	/// </summary>
	/// <param name="id">�ύX��̃V�[��ID</param>
	void ChangeSceneID(SceneID id);

	/// <summary>
	/// �V�[�������̂��߂�function�Z�b�g�p
	/// </summary>
	/// <param name="sceneMakeFunc">�����ɕK�v��unction</param>
	/// <param name="id">��������V�[��ID</param>
	void SetMakeSceneFunc(std::function<SceneUptr(SceneUptr)>&& sceneMakeFunc, const SceneID id);

	/// <summary>
	/// ���[�h���������������Ƃ��̏���
	/// </summary>
	/// <param name="controller"></param>
	void Loaded(Controller& controller);

	/// <summary>
	/// ���[�h�������̒ǉ�����
	/// </summary>
	/// <param name="loadFunc"></param>
	void AddLoadedFunc(LoadedFunc&& loadFunc);

	/// <summary>
	/// �J�����̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns>�J�����̎Q��</returns>
	Camera& GetCamera(void)
	{
		return *camera_;
	}

	/// <summary>
	/// �I�u�W�F�N�g�}�l�[�W���[�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns>�I�u�W�F�N�g�}�l�[�W���[�̎Q��</returns>
	ObjectManager& GetObjectManager(void)
	{
		return *objManager_;
	}

protected:

	// �ڍs���ID
	SceneID nextID_;

	// �X�N���[����ID
	ScreenID screenID_;

	// �X�N���[���n���h��
	SharedRenderTargetHandle screenHandle_;

	// �J����
	std::unique_ptr<Camera> camera_;

	// UI�}�l�[�W���[
	std::unique_ptr<UiManager> uiMneger_;

	// �I�u�W�F�N�g�}�l�[�W���[
	std::unique_ptr<ObjectManager> objManager_;

private:
	
	// �V�[���쐬��function���܂Ƃ߂��}�b�v
	std::unordered_map<SceneID, std::function<SceneUptr(SceneUptr)>> seneMakeFuncMap_;

	// ���[�h�����������Ƃ��Ɏ��s����function
	std::list<LoadedFunc> loadedFunc_;
};


