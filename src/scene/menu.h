#pragma once
#include <unordered_map>
#include "../3rd_party/olc/olcPixelGameEngine.h"
#include "../3rd_party/imgui/imgui.h"
#include "../3rd_party/imgui/imgui_impl_pge.h"
#include "scene.h"

class SceneMenu : Scene
{
public:
	SceneMenu();
	~SceneMenu();
	void init(olc::PixelGameEngine* app);
	void destroy();
	void update(double ms, olc::imgui::PGE_ImGUI* gui);
	void loadMedia();
	bool running();
	int getNextState();
private:
	olc::PixelGameEngine* app;
	ResourceManager* resource;
	bool quit;
	int nextState;
	olc::Sprite* background;
};