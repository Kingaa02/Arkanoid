#pragma once
#include"Pilka.h"

#define SCREEN_WIDTH  976
class Paletka
{
public:
	float x = 0, x2 = 0, y = 0, y2 = 0;
	float dx = 0, dy = 0;

	Paletka()
	{
		dx = 10;
		x = SCREEN_WIDTH / 2 - 100;
		x2 = x + 100;
		y = 400;
		y2 = y + 20;
	}


};

