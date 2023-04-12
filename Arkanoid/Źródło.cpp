#include <allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include<math.h>
#include<iostream>
#include<vector>
using namespace std;

int width = 640;
int height = 480;


class pilka {
public:
	float promien;
	float x, y;
	float vx, vy;
	float speed;


	pilka()
	{
		x = 20;
		y = 20;
		speed = 2;
		vx = speed;
		vy = speed;

		promien = 15;

	}
};

class paletka {
public:
	float x = 0, x2 = 0, y = 0, y2 = 0;
	float dx = 0, dy = 0;

	paletka()
	{
		dx = 15;
		x = 250;
		x2 = x + 100;
		y = 400;
		y2 = y + 20;
	}

};

class block {
public:
	float x = 0, x2 = 0;
	float y = 0, y2 = 0;
	float points = 0;

	block(int x)
	{
		this->x = x;
		x2 = this->x + 50;
		y = 200;
		y2 = y + 20;

	}

	block()
	{

	}

};


bool collision(pilka* ball, block* blocks[], int n)
{
	for (int i = 0; i < n; i++)
	{
		if (ball->x + ball->promien >= blocks[i]->x &&
			ball->x - ball->promien <= blocks[i]->x2 &&
			ball->y + ball->promien >= blocks[i]->y &&
			ball->y - ball->promien <= blocks[i]->y2)
		{
			ball->vy *= -1;
			delete(blocks[i]);
			return true;
		}

	}

	return false;
}




void odbijanie(pilka* ball, paletka p, block* blocks[], int n)
{
	ball->x += ball->vx;
	ball->y += ball->vy;

	if (ball->x + ball->promien >= width)
	{
		ball->vx = ball->speed * (-1);
	}

	if (ball->y + ball->promien >= height)
	{
		ball->vy = ball->speed * (-1);
	}

	if (ball->y - ball->promien <= 0)
	{
		ball->vy = ball->speed;
	}

	if (ball->x - ball->promien <= 0)
	{
		ball->vx = ball->speed;
	}


	if (ball->y + ball->promien > p.y && ball->y + ball->promien < p.y2)
	{
		if (ball->x + ball->promien > p.x && ball->x + ball->promien < p.x2)
			ball->vy = ball->speed * (-1);
	}
	collision(ball, blocks, n);


}



//Sprawdanie inicjacji
void sprawdznie_init(bool test, string opis)
{
	if (test)
		return;

	cout << "Nie udalo sie uruchomic " << opis << endl;
}

int main()
{
	pilka b;
	paletka p;
	block a(100);





	sprawdznie_init(al_init(), "allegro");
	sprawdznie_init(al_install_keyboard(), "klawiatura");
	sprawdznie_init(al_init_primitives_addon(), "primitives addon");



	ALLEGRO_DISPLAY* display = al_create_display(width, height);
	sprawdznie_init(display, "Obraz");



	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	sprawdznie_init(timer, "Zegar");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	sprawdznie_init(queue, "Kolejka");


	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	bool done = false;
	bool redraw = true;

	ALLEGRO_EVENT event;

	// TUTAJ TERAZ ROBISZ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	const int ilosc_wierszy = 6;
	const int ilosc_rzedow = 13;
	const int n = (ilosc_wierszy * ilosc_rzedow);

	block* blocks[n];

	for (int i = 0; i < n; i++)
	{
		if (i == 0)
			blocks[i] = new block(100);
		else
			blocks[i] = new block();
	}

	for (int i = 0; i < 3; i++)
	{
		blocks[i + 1]->x = blocks[i]->x2;
		blocks[i + 1]->y = blocks[i]->y2 - 20;

		blocks[i + 1]->x2 = blocks[i + 1]->x + 50;
		blocks[i + 1]->y2 = blocks[i + 1]->y + 20;
	}



	for (int i = 3; i < 6; i++)
	{


		blocks[i]->x = blocks[i - 3]->x;
		blocks[i]->y = blocks[i - 3]->y - 20;

		blocks[i]->x2 = blocks[i - 3]->x2;
		blocks[i]->y2 = blocks[i - 3]->y2;
	}


	/*int z = 0;
	for (int i = 0; i < ilosc_wierszy; i++)
	{
		for (int j = 0; j < ilosc_rzedow; j++)
		{
			if (i == 0)
			{
				blocks[j + 1]->x = blocks[j]->x2;
				blocks[j + 1]->y = blocks[j]->y2 - 20;

				blocks[j + 1]->x2 = blocks[j + 1]->x + 50;
				blocks[j + 1]->y2 = blocks[j + 1]->y + 20;
				z++;
			}
			else
			{
				blocks[z]->x = blocks[z - ilosc_rzedow]->x;
				blocks[z]->y = blocks[z - ilosc_rzedow]->y - 20;

				blocks[z]->x2 = blocks[z - ilosc_rzedow]->x2;
				blocks[z]->y2 = blocks[z - ilosc_rzedow]->y2 - 10;
				z++;
			}
		}
	}

	*/

	al_start_timer(timer);

	while (true)
	{
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			// Poruszanie siê pi³ki
			odbijanie(&b, p, blocks, n);


			redraw = true;
			break;
		case ALLEGRO_EVENT_KEY_CHAR:
			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			{
				if (p.x < width && p.x2 < width)
				{
					p.x += p.dx;
					p.x2 += p.dx;
				}

			}

			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
			{
				if (p.x > 0 && p.x2 > 0)
				{
					p.x -= p.dx;
					p.x2 -= p.dx;
				}

			}



			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		}

		if (done)
			break;

		if (redraw && al_is_event_queue_empty(queue))
		{
			al_clear_to_color(al_map_rgb(255, 255, 255));
			al_draw_filled_circle(b.x, b.y, b.promien, al_map_rgb(121, 29, 91));




			al_draw_rectangle(p.x, p.y, p.x2, p.y2, al_map_rgb(0, 0, 0), 2);


			for (int i = 0; i < 6; i++)
			{

				al_draw_rectangle(blocks[i]->x, blocks[i]->y, blocks[i]->x2, blocks[i]->y2, al_map_rgb(0, 0, 0), 2);
			}

			al_flip_display();

			redraw = false;
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);





	return 0;
}