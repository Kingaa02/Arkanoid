#pragma once
class Block
{
public:
	float x = 0, x2 = 0;
	float y = 0, y2 = 0;
	float points = 0;

	Block(int x)
	{
		this->x = x;
		x2 = this->x + 50;
		y = 200;
		y2 = y + 20;

	}

	Block()
	{

	}

};

