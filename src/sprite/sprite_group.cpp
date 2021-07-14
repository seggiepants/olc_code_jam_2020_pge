#include "sprite_group.h"
#include <iostream>

SpriteGroup::SpriteGroup()
{
	this->destroy();
}

SpriteGroup::~SpriteGroup()
{
	this->destroy();
}

void SpriteGroup::init()
{
	this->destroy();
}

void SpriteGroup::destroy()
{
	if (this->sprites.size() > 0)
	{
		for (const auto &element : this->sprites) {
			delete element;
		}
		this->sprites.clear();
	}
}

void SpriteGroup::draw()
{
	if (this->sprites.size() > 0)
	{
		for (const auto &element : this->sprites) {
			if (element->getActive() && element->getVisible())
			{
				element->draw();
			}
		}
	}
}

void SpriteGroup::update(double ms)
{
	for (const auto &element : this->sprites) {
		if (element->getActive())
		{
			element->update(ms);
		}
	}
}

void SpriteGroup::add(Sprite* item)
{
	this->sprites.push_back(item);
}

Sprite* SpriteGroup::operator[](int index)
{
	if ((index >= 0) && (index < (int)this->sprites.size())) {
		return this->sprites[index];
	} 
	else
	{
		return NULL;
	}
}

int SpriteGroup::length()
{
	return (int) this->sprites.size();
}

std::vector<Sprite*>::iterator SpriteGroup::begin()
{
	return this->sprites.begin();
}

std::vector<Sprite*>::iterator SpriteGroup::end()
{
	return this->sprites.end();
}

bool SpriteGroup::collisionDetect(SpriteGroup* other, bool hitOther = true)
{
	bool hit, ret;
	ret = false;

	for (const auto &current : this->sprites) {
		hit = false;
		for (auto target = other->begin(); target < other->end(); target++) {
			if (current->collisionDetect(*target))
			{
				ret = true;
				hit = true;
				current->hit();
				if (hitOther)
					(*target)->hit();
				break;
			}
		}
	}
	return ret;
}

void SpriteGroup::removeInactive()
{
	std::vector<Sprite*>::iterator i = this->sprites.begin();
	while (i != this->sprites.end()) 
	{ 
		if (!(*i)->getActive())
		{
			i = this->sprites.erase(i);
		}
		else
		{
			++i;
		}
	}
}