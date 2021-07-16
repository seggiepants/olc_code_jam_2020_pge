#include <algorithm>
#include <codecvt>
#include <iostream>
#include <iomanip>
#include <locale>
#include <vector>
#define OLC_PGE_GAMEPAD
#include "game.h"
#include "../utility.h"
#include "../sprite/bullet.h"
#include "../sprite/invader.h"
#include "../sprite/shield.h"
#include "../sprite/tank.h"

const double INVADER_SHOOT_DELAY_MIN = 700;
const int INVADER_SHOOT_RANDOM = 700;
const double DROP_TOTAL = 12.0;
const int MAX_COLS = 10; // duplicate data :(
const int MAX_ROWS = 5; // duplicate data :(
const float JOYSTICK_DEAD_ZONE = 0.3;

const std::string PATH_BLOOP = "assets/sound/bloop.wav"; // .ogg";
const std::string PATH_FONT = "assets/font/NovaSquareBoldOblique.ttf";
const std::string PATH_LASER = "assets/sound/laser.wav"; // .ogg";

const int FONT_SIZE = 18;

SceneGame::SceneGame() {
	this->app = nullptr;
	this->bloop = 0;
	this->invaderShootDelay = INVADER_SHOOT_DELAY_MIN + (double)(rand() % INVADER_SHOOT_RANDOM);
	this->laser = 0;
	this->quit = false;	
	this->font = ResourceManager::Instance()->GetFont(PATH_FONT, FONT_SIZE);
	this->pause = false;
	this->points = 0;
	this->dropDistance = 0;
	this->nextDirection = 0.0;
	this->isGameOver = false;
	this->left = this->right = false;
#ifdef _DEBUG
	this->showFPS = true;
#else
	this->showFPS = false;
#endif
	this->joystick = nullptr;
}

SceneGame::~SceneGame() {
	this->destroy();
}

int SceneGame::getNextState() {
	return STATE_MENU;
}

void SceneGame::loadMedia() {
	this->bloop = ResourceManager::Instance()->GetAudio(PATH_BLOOP);
	this->laser = ResourceManager::Instance()->GetAudio(PATH_LASER);
	// this->font = ResourceManager::Instance()->GetFont(PATH_FONT, FONT_SIZE);
	
	if (this->bloop == NULL || this->laser == NULL) //|| this->font == NULL)
		this->quit = true;
}

void SceneGame::init(olc::PixelGameEngine* app) {
	this->app = app;
	olc::GamePad::init();
	this->loadMedia();
	this->tank.init();
	this->invader.init();
	this->invaderBullet.init();
	this->tankBullet.init();
	this->shield.init();

	this->initGame();

	std::vector<olc::GamePad*> gamePads = olc::GamePad::getGamepads();
	if (gamePads.size() > 0)
	{
		//this->joystick = new olc::GamePad(*gamePads[0]);
		this->joystick = gamePads[0];
	}
	else
	{
		this->joystick = nullptr;
	}
}

void SceneGame::initGame()
{
	this->points = 0;
	this->isGameOver = false;
	this->pause = false;
	this->quit = false;
	this->left = false;
	this->right = false;
	this->dropDistance = 0.0;
	this->nextDirection = 0.0;
	this->invaderShootDelay = INVADER_SHOOT_DELAY_MIN + (double)(rand() % INVADER_SHOOT_RANDOM);	


	if (this->tank.length() > 0)
		this->tank.destroy();
	this->initTank();

	if (this->invaderBullet.length() > 0)
		this->invaderBullet.destroy();

	if (this->tankBullet.length() > 0)
		this->tankBullet.destroy();

	if (this->invader.length() > 0)
		this->invader.destroy();
	this->initInvaders();

	int w, h;
	olc::vi2d win = app->GetWindowSize();
	int x, y;
	this->tank[0]->getHitBox(&x, &y, &w, &h);
	if (this->shield.length() > 0)
		this->shield.destroy();
	this->initShields((int)(win.y - (2.5 * h)));	
}

void SceneGame::destroy() {
	this->tank.destroy();
	this->invader.destroy();
	this->invaderBullet.destroy();
	this->tankBullet.destroy();
	this->shield.destroy();
}

