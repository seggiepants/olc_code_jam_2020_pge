#pragma once
#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__
typedef struct
{
	int x;
	int y;
	int w;
	int h;
} Rect;

bool Intersect(Rect* first, Rect* second);

#endif
