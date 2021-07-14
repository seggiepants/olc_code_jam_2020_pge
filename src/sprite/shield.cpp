#include <string>
#include "shield.h"

const std::string PATH_SHIELD = "assets/image/shield.png";

Shield::Shield() : Sprite()
{
	this->app = NULL;
	this->image = NULL;
	this->x = this->y = 0.0;
}

Shield::~Shield()
{
	this->destroy();
}

void Shield::init(olc::PixelGameEngine* app)
{
	this->app = app;
	this->image = NULL;
	this->active = true;
	this->visible = true;
	olc::Sprite* sprite = ResourceManager::Instance()->GetImage(PATH_SHIELD);
	this->w = sprite->width;
	this->h = sprite->height;
	this->image = sprite;
}

void Shield::destroy()
{
	this->image = NULL;
}

void Shield::draw()
{
	if (this->active && this->getVisible() && this->image != NULL)
	{
		app->DrawSprite({ (int) this->x, (int) this->y }, this->image);
	}
}

void Shield::update(double ms)
{
	// Do nothing
}

void Shield::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Shield::getHitBox(int* x, int* y, int* w, int* h)
{
	*x = (int)this->x;
	*y = (int)this->y;
	*w = this->w;
	*h = this->h;
}

void Shield::hit()
{
	this->setActive(false);
}
