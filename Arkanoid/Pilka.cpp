#include "Pilka.h"
#include <cmath>

///Klasa pilki
Pilka::Pilka()
{
	x = SCREEN_WIDTH / 2 - 50;
	y = SCREEN_HEIGHT / 2;
	speed = 0;
	vx = speed;
	vy = speed;

	promien = 10;
}

///Szybkosc pilki
void Pilka::set_speed(int s)
{
	speed = s;
	vx = vy = speed;
}

///Kolizje pilki z blokami
bool Pilka::collision(Pilka* ball, Block* blocks[], int n, int points, int level)
{
	for (int i = 0; i < n; i++)
	{
		// Sprawdzenie czy pilka dotknela bloku
		if (ball->x + ball->promien >= blocks[i]->x &&
			ball->x - ball->promien <= blocks[i]->x2 &&
			ball->y + ball->promien >= blocks[i]->y &&
			ball->y - ball->promien <= blocks[i]->y2)
		{
			blocks[i]->setHitCount(blocks[i]->getHitCount() + 1);

			// Sprawdzenie z ktorej strony blok zostal dotkniety gora/dol/lewo/prawo
			bool isCollisionFromTop = ball->y - ball->promien <= blocks[i]->y2 && ball->vy > 0;
			bool isCollisionFromBottom = ball->y + ball->promien >= blocks[i]->y && ball->vy < 0;
			bool isCollisionFromLeft = ball->x + ball->promien >= blocks[i]->x && ball->x - ball->promien <= blocks[i]->x;
			bool isCollisionFromRight = ball->x - ball->promien <= blocks[i]->x2 && ball->x + ball->promien >= blocks[i]->x2;

			if (isCollisionFromTop || isCollisionFromBottom)
			{
				ball->vy *= -1;
				ball->y += ball->vy;
			}

			if (isCollisionFromLeft || isCollisionFromRight)
			{
				ball->vx *= -1;
				ball->x += ball->vx;
			}

			if (ball->level == 1)
			{
				delete blocks[i];
				ball->points += 10;
				ball->blocksDestroyed += 1;
			}
			else if (ball->level == 2)
			{
				if (blocks[i]->getHitCount() == 2)
				{
					delete blocks[i];
					ball->points += 10;
					ball->blocksDestroyed += 1;
					blocks[i]->setHitCount(0);
				}
			}

			return true;
		}
	}

	return false;
}

///Odbijanie pilki od reszty obiektow
void Pilka::odbijanie(Pilka* ball, Paletka* p, Block* blocks[], int n, Block* health[], int* z)
{

	ball->x += ball->vx;
	ball->y += ball->vy;

	///Kolizja pilki z lewa strona ekranu
	if (ball->x - ball->promien <= 0)
	{
		ball->vx *= (-1);
	}

	///Kolizja pilki z prawa strona ekranu
	if (ball->x + ball->promien >= SCREEN_WIDTH)
	{
		ball->vx *= (-1);
	}
	///Kolizja pilki z dolem ekranu
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
	///Kolizja pilki z gora ekranu
	if (ball->y - ball->promien <= 0)
	{
		ball->vy *= (-1);
	}



	// Kolizja pilki z paletka
	if (ball->y + ball->promien > p->y && ball->y + ball->promien < p->y2)
	{
		if ((ball->x + ball->promien > p->x && ball->x + ball->promien < p->x2) ||
			(ball->x - ball->promien > p->x && ball->x - ball->promien < p->x2) ||
			((ball->x - ball->promien < p->x2 && ball->x + ball->promien > p->x)))
		{

			// Obliczanie względnego położenia piłki na paletce
			float relativePosition = (ball->x - p->x) / p->width;

			// Dostosowanie kata odbicia na podstawie względnej pozycji
			const float maxReflectionAngle = 60.0f;  // Maksymalny kąt odbicia w stopniach
			float reflectionAngle = relativePosition * (2 * maxReflectionAngle) - maxReflectionAngle;
			float reflectionRadians = reflectionAngle * 3.14159f / 180.0f;

			// Aktualizowanie predkosci pilki na podstawie kata odbicia, zachowując predkosc
			float currentSpeed = sqrt(ball->vx * ball->vx + ball->vy * ball->vy);
			ball->vx = currentSpeed * sin(reflectionRadians);
			ball->vy = -currentSpeed * cos(reflectionRadians);
		}
	}

	ball->collision(ball, blocks, n, points, level);

}