void SceneGame::update(double ms, olc::imgui::PGE_ImGUI* gui) {
	double fps = 1000.0 / ms;
	olc::vi2d win = app->GetWindowSize();
	bool showGUI = this->pause || this->isGameOver || this->invader.length() == 0;
	bool left = false, right = false;

	if (app->GetKey(olc::Key::ESCAPE).bReleased)
	{
		this->quit = true;
	}
	// Joystick
	olc::HWButton j_fire, j_left, j_right;
	float lx = 0.0, ly = 0.0;

	if (this->joystick != nullptr)
	{
		j_fire = joystick->getButton(olc::GPButtons::FACE_D);
		j_left = joystick->getButton(olc::GPButtons::DPAD_L);
		j_right = joystick->getButton(olc::GPButtons::DPAD_R);
		lx = joystick->getAxis(olc::GPAxes::LX);
		ly = joystick->getAxis(olc::GPAxes::LY);
	}
	else
	{
		this->joystick = olc::GamePad::selectWithButton(olc::GPButtons::SELECT);
	}
	
	// Key Up
	if (app->GetKey(olc::Key::SPACE).bReleased || (this->joystick != nullptr && j_fire.bReleased))
	{
		if (!showGUI)
		{
			this->spawnTankBullet();
		}
	}
	if (app->GetKey(olc::Key::P).bReleased)
	{
		this->pause = !this->pause;
	}

	// Key Down
	if (app->GetKey(olc::Key::A).bHeld || app->GetKey(olc::Key::LEFT).bHeld || (this->joystick != nullptr && (j_left.bHeld || lx < -1.0 * JOYSTICK_DEAD_ZONE)))
	{
		this->left = true;
	}
	else
	{
		this->left = false;
	}
	if (app->GetKey(olc::Key::D).bHeld || app->GetKey(olc::Key::RIGHT).bHeld || (this->joystick != nullptr && (j_right.bHeld || lx > JOYSTICK_DEAD_ZONE)))
	{
		this->right = true;
	}
	else
	{
		this->right = false;
	}
		

	olc::vi2d mousePos = this->app->GetWindowMouse(); // GetMousePos();
	olc::HWButton btnLeft = this->app->GetMouse(olc::Mouse::LEFT);
	olc::HWButton btnRight = this->app->GetMouse(olc::Mouse::RIGHT);

	if (!showGUI) {
		this->invader.removeInactive();
		this->invader.update(ms);
		this->invaderPostUpdate(ms);
		this->invader.collisionDetect(&this->tankBullet, true);		

		this->tank.update(ms);
		((Tank*)this->tank[0])->setInput(this->left, this->right);
		this->tank.collisionDetect(&this->invader, false);
		this->tank.collisionDetect(&this->invaderBullet, false);

		this->invaderBullet.removeInactive();
		this->invaderBullet.update(ms);

		this->tankBullet.removeInactive();	
		this->tankBullet.update(ms);

		this->shield.removeInactive();
		this->shield.update(ms);
		this->shield.collisionDetect(&this->tankBullet, true);
		this->shield.collisionDetect(&this->invader, false);
		this->shield.collisionDetect(&this->invaderBullet, true);
	}	
	
	if (showGUI) {
		// Draw a pause message box
		if (this->pause)
		{
			gui->ImGui_ImplPGE_NewFrame();
			ImGui::SetNextWindowPos(ImVec2((float)(win.x / 3.0), (float)(win.y / 3.0)), ImGuiCond_::ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2((float)(win.x / 3.0), (float)(win.y / 8.0)), ImGuiCond_::ImGuiCond_Once);
			ImGui::Begin("Pause");
			ImGui::Text("Paused...");
			if (ImGui::Button("OK")) {
				this->pause = false;
			}
			ImGui::End();
			ImGui::EndFrame();
		}
		if (this->isGameOver)
		{
			gui->ImGui_ImplPGE_NewFrame();
			ImGui::SetNextWindowPos(ImVec2((float)(win.x / 3.0), (float)(win.y / 3.0)), ImGuiCond_::ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2((float)(win.x / 3.0), (float)(win.y / 5.0)), ImGuiCond_::ImGuiCond_Once);
			ImGui::Begin("Game Over");
			ImGui::Text("Game Over.");
			ImGui::Text("Would you like to play again ? ");
			if (ImGui::Button("Yes")) {
				this->initGame();
			}
			if (ImGui::Button("No")) {
				this->quit = true;
			}
			ImGui::End();
			ImGui::EndFrame();
		}
		else if (this->invader.length() == 0)
		{
			gui->ImGui_ImplPGE_NewFrame();
			ImGui::SetNextWindowPos(ImVec2((float)(win.x / 4.0), (float)(win.y / 3.0)), ImGuiCond_::ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2((float)(win.x / 2.0), (float)(win.y / 6.0)), ImGuiCond_::ImGuiCond_Once);
			ImGui::Begin("Next Level");
			ImGui::Text("Congratulations!");
			ImGui::Text("You have destroyed a wave of the Great Machine Armada.");
			if (ImGui::Button("Next Level")) {
				int score = this->points; // preserve the score.
				this->initGame();
				this->points = score;
			}
			ImGui::End();
			ImGui::EndFrame();
		}
		ImGui::Render();

	}
	olc::Pixel bg;
	bg.a = 255;
	bg.r = bg.g = 8;
	bg.b = 32;
	this->app->Clear(bg);

	//Apply the image
	app->SetPixelMode(olc::Pixel::ALPHA);
	this->invader.draw();
	this->invaderBullet.draw();
	this->tankBullet.draw();
	this->tank.draw();
	this->shield.draw();
	
	if (this->showFPS) {
		this->drawFPS(fps);
	}
	this->drawScore();
	app->SetPixelMode(olc::Pixel::NORMAL);
}

