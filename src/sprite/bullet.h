#pragma once
#ifndef __BULLET_H__
#define __BULLET_H__

#include "sprite.h"
#include "../resource_manager.h"

class Bullet : public Sprite
{
public:
	Bullet();
	~Bullet();
	void init(olc::PixelGameEngine* app) ;
	void destroy();
	void draw();
	void update(double ms);
	int getDirection();
	void setDirection(int direction);
	void setPosition(double x, double y);
	void getHitBox(int* x, int* y, int* w, int* h) ;
	void hit();
private:
	olc::PixelGameEngine* app;
	olc::Sprite* image;
	int direction;
	double x, y;
	int w, h;
};

#endif