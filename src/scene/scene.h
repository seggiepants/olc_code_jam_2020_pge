#include "../3rd_party/olc/olcPixelGameEngine.h"
#include "../resource_manager.h"

#pragma once

#ifndef __SCENE_H__
#define __SCENE_H__

class Scene {
public:
	Scene() {};
	virtual ~Scene() {};
	virtual void init(olc::PixelGameEngine* app) = 0;
	virtual void destroy() = 0;
	virtual void update(double ms, olc::imgui::PGE_ImGUI* gui) = 0;
	virtual bool running() = 0;
	virtual int getNextState() = 0;
};

#endif