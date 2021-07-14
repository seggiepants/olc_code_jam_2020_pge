#pragma once

#ifndef __SHEILD_H__
#define __SHIELD_H_

#include "sprite.h"

class Shield : public Sprite
{
public:
	Shield();
	~Shield();
	void init(olc::PixelGameEngine* app);
	void destroy();
	void update(double ms);
	void draw();
	void getHitBox(int* x, int* y, int* w, int* h);
	void setPosition(double x, double y);
	void hit();
private:
	olc::PixelGameEngine* app;
	ResourceManager* resource;
	olc::Sprite* image;
	double x, y;
	int w, h;
};

#endif