#pragma once
#include"Block.h"
#include"Paletka.h"
#define SCREEN_WIDTH 976
#define SCREEN_HEIGHT 614

class Pilka
{

public:
	float promien;
	float x, y;
	float vx, vy;
	float speed;
	int points = 0;


	Pilka();

	void set_speed(int s);

	bool collision(Pilka* ball, Block* blocks[], int n, int points);

	void odbijanie(Pilka* ball, Paletka* p, Block* blocks[], int n, Block* health[], int* z);


};

