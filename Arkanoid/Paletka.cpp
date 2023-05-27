#include "Paletka.h"

Paletka::Paletka()
{
	dx = 10;
	x = SCREEN_WIDTH / 2 - 100;
	x2 = x + 100;
	y = 550;
	y2 = y + 20;
	width = x2 - x;
}
