#pragma once

#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__

#include <string>
#include <unordered_map>
#include "3rd_party/olc/olcPixelGameEngine.h"
#include "3rd_party/olc/olcPGEX_Sound.h"
#include "3rd_party/olc/olcPGEX_TTF.h"


class ResourceManager {
public:	
	~ResourceManager();
	int GetAudio(std::string path);
	olc::Font* GetFont(std::string path, int size);
	olc::Sprite* GetImage(std::string path);
	void Clear();
	bool HasAudio(std::string);
	bool HasFont(std::string, int size);
	bool HasImage(std::string);
	static ResourceManager* Instance() {
		if (!instance) {
			instance = new ResourceManager();
		}
		return instance;
	}
private:
	ResourceManager();
	static ResourceManager* instance;
	std::unordered_map<std::string, int> audio;
	std::unordered_map<std::string, olc::Sprite*> image;
	std::unordered_map<std::string, olc::Font*> font;
};

#endif