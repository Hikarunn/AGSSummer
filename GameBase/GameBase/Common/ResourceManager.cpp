#include <DxLib.h>
#include "ResourceManager.h"

#include "Debug.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	imageMap_.clear();
	screenMap_.clear();
	modelMap_.clear();
	soundMap_.clear();
	shaderMap_.clear();
}

void ResourceManager::LoadTexture(SharedGraphicHandle& out, const std::filesystem::path& path, bool isNotRelese)
{
	std::lock_guard < std::mutex> lock{ mutex_ };
	auto key{ std::hash<std::string>()(path.string()) };
	if (imageMap_.contains(key))
	{
		out.SetPtr(imageMap_[key].first);
		return;
	}

	int h{ LoadGraph(path.wstring().c_str()) };
	if (h == -1)
	{
		DebugErrorLog("�摜�ǂݍ��ݎ��s");
	}
	imageMap_.emplace(key, std::make_pair(std::make_shared<int>(), isNotRelese));
	*imageMap_[key].first = h;
	out.SetPtr(imageMap_[key].first);
}

void ResourceManager::LoadDivTexture(SharedDivGraphicHandle& out, const std::filesystem::path& path, const Vector2& divCount, const Vector2I& divSize, bool isNotRelese)
{
	std::lock_guard < std::mutex> lock{ mutex_ };
	auto key{ std::hash<std::string>()(path.string()) };
	if (imageDivMap_.contains(key))
	{
		out.Set(std::get<0>(imageDivMap_[key]));
		out.SetPtr(std::get<1>(imageDivMap_[key]));
		return;
	}
	auto& vec{ std::get<0>(imageDivMap_[key]) };
	auto& sPtr{ std::get<1>(imageDivMap_[key]) };
	vec.resize(divCount.x * divCount.y);
	sPtr = std::make_shared<int>();
	int h{ LoadGraph(path.wstring().c_str()) };
	if (h == -1)
	{
		DebugErrorLog("�摜�ǂݍ��ݎ��s");
	}
	std::get<2>(imageDivMap_[key]) = isNotRelese;
	out.Set(vec);
	*sPtr = *vec.begin();
	out.SetPtr(sPtr);
}

void ResourceManager::LoadModelTexture(SharedGraphicHandle& out, const std::filesystem::path& path, bool isNotRelese)
{
	std::lock_guard < std::mutex> lock{ mutex_ };
	auto key{ std::hash<std::string>()(path.string()) };
	if (imageMap_.contains(key))
	{
		
		out.SetPtr(imageMap_[key].first);
		return;
	}

	int h{ MV1LoadTexture(path.wstring().c_str()) };
	if (h == -1)
	{
		DebugErrorLog("�摜�ǂݍ��ݎ��s");
	}
	imageMap_.emplace(key, std::make_pair(std::make_shared<int>(), isNotRelese));
	*imageMap_[key].first = h;
	out.SetPtr(imageMap_[key].first);
}

void ResourceManager::Remove(SharedGraphicHandle& handle)
{
	std::lock_guard<std::mutex> lock{ mutex_ };
	auto itr = std::find_if(imageMap_.begin(), imageMap_.end(), [&handle](auto& img) {return *img.second.first == *handle; });
	if (itr != imageMap_.end())
	{
		if (!itr->second.second)
		{
			imageMap_.erase(itr);
		}
	}
}

void ResourceManager::Remove(SharedDivGraphicHandle& handle)
{
	std::lock_guard<std::mutex> lock{ mutex_ };
	auto itr = std::find_if(imageDivMap_.begin(), imageDivMap_.end(), [&handle](auto& img) {return *std::get<1>(img.second) == *handle; });
	if (itr != imageDivMap_.end())
	{
		if (!std::get<2>(itr->second))
		{
			for (auto& h : std::get<0>(itr->second))
			{
				DeleteGraph(h);
			}
		}
	}

	imageDivMap_.erase(itr);

}

void ResourceManager::MakeRenderTarget(SharedRenderTargetHandle& out, const ScreenID id, const Vector2& size, bool alpha, bool isNotRelese)
{
	std::lock_guard<std::mutex> lock{ mutex_ };
	if (screenMap_.contains(id))
	{
		out.SetPtr(screenMap_[id].first);
		out.SetScreenID(id);
		return;
	}

	int h = MakeScreen(static_cast<int>(size.x), static_cast<int>(size.y), alpha);
	if (h == -1)
	{
		DebugErrorLog("�X�N���[���쐬�Ɏ��s");
	}
	screenMap_.emplace(id, std::make_pair(std::make_shared<int>(), isNotRelese));
	*screenMap_[id].first = h;
	out.SetPtr(screenMap_[id].first);
	out.SetScreenID(id);
}

void ResourceManager::Remove(const ScreenID id)
{
	if (screenMap_.contains(id))
	{
		if (!screenMap_[id].second)
		{
			screenMap_.erase(id);
		}
	}
}

void ResourceManager::LoadModel(SharedModelHandle& out, const std::filesystem::path& path, bool isNotRelese)
{
	// �L�[���v�Z
	auto key{ std::hash<std::string>()(path.string()) };
	std::lock_guard<std::mutex> lock{ mutex_ };
	// �`�F�b�N
	if (modelMap_.contains(key))
	{
		// ���Ƀ��[�h�������̂��L������
		if (CheckHandleASyncLoad(*std::get<0>(modelMap_[key])) == TRUE)
		{
			// ���[�h���������Ă��Ȃ��Ƃ�
			// �匳�ɂȂ�n���h����shared_ptr���Z�b�g
			out.SetParent(std::get<0>(modelMap_[key]));

			// ���[�h��������collback�����֐����Z�b�g
			loadedFunc_.push_back(std::bind(&SharedModelHandle::CopyParent, &out));
			return;
		}
		out.SetHandle(MV1DuplicateModel(*std::get<0>(modelMap_[key])));
		return;
	}
	int h{ MV1LoadModel(path.wstring().c_str()) };
	if (h == -1)
	{
		DebugErrorLog("���f�����[�h�Ɏ��s");
	}

	// �^�v���𐶐�
	modelMap_.emplace(key, std::make_tuple(std::make_shared<int>(), isNotRelese));

	// �匳�ɂȂ�ptr�Ƀn���h���̒l���Z�b�g
	*std::get<0>(modelMap_[key]) = h;

	// �o�͐�ɑ匳�ɂȂ�ptr���Z�b�g
	out.SetParent(std::get<0>(modelMap_[key]));

	// ���[�h�������ɌĂ΂��֐����Z�b�g
	loadedFunc_.push_back(std::bind(&SharedModelHandle::CopyParent, &out));

}

void ResourceManager::RemoveModel(const int handle)
{
	std::lock_guard<std::mutex> lock{ mutex_ };
	std::erase_if(modelMap_, [handle](auto& mdata) {
		if (*std::get<0>(mdata.second) == handle)
		{
			if (!std::get<1>(mdata.second))
			{
				MV1DeleteModel(handle);
				return true;
			}
		}
		return false;
		});

}

void ResourceManager::LoadPS(SharedShaderHandle& out, const std::filesystem::path& path, bool isNotRelese)
{
	auto key{ std::hash<std::string>()(path.string()) };
	if (shaderMap_.contains(key))
	{
		out.SetPtr(shaderMap_[key].first);
		return;
	}
	int h{ LoadPixelShader(path.wstring().c_str()) };
	if (h == -1)
	{
		DebugErrorLog("�s�N�Z���V�F�[�_�ǂݍ��ݎ��s");
	}
	shaderMap_.emplace(key, std::make_pair(std::make_shared<int>(), isNotRelese));
	*shaderMap_[key].first = h;
	out.SetPtr(shaderMap_[key].first);
	return;
}

void ResourceManager::LoadVS(SharedShaderHandle& out, const std::filesystem::path& path, bool isNotRelese)
{
	auto key{ std::hash<std::string>()(path.string()) };
	if (shaderMap_.contains(key))
	{
		out.SetPtr(shaderMap_[key].first);
		return;
	}
	int h{ LoadVertexShader(path.wstring().c_str()) };
	if (h == -1)
	{
		DebugErrorLog("���_�V�F�[�_�ǂݍ��ݎ��s");
	}
	shaderMap_.emplace(key, std::make_pair(std::make_shared<int>(), isNotRelese));
	*shaderMap_[key].first = h;
	out.SetPtr(shaderMap_[key].first);
	return;
}

bool ResourceManager::IsRemove(SharedShaderHandle& handle)
{
	auto itr = std::find_if(shaderMap_.begin(), shaderMap_.end(), [&handle](auto& img) {return *img.second.first == *handle; });
	if (itr == shaderMap_.end())
	{
		return false;
	}
	return !itr->second.second;
}

void ResourceManager::Remove(SharedShaderHandle& handle)
{
	auto itr = std::find_if(shaderMap_.begin(), shaderMap_.end(), [&handle](auto& img) {return *img.second.first == *handle; });
	if (itr != shaderMap_.end())
	{
		if (!itr->second.second)
		{
			shaderMap_.erase(itr);
		}
	}
}

void ResourceManager::LoadSound(SharedSoundHandle&& out, const std::filesystem::path& path, bool isNotRelese)
{
	auto key{ std::hash<std::string>()(path.string()) };
	if (soundMap_.contains(key))
	{
		// ���Ƀ��[�h�������̂��L������
		if (CheckHandleASyncLoad(*soundMap_[key].first) == TRUE)
		{
			// ���[�h���������Ă��Ȃ��Ƃ�
			// �匳�ɂȂ�n���h����shared_ptr���Z�b�g
			out.SetPtr(soundMap_[key].first);

			// ���[�h��������collback�����֐����Z�b�g
			loadedFunc_.push_back(std::bind(&SharedSoundHandle::CopyParent, &out));
			return;
		}
		out.SetHandle(DuplicateSoundMem(*std::get<0>(modelMap_[key])));
		return;
	}
	int h{ LoadSoundMem(path.c_str()) };
	if (h == -1)
	{
		DebugErrorLog("�T�E���h�ǂݍ��ݎ��s");
	}
	soundMap_.emplace(key, std::make_pair(std::make_shared<int>(), isNotRelese));
	*soundMap_[key].first = h;
	out.SetPtr(soundMap_[key].first);
	loadedFunc_.push_back(std::bind(&SharedSoundHandle::CopyParent, &out));
}

void ResourceManager::RemoveSound(const int handle)
{
	auto itr = std::find_if(soundMap_.begin(), soundMap_.end(), [&handle](auto& img) {return *img.second.first == handle; });
	if (itr != soundMap_.end())
	{
		if (!itr->second.second)
		{
			DeleteSoundMem(*itr->second.first);
			soundMap_.erase(itr);
		}
	}
}

void ResourceManager::Loaded(void)
{
	for (auto& f : loadedFunc_)
	{
		f();
	}
	loadedFunc_.clear();
}
