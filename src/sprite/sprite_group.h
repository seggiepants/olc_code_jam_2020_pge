#pragma once
#ifndef __SPRITE_GROUP_H__
#define __SPRITE_GROUP_H__

#include <vector>
#include "../resource_manager.h"
#include "sprite.h"

class SpriteGroup {
public:
	SpriteGroup();
	~SpriteGroup();
	void init();
	void destroy();
	void draw();
	void update(double ms);
	void add(Sprite* item);
	void removeInactive();
	Sprite* operator[](int index);
	int length();
	bool collisionDetect(SpriteGroup* other, bool hitOther);
	std::vector<Sprite*>::iterator begin();
	std::vector<Sprite*>::iterator end();
	
private:
	std::vector<Sprite*> sprites;
};

#endif