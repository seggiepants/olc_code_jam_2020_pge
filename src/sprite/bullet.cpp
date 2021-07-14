#include <string>
#include "bullet.h"

const int DIRECTION_UP = -1;
const int DIRECTION_DOWN = 1;
const double BULLET_SPEED = 300.0;

const std::string PATH_BULLET_UP = "assets/image/tank_bullet.png";
const std::string PATH_BULLET_DOWN = "assets/image/laser.png";

Bullet::Bullet() : Sprite()
{
	this->app = NULL;
	this->image = NULL;
	this->direction = DIRECTION_DOWN;
	this->w = this->h = 0;
	this->x = this->y = 0.0;

}

Bullet::~Bullet() 
{
	this->destroy();
}

void Bullet::init(olc::PixelGameEngine* app)
{
	this->app = app;
	this->image = NULL;
	this->active = true;
	this->visible = true;
	this->setDirection(DIRECTION_DOWN);
}

void Bullet::destroy()
{
	this->image = NULL;
}

void Bullet::draw()
{
	if (this->active && this->getVisible() && this->image != NULL)
	{
		app->DrawSprite({ (int) this->x, (int)this->y }, this->image);
	}
}

void Bullet::update(double ms)
{
	olc::vi2d size = app->GetWindowSize();
	int winW = size.x, winH = size.y;
	
	if (this->active)
	{
		this->y += (BULLET_SPEED * ms * this->direction) / 1000.0;
		if ((this->y + this->h < 0) || (this->y >= winH))
		{
			this->setActive(false);
		}
	}
}

int Bullet::getDirection()
{
	return this->direction;
}

void Bullet::setDirection(int direction)
{
	olc::Sprite* sprite;
	if (direction < 0) 
	{
		this->direction = DIRECTION_UP;
		sprite = ResourceManager::Instance()->GetImage(PATH_BULLET_UP);
	}
	else if (direction == 0)
	{
		this->direction = 0;
		sprite = ResourceManager::Instance()->GetImage(PATH_BULLET_UP);
	}
	else
	{
		this->direction = DIRECTION_DOWN;
		sprite = ResourceManager::Instance()->GetImage(PATH_BULLET_DOWN);
	}
	this->w = sprite->width;
	this->h = sprite->height;
	this->image = sprite;
}

void Bullet::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Bullet::getHitBox(int* x, int* y, int* w, int* h)
{
	*x = (int)this->x;
	*y = (int)this->y;
	*w = this->w;
	*h = this->h;
}

void Bullet::hit()
{
	this->setActive(false);
}