bool SceneGame::running() {
	return !this->quit;
}

void SceneGame::spawnTankBullet()
{
	if (this->tankBullet.length() == 0)
	{
		Tank* tank = ((Tank*)this->tank[0]);
		int turretX, turretY;
		tank->getTurretPosition(&turretX, &turretY);
		Bullet* bullet = new Bullet();
		bullet->init(app);
		bullet->setDirection(-1);
		int bulletX, bulletY, bulletW, bulletH;
		bullet->setPosition(0.0, 0.0);
		bullet->getHitBox(&bulletX, &bulletY, &bulletW, &bulletH);
		turretX -= (bulletW / 2);
		turretY -= bulletH;
		bullet->setPosition(turretX, turretY);
		this->tankBullet.add((Sprite*)bullet);
		olc::SOUND::PlaySample(this->bloop, false);
	}
}

void SceneGame::initInvaders()
{
	this->invader.destroy();

	for (int j = 0; j < MAX_ROWS; ++j)
	{
		for (int i = 0; i < MAX_COLS; ++i)
		{
			Invader* invader = new Invader();
			invader->init(this->app);
			invader->setParent(this);
			invader->setType(j, i);
			this->invader.add((Sprite*)invader);
		}
	}

}

void SceneGame::initShields(int y)
{
	const int NUM_SHIELDS = 4;
	bool skip;
	int x, temp, iStep = 0, jStep = 0;
	int corner_w, corner_h;
	Shield* chunk;
	double x1, y1;

	olc::vi2d win = app->GetWindowSize();

	int shield_step = win.x / (4 * NUM_SHIELDS);
	int shield_w = shield_step * 2;
	int shield_h = shield_step;
	corner_w = (shield_w / 4);
	corner_h = (shield_h / 4);
	chunk = new Shield();
	chunk->init(this->app);
	chunk->getHitBox(&x, &temp, &iStep, &jStep);
	delete chunk;
	x = shield_step;
	double slope = ((double)corner_h / (double)corner_w);

	for (int k = 0; k < NUM_SHIELDS; k++)
	{
		for (int j = 0; j < shield_h; j += jStep)
		{
			y1 = (double)j + (double)(jStep / 2.0);
			for (int i = 0; i < shield_w; i += iStep)
			{
				x1 = (double)i + (double)(iStep / 2.0);
				skip = false;
				if ((i <= corner_w) && (j <= corner_h) && (((slope * x1) - corner_h)  * -1 >= y1)) {
					skip = true;
				}
				else if ((i >= shield_w - corner_w) && (j <= corner_h) && ((slope * (shield_w - x1)) - corner_h) * -1 >= y1) {
					skip = true;
				}
				else if ((i >= corner_w) && (i <= (shield_w - corner_w)) && (j >= (shield_h - corner_h))) {
					skip = true;
				}

				if (!skip)
				{
					chunk = new Shield();
					chunk->init(this->app);
					chunk->setPosition((double)(x + i), (double)(y + j));
					this->shield.add((Sprite*)chunk);
				}
			}
		}
		x += (shield_step * 4);
	}
}

void SceneGame::initTank()
{
	Tank* tank = new Tank();
	tank->init(this->app);
	tank->setParent(this);
	this->tank.add(tank);

}

