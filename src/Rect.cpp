#include "Rect.h"

bool Intersect(Rect* first, Rect* second)
{
	// Right is before Left of other
	if (first->x + first->w < second->x) return false;
	// Left is after Right of other.
	if (first->x > second->x + second->w) return false;
	// Bottom is above top of other.
	if (first->y + first->h < second->y) return false;
	// Top is below bottom of other.
	if (first->y > second->y + second->h) return false;
	// Must be intersecting then.
	return true;
}