#include "Pilka.h"
#include <cmath>

Pilka::Pilka()
{
	x = SCREEN_WIDTH / 2 - 50;
	y = SCREEN_HEIGHT / 2;
	speed = 0;
	vx = speed;
	vy = speed;

	promien = 10;
}

void Pilka::set_speed(int s)
{
	speed = s;
	vx = vy = speed;
}

bool Pilka::collision(Pilka* ball, Block* blocks[], int n, int points)
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
			ball->points += 10;
			ball->blocksDestroyed += 1;
			return true;
		}

	}

	return false;
}


void Pilka::odbijanie(Pilka* ball, Paletka* p, Block* blocks[], int n, Block* health[], int* z)
{

	ball->x += ball->vx;
	ball->y += ball->vy;

	///Kolizja pi³ki z lew¹ stron¹ ekranu
	if (ball->x - ball->promien <= 0)
	{
		ball->vx *= (-1);
	}

	///Kolizja pi³ki z praw¹ stron¹ ekranu
	if (ball->x + ball->promien >= SCREEN_WIDTH)
	{
		ball->vx *= (-1);
	}
	///Kolizja pi³ki z do³em stron¹ ekranu
	if (ball->y + ball->promien >= SCREEN_HEIGHT)
	{
		ball->x = SCREEN_WIDTH / 2 - 50;
		ball->y = SCREEN_HEIGHT / 2;
		ball->set_speed(0);
		if (*z >= 0)
		{
			delete(health[*z]);
			*z -= 1;
		}

	}
	///Kolizja pi³ki z gór¹ stron¹ ekranu
	if (ball->y - ball->promien <= 0)
	{
		ball->vy *= (-1);
	}



	// Kolizja piłki z paletką
	if (ball->y + ball->promien > p->y && ball->y + ball->promien < p->y2)
	{
		if ((ball->x + ball->promien > p->x && ball->x + ball->promien < p->x2) ||
			(ball->x - ball->promien > p->x && ball->x - ball->promien < p->x2) ||
			(ball->x + ball->promien > p->x2 && ball->x - ball->promien < p->x))
		{

			// Obliczanie względnego położenia piłki na paletce
			float relativePosition = (ball->x - p->x) / p->width;

			// Dostosowanie kąta odbicia na podstawie względnej pozycji
			const float maxReflectionAngle = 60.0f;  // Maksymalny kąt odbicia w stopniach
			float reflectionAngle = relativePosition * (2 * maxReflectionAngle) - maxReflectionAngle;
			float reflectionRadians = reflectionAngle * 3.14159f / 180.0f;

			// Aktualizowanie prędkości piłki na podstawie kąta odbicia, zachowując prędkość
			float currentSpeed = sqrt(ball->vx * ball->vx + ball->vy * ball->vy);
			ball->vx = currentSpeed * sin(reflectionRadians);
			ball->vy = -currentSpeed * cos(reflectionRadians);
		}
	}
	ball->collision(ball, blocks, n, points);

}

