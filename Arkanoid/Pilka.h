#pragma once
#include"Block.h"
#define SCREEN_WIDTH  976
#define SCREEN_HEIGHT  582

class Pilka
{

public:
	float promien;
	float x, y;
	float vx, vy;
	float speed;


	Pilka()
	{
		x = SCREEN_WIDTH / 2 - 50;
		y = SCREEN_HEIGHT / 2;
		speed = 0;
		vx = speed;
		vy = speed;

		promien = 15;

	}

	void set_speed(int s)
	{
		speed = s;
		vx = vy = speed;
	}


	bool collision(Pilka* ball, Block* blocks[], int n)
	{
		for (int i = 0; i < n; i++)
		{
			///Sprawdzenie czy pi³ka dotknê³a bloku
			if (ball->x + ball->promien >= blocks[i]->x &&
				ball->x - ball->promien <= blocks[i]->x2 &&
				ball->y + ball->promien >= blocks[i]->y &&
				ball->y - ball->promien <= blocks[i]->y2)
			{
				///Sprawdzenie z której strony blok zostal dotkniêty góra/dó³/lewo/prawo
				if (ball->x < blocks[i]->x || ball->x > blocks[i]->x2)
				{
					ball->vx *= (-1);
				}
				else
				{
					ball->vy *= (-1);
				}

				delete(blocks[i]);
				return true;
			}

		}

		return false;
	}


};

