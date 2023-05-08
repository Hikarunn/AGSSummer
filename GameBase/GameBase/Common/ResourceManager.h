#pragma once
#include <filesystem>
#include <map>
#include <mutex>
#include <list>
#include <tuple>
#include "Shared.h"
#include "ScreenID.h"

class ResourceManager
{
	using HandleFlagPair = std::pair<std::shared_ptr<int>, bool>;
	using HandleMap = std::map<std::uint64_t, HandleFlagPair>;
	using HandleDivMap = std::map <std::uint64_t, std::tuple<std::vector<int>, std::shared_ptr<int>, bool>>;
	using ScreenMap = std::map<ScreenID, HandleFlagPair>;
	using ModelMap = std::map<std::uint64_t, std::tuple<std::shared_ptr<int>, bool>>;

public:
	ResourceManager();
	~ResourceManager();

	/// <summary>
	/// �摜��̓ǂݍ���
	/// </summary>
	/// <param name="out"></param>
	/// <param name=""></param>
	/// <param name="isNotRelese"></param>
	void LoadTexture(SharedGraphicHandle& out, const std::filesystem::path& path, 
		bool isNotRelese = false);
	
	/// <summary>
	/// �摜�����ǂݍ���
	/// </summary>
	/// <param name="out"></param>
	/// <param name=""></param>
	/// <param name="divCount"></param>
	/// <param name="divSize"></param>
	/// <param name="isMotRelese"></param>
	void LoadDivTexture(SharedDivGraphicHandle& out, const std::filesystem::path& path, 
		const Vector2& divCount, const Vector2I& divSize, bool isNotRelese = false);

	/// <summary>
	/// ���f����
	/// </summary>
	/// <param name="out"></param>
	/// <param name=""></param>
	/// <param name="isNotRelese"></param>
	void LoadModelTexture(SharedGraphicHandle& out, const std::filesystem::path& path,
		bool isNotRelese = false);

	/// <summary>
	/// ��v����O���t�B�b�N�n���h�����폜
	/// </summary>
	/// <param name="handle"></param>
	void Remove(SharedGraphicHandle& handle);

	/// <summary>
	/// ��v����O���t�B�b�N�n���h�����폜
	/// </summary>
	/// <param name="handle"></param>
	void Remove(SharedDivGraphicHandle& handle);
	
	/// <summary>
	/// �����_�[�^�[�Q�b�g�̍쐬
	/// </summary>
	/// <param name="out"></param>
	/// <param name="id"></param>
	/// <param name="size"></param>
	/// <param name="alpha"></param>
	/// <param name="isNotRelese"></param>
	void MakeRenderTarget(SharedRenderTargetHandle& out, const ScreenID id,
		const Vector2& size, bool alpha, bool isNotRelese = false);

	/// <summary>
	/// �폜
	/// </summary>
	/// <param name="id"></param>
	void Remove(const ScreenID id);

	/// <summary>
	/// ���f���̃��[�h
	/// </summary>
	/// <param name="out"></param>
	/// <param name=""></param>
	/// <param name="isNotRelese"></param>
	void LoadModel(SharedModelHandle& out, const std::filesystem::path& path, 
		bool isNotRelese = false);

	/// <summary>
	/// ���f���̃n���h���폜
	/// </summary>
	/// <param name="handle"></param>
	void RemoveModel(const int handle);

	/// <summary>
	/// �s�N�Z���V�F�[�_��ǂݍ���
	/// </summary>
	/// <param name="path"></param>
	/// <param name="isNotRelese"></param>
	/// <returns></returns>
	void LoadPS(SharedShaderHandle& out, const std::filesystem::path& path, bool isNotRelese = false);

	/// <summary>
	/// ���_�V�F�[�_��ǂݍ���
	/// </summary>
	/// <param name="path"></param>
	/// <param name="isNotRelese"></param>
	/// <returns></returns>
	void LoadVS(SharedShaderHandle& out, const std::filesystem::path& path, bool isNotRelese = false);

	/// <summary>
	/// ��v����V�F�[�_�n���h�����Ǘ�����폜���ׂ���
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	bool IsRemove(SharedShaderHandle& handle);

	/// <summary>
	/// ��v����V�F�[�_�n���h�����Ǘ�����폜����
	/// </summary>
	/// <param name="handle"></param>
	void Remove(SharedShaderHandle& handle);


	/// <summary>
	/// �T�E���h�̃��[�h
	/// </summary>
	/// <param name="out"></param>
	/// <param name=""></param>
	/// <param name="isNotRelese"></param>
	void LoadSound(SharedSoundHandle&& out, const std::filesystem::path& path, bool isNotRelese);

	/// <summary>
	/// �T�E���h�̃n���h���폜
	/// </summary>
	/// <param name="handle"></param>
	void RemoveSound(const int handle);


	void Loaded(void);

private:
	std::mutex mutex_;


	HandleMap imageMap_;
	HandleDivMap imageDivMap_;
	ScreenMap screenMap_;
	HandleMap soundMap_;
	ModelMap modelMap_;
	HandleMap shaderMap_;

	std::list<std::function<void(void)>>loadedFunc_;
};

