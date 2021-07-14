#include <iostream>
#include <string>
#include "invader.h"

const int MAX_ROWS = 5;
const int MAX_COLS = 10;
const std::string COLOR_BLUE = "Blue";
const std::string COLOR_GREEN = "Green";
const std::string COLOR_RED = "Red";
const int POINTS_BLUE = 10;
const int POINTS_GREEN = 20;
const int POINTS_RED = 30;


Invader::Invader()
{
	this->app = NULL;
	this->image = NULL;
	this->dx = 1.0;
	this->dy = 0.0;
}

Invader::~Invader()
{
	this->destroy();
}

void Invader::init(olc::PixelGameEngine* app)
{
	this->image = NULL;
	this->app = app;

	this->dx = 1.0;
	this->dy = 0.0;
}

void Invader::destroy()
{
	this->image = NULL;
}

void Invader::draw()
{	
	if (this->active && this->getVisible() && this->image != NULL)
	{
		app->DrawSprite({ (int)this->x, (int)this->y }, this->image);
	}
}

void Invader::update(double ms)
{
	int winW, winH;
	if (this->active)
	{
		double speed = this->parent->getInvaderSpeed();
		olc::vi2d size = app->GetWindowSize();
		this->x += (speed * ms * this->dx) / 1000.0;
		this->y += (speed * ms * this->dy) / 1000.0;
	}
}

void Invader::setDirection(double dx, double dy)
{
	this->dx = dx;
	this->dy = dy;
}

void Invader::setPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Invader::getHitBox(int* x, int* y, int* w, int* h)
{
	*x = (int) this->x;
	*y = (int) this->y;
	*w = this->w;
	*h = this->h;
}

void Invader::getTurretPosition(int* x, int* y)
{
	*x = (int)(this->x + (this->w / 2));
	*y = (int)(this->y + this->h + 1);
}

void Invader::hit()
{
	this->setActive(false);
	this->parent->addPoints(this->points);
}

void Invader::setType(int row, int column)
{
	const int MAX_TYPES = 5;
	char buffer[100];
	if (row >= 0 && row < MAX_ROWS && column >= 0 && column < MAX_COLS)
	{
		switch (row)
		{
		case 0:
			snprintf(buffer, 100, "assets/image/enemy%s%d.png", COLOR_RED.c_str(), (column % MAX_TYPES) + 1);
			this->points = POINTS_RED;
			break;
		case 1:
		case 2:
			snprintf(buffer, 100, "assets/image/enemy%s%d.png", COLOR_GREEN.c_str(), (column % MAX_TYPES) + 1);
			this->points = POINTS_GREEN;
			break;
		default:
			snprintf(buffer, 100, "assets/image/enemy%s%d.png", COLOR_BLUE.c_str(), (column % MAX_TYPES) + 1);
			this->points = POINTS_BLUE;
			break;
		}
		olc::Sprite* sprite = ResourceManager::Instance()->GetImage(std::string(buffer));
		this->w = sprite->width;
		this->h = sprite->height;
		this->image = sprite;

	}
	else 
	{
		std::cout << "Incorrect invader type row: " << row << ", column:  " << column << std::endl;
		this->image = NULL;
	}

	if (this->image != NULL)
	{
		olc::vi2d win = app->GetWindowSize();
		int spacer_w = this->w / 8;
		int spacer_h = this->h / 8;
		int invader_w = (w * MAX_COLS) + ((MAX_COLS - 1) * spacer_w);
		int offset_x = (win.x - invader_w) / 2;
		int offset_y = h;
		this->x = offset_x + (column * spacer_w) + (column * w);
		this->y = offset_y + (row * spacer_h) + (row * h);
	}
	else
	{
		this->x = 0.0;
		this->y = 0.0;
	}

}