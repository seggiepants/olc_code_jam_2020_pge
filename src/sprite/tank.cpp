#include <algorithm>
#include <iostream>
#include <string>
#include "tank.h"

const int MOVE_SPEED = 400; // pixels per second.

const std::string PATH_TANK = "assets/image/tank.png";

Tank::Tank()
{
	this->x = this->y = 0;
	this->app = NULL;
	this->resource = NULL;
	this->image = NULL;
	this->parent = NULL;
	this->left = this->right = false;
	this->image = NULL;
}

Tank::~Tank()
{
	this->destroy();
}

void Tank::init(olc::PixelGameEngine* app)
{
	this->app = app;
	this->image = ResourceManager::Instance()->GetImage(PATH_TANK);
	this->left = this->right = false;

	int w, h, winW, winH;
	olc::vi2d winSize = app->GetWindowSize();
	winW = winSize.x;
	winH = winSize.y;
	w = this->image->width;
	h = this->image->height;
	this->x = (winW - w) / 2;
	this->y = winH - (1.5 * h);
}

void Tank::destroy()
{
	// do nothing
	this->image = NULL;
}

void Tank::draw()
{
	if (this->active && this->getVisible() && this->image != NULL)
	{
		olc::vi2d pos = { (int32_t) this->x, (int32_t) this->y };
		app->DrawSprite(pos, this->image);
	}
}

void Tank::update(double ms)
{
	double delta;
	double step;

	if (this->active)
	{
		step = (MOVE_SPEED * ms) / 1000.0;
		int w, h, winW, winH;
		w = this->image->width;
		h = this->image->height;
		olc::vi2d size = app->GetWindowSize();
		winW = size.x;
		winH = size.y;

		if (this->left) {
			this->x = (double)std::max<double>(0.0, this->x - step);
		}
		if (this->right) {
			delta = (double)(winW - w - 1);
			this->x = (double)std::min<double>(delta, this->x + step);
		}		
	}
}

void Tank::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Tank::getHitBox(int* x, int* y, int* w, int* h)
{
	*x = (int) this->x;
	*y = (int) this->y;
	*w = this->image->width;
	*h = this->image->height;
}

void Tank::getTurretPosition(int* x, int* y)
{
	int w, h;
	w = this->image->width;
	h = this->image->height;
	*x = (int)(this->x + (w / 2));
	*y = (int)(this->y - 1);
}

void Tank::hit()
{
	this->parent->gameOver();
}