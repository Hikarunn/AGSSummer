#pragma once
#include <memory>
#include "Common/Time.h"
#include "Common/Random.h"
#include "Common/Math.h"

#define lpSceneManager (SceneManager::GetInstance())
#define Rand (SceneManager::GetInstance().GetRandomClass())
#define Thread (SceneManager::GetInstance().GetThredPool())

class ResourceManager;
class BaseScene;
class ThreadPool;
class Controller;

class SceneManager
{
public:
	/// <summary>
	/// ���s����
	/// </summary>
	int Run(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();


	static SceneManager& GetInstance(void)
	{
		return instance_;
	};

	/// <summary>
	/// �^�C���N���X�̎擾
	/// </summary>
	TimeClass& GetTimeClass(void);

	/// <summary>
	/// ���������N���X�̎擾
	/// </summary>
	Random& GetRandomClass(void);

	/// <summary>
	/// �X���b�h�v�[��
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ThreadPool& GetThredPool(void);

	/// <summary>
	/// ���\�[�X�}�l�[�W���[�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ResourceManager& GetResourceManager(void);

	/// <summary>
	/// �v���O�������I��
	/// </summary>
	/// <param name=""></param>
	void End(void)
	{
		isEnd_ = true;
	}

	// �X�N���[���T�C�Y
	template<ValueC C>
	static constexpr Vector2Tmp<C> screenSize_{ 1920, 1080}

	Controller& GetController(void);

private:
	SceneManager();
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name=""></param>
	/// <returns>�����̏ꍇtrue:���s�̏ꍇfalse</returns>
	bool Init(void);

	/// <summary>
	/// �V�X�e���֘A�̏�����
	/// </summary>
	/// <param name=""></param>
	/// <returns>�����̏ꍇtrue:���s�̏ꍇfalse</returns>
	bool SystemInit(void);

	// �C���X�^���X
	static SceneManager instance_;

	// ���������ł������ǂ����̃t���O
	bool isInit_;

	// �I�����邩�̃t���O
	bool isEnd_;

	std::unique_ptr<ResourceManager>resourceManager_;

	// �^�C���N���X
	TimeClass timeClass_;

	// �����_���N���X
	Random random_;

	// �V�[��
	std::unique_ptr<BaseScene> scene_;

	// �X���b�h�v�[��
	std::unique_ptr<ThreadPool> threadPool_;

	// �R���g���[���[
	std::unique_ptr<Controller> controller_;

	// ���\�[�X�}�l�[�W���[
	std::unique_ptr<ResourceManager> resourceMnager_;
};

