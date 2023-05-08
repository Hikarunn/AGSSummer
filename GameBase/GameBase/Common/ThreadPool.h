#pragma once
#include <mutex>
#include <list>
#include <functional>
#include <memory>
#include <vector>

class Worker;

class ThreadPool
{
public:
	ThreadPool(unsigned int threadNum = 1);
	~ThreadPool();

	/// <summary>
	/// ���s����֐���ǉ�
	/// </summary>
	/// <param name="func"> ���s�������֐� </param>
	void Add(std::function<void(void)>&& func);

	/// <summary>
	/// �X���b�h���I���҂�����
	/// </summary>
	/// <param name=""></param>
	void Join(void);

	/// <summary>
	/// List�̒��g��j�����Ď��s���̏������I����܂őҋ@
	/// </summary>
	/// <param name=""></param>
	void Wait(void);

	/// <summary>
	/// ���ׂẴ^�X�N���I���܂őҋ@����
	/// </summary>
	/// <param name=""></param>
	void WaitAllTask(void);

	bool GetTask(Worker& woerker);


private:

	// �~���[�e�b�N�X
	std::mutex mutex_;

	// ���s����^�X�N
	std::list<std::function<void(void)>> taskList_;


	std::vector<std::unique_ptr< Worker>> workers_;
};