void SceneGame::drawFPS(double fps)
{
	// Write the fps
	char buffer[50];
	std::snprintf(buffer, 50, "fps: %3.2f", fps);
	olc::Pixel fg = { 255, 255, 255 };
	olc::vi2d win = app->GetWindowSize();
	std::string temp = std::string(buffer);
	std::u32string strBuffer(temp.begin(), temp.end());	
	olc::FontRect size = this->font->GetStringBounds(strBuffer, 0.0f);
	olc::vi2d pos = { win.x - size.size.x - 1, win.y - size.size.y - 1 };
	olc::Sprite* sprite = this->font->RenderStringToSprite(strBuffer, fg);
	this->app->DrawSprite(pos, sprite);
	delete sprite;

}

void SceneGame::drawScore()
{
	// Write the score
	char buffer[50];
	std::snprintf(buffer, 50, "Score: %d", this->points);
	olc::Pixel fg = olc::WHITE; // { 255, 255, 255 };
	olc::vi2d win = app->GetWindowSize();
	std::string temp = std::string(buffer);
	std::u32string strBuffer(temp.begin(), temp.end());

	olc::FontRect size = this->font->GetStringBounds(strBuffer, 0.0f);
	olc::vf2d pos = { (float)win.x - size.size.x - 1, (float)0 };
	olc::Sprite* sprite = this->font->RenderStringToSprite(strBuffer, fg);
	this->app->DrawSprite(pos, sprite);
	delete sprite;
}


void SceneGame::addPoints(int points)
{
	this->points += points;
}

double SceneGame::getInvaderSpeed()
{
	const int SPEED_PER_INVADER = 6; // pixels per second
	return (int)40 + (((MAX_COLS * MAX_ROWS) - this->invader.length()) * SPEED_PER_INVADER); 
}

void SceneGame::invaderPostUpdate(double ms)
{
	double dist = (this->getInvaderSpeed() * ms) / 1000.0;
	if (this->dropDistance > 0.0)
	{
		this->dropDistance -= dist;
		if (this->dropDistance <= 0.0)
		{
			this->dropDistance = 0.0;

			for (int i = 0; i < this->invader.length(); ++i)
			{
				Invader* enemy = (Invader*)this->invader[i];
				if (enemy->getActive() && enemy->getVisible())
				{
					enemy->setDirection(this->nextDirection, 0.0);
				}
			}
			this->nextDirection = 0.0;
		}
	} 
	else
	{
		int x, y, w, h;
		double dx, dy;
		olc::vi2d win = app->GetWindowSize();
		for (int i = 0; i < this->invader.length(); ++i)
		{
			Invader* enemy = (Invader*)this->invader[i];
			if (enemy->getActive() && enemy->getVisible())
			{				
				enemy->getHitBox(&x, &y, &w, &h);
				enemy->getDirection(&dx, &dy);
				if (dx <= 0.0 && x <= 0)
				{
					for (int j = 0; j < this->invader.length(); ++j)
					{
						((Invader*) this->invader[j])->setDirection(0.0, 1.0);
					}
					this->dropDistance = DROP_TOTAL;
					this->nextDirection = 1.0;
					break;
				}
				else if (dx >= 0.0 && x >= (win.x - w - 1))
				{
					for (int j = 0; j < this->invader.length(); ++j)
					{
						((Invader*)this->invader[j])->setDirection(0.0, 1.0);
					}
					this->dropDistance = DROP_TOTAL;
					this->nextDirection = -1.0;
					break;
				}
			}
		}
	}
	this->invaderShootDelay -= ms;
	if (this->invaderShootDelay <= 0.0)
	{
		this->invaderShootDelay = INVADER_SHOOT_DELAY_MIN + (double)(rand() % INVADER_SHOOT_RANDOM);
		// pick an invader.
		if (this->invader.length() > 0 && this->invaderBullet.length() == 0)
		{
			Invader* invader = ((Invader*)this->invader[rand() % this->invader.length()]);
			int turretX, turretY;
			invader->getTurretPosition(&turretX, &turretY);
			turretY += 1;
			Bullet* bullet = new Bullet();
			bullet->init(this->app);
			bullet->setDirection(1);
			int bulletX, bulletY, bulletW, bulletH;
			bullet->setPosition(0.0, 0.0);
			bullet->getHitBox(&bulletX, &bulletY, &bulletW, &bulletH);
			turretX -= (bulletW / 2);
			bullet->setPosition(turretX, turretY);
			this->invaderBullet.add((Sprite*)bullet);
			olc::SOUND::PlaySample(this->laser, false);
		}
	}
}

void SceneGame::gameOver()
{
	this->isGameOver = true;
}