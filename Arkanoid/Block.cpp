#include "Block.h"

///Klasa bloku
Block::Block(int x)
{
	this->x = x - 140;
	x2 = this->x + 65;
	y = 200;
	y2 = y + 20;
	hitCount = 0;
}
