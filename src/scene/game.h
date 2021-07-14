#pragma once
#include <unordered_map>
#include "../3rd_party/olc/olcPixelGameEngine.h"
#include "../3rd_party/olc/olcPGEX_Gamepad.h"
#include "../3rd_party/olc/olcPGEX_Sound.h"
#include "../3rd_party/olc/olcPGEX_TTF.h"
#include "../3rd_party/imgui/imgui.h"
#include "../3rd_party/imgui/imgui_impl_pge.h"
#include "scene.h"
#include "../sprite/sprite_group.h"

class SceneGame : Scene
{
public:
	SceneGame();
	~SceneGame();
	void init(olc::PixelGameEngine* app);
	void destroy();
	void update(double ms, olc::imgui::PGE_ImGUI* gui);
	void loadMedia();	
	bool running();
	int getNextState();
	void addPoints(int points);
	double getInvaderSpeed();
	void gameOver();
private:
	void initGame();
	void initInvaders();
	void initShields(int y); 
	void initTank();
	void invaderPostUpdate(double ms);
	void drawFPS(double fps);
	void drawScore();
	void spawnTankBullet();
	olc::PixelGameEngine* app;
	olc::GamePad* joystick;
	olc::Font* font;
	int points;
	double invaderShootDelay;
	double dropDistance;
	double nextDirection;
	bool showFPS;
	bool isGameOver;
	bool quit;
	bool pause;
	bool left;
	bool right;
	int bloop;
	int laser;
	SpriteGroup tank;
	SpriteGroup tankBullet;
	SpriteGroup invader;
	SpriteGroup invaderBullet;
	SpriteGroup shield;
};