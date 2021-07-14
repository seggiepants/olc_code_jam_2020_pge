#pragma once
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "../3rd_party/olc/olcPixelGameEngine.h"
#include "../resource_manager.h"
#include "../Rect.h"

class Sprite
{
public:
	Sprite() {
		this->active = true;
		this->visible = true;
	};
	virtual ~Sprite() {};
	virtual void init(olc::PixelGameEngine* app) = 0;	
	virtual void destroy() = 0;
	virtual void draw() = 0;
	virtual void update(double ms) = 0;
	virtual void getHitBox(int* x, int* y, int* w, int* h) = 0;
	bool collisionDetect(Sprite* other)
	{
		Rect src, target;
		this->getHitBox(&src.x, &src.y, &src.w, &src.h);
		other->getHitBox(&target.x, &target.y, &target.w, &target.h);
		return Intersect(&src, &target);
	};
	virtual void hit() = 0;
	bool getActive() 
	{ 
		return this->active; 
	};
	void setActive(bool value) 
	{
		this->active = value;
	};
	bool getVisible()
	{
		return this->visible;
	};
	void setVisible(bool value)
	{
		this->visible = value;
	};
protected:
	bool active;
	bool visible;
};

#endif