#pragma once
#ifndef __INVADER_H__
#define __INVADER_H__

#include "sprite.h"
#include "../scene/game.h"

class Invader : public Sprite
{
public:
	Invader();
	~Invader();
	void init(olc::PixelGameEngine* app);
	void destroy();
	void draw();
	void update(double ms);
	void setDirection(double dx, double dy);
	void setPosition(double x, double y);
	void setParent(SceneGame* parent) { this->parent = parent; };
	void getHitBox(int* x, int* y, int* w, int* h);
	void getDirection(double* dx, double* dy) { *dx = this->dx; *dy = this->dy; };
	void getTurretPosition(int* x, int* y);
	void hit();	
	void setType(int row, int column);
private:
	olc::PixelGameEngine* app;
	SceneGame* parent;
	olc::Sprite* image;
	int points;
	double x, y;
	int w, h;
	double dx, dy;
};

#endif