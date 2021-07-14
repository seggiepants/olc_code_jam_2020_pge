#pragma once
#ifndef __TANK_H__
#define __TANK_H__

#include "sprite.h"
#include "../scene/game.h"

class Tank : public Sprite
{
public:
	Tank();
	~Tank();
	void init(olc::PixelGameEngine* app);
	void destroy();
	void draw();
	void update(double ms);
	void setInput(bool left, bool right) { this->left = left; this->right = right; }
	void setParent(SceneGame* parent) { this->parent = parent; };
	void setPosition(double x, double y);
	void getHitBox(int* x, int* y, int* w, int* h);
	void getTurretPosition(int* x, int* y);
	void hit();	
private:
	olc::PixelGameEngine* app;
	ResourceManager* resource;
	olc::Sprite* image;
	SceneGame* parent;
	double x, y;
	bool left, right;
};

#endif