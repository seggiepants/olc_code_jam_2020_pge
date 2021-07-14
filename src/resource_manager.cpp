#include <iostream>
#include "resource_manager.h"

ResourceManager* ResourceManager::instance = NULL;

ResourceManager::ResourceManager()
{
	instance->Clear();
}

ResourceManager::~ResourceManager()
{
	instance->Clear();
}

int ResourceManager::GetAudio(std::string path)
{
	if (instance->HasAudio(path))
	{
		return instance->audio[path];
	}
	else
	{
		int sampleID = olc::SOUND::LoadAudioSample(path);

		if (sampleID < 0) {
			std::cout << "Failed to load sound \"" << path << "\"" << std::endl;
		}
		else
		{
			instance->audio[path] = sampleID;
		}

		return sampleID;
	}
}

olc::Font* ResourceManager::GetFont(std::string path, int size)
{
	if (instance->HasFont(path, size))
	{
		return instance->font[path + "_" + std::to_string(size)];
	}
	else
	{
		std::string key = path + "_" + std::to_string(size);
		olc::Font* font = new olc::Font(path, size);
		if (font == NULL)
		{
			std::cout << "Failed to load font \"" << path << "\" Size " << size << "." << std::endl;
		}
		else
		{
			instance->font[key] = font;
		}
		return font;
	}
}

olc::Sprite* ResourceManager::GetImage(std::string path)
{

	if (instance->HasImage(path))
		return instance->image[path];
	else 
	{
		// Load image at specified path
		olc::Sprite* sprite = new olc::Sprite();
		olc::rcode ret = sprite->LoadFromFile(path);		
		if (ret != olc::rcode::OK)
		{
			std::cout << "Unable to load image \"" << path << "\"" << std::endl;
		}
		else {
			instance->image[path] = sprite;
		}
		return sprite;
	}
}

void ResourceManager::Clear()
{
	if (instance != NULL)
	{
		// PGE Audio extension doesn't appear to have an unload sample command.
		instance->audio.clear();

		for (const auto &element : instance->font) {
			delete element.second;
		}
		instance->font.clear();
		
		for (const auto &element : instance->image) {
			delete element.second;
		}
		instance->image.clear();
		ResourceManager* temp = instance;
		instance = NULL;
		delete temp;
	}
}

bool ResourceManager::HasAudio(std::string path)
{
	std::unordered_map<std::string, int>::const_iterator search = instance->audio.find(path);

	return (search != instance->audio.end());

}


bool ResourceManager::HasFont(std::string path, int size)
{
	std::unordered_map<std::string, olc::Font*>::const_iterator search = instance->font.find(path + "_" + std::to_string(size));

	return (search != instance->font.end());
}

bool ResourceManager::HasImage(std::string path)
{
	std::unordered_map<std::string, olc::Sprite*>::const_iterator search = instance->image.find(path);

	return (search != instance->image.end());
}
