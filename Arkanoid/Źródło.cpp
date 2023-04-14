#include <allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include<math.h>
#include<iostream>
#include<vector>
#include "Pilka.h"
#include"Paletka.h"
#include"Block.h"
using namespace std;


// Kolizja z œcianami 
void odbijanie(Pilka* ball, Paletka p, Block* blocks[], int n)
{
	ball->x += ball->vx;
	ball->y += ball->vy;

	///Kolizja pi³ki z lew¹ stron¹ ekranu
	if (ball->x - ball->promien <= 0)
	{
		ball->vx *=(-1);
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
	}
	///Kolizja pi³ki z gór¹ stron¹ ekranu
	if (ball->y - ball->promien <= 0)
	{
		ball->vy *= (-1);
	}
	

	///Kolizja pi³ki z paletk¹
	if (ball->y + ball->promien > p.y && ball->y + ball->promien < p.y2)
	{
		if (ball->x + ball->promien > p.x && ball->x + ball->promien < p.x2)
			if(ball->x + ball->promien > p.x && ball->x + ball->promien < p.x2)
			ball->vy = ball->speed * (-1);
		
	}
	ball->collision(ball, blocks, n);


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
	Pilka b;
	Paletka p;

	sprawdznie_init(al_init(), "allegro");
	sprawdznie_init(al_install_keyboard(), "klawiatura");
	sprawdznie_init(al_init_primitives_addon(), "primitives addon");



	ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
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

	const int ilosc_wierszy = 6;
	const int ilosc_rzedow = 13;
	const int n = (ilosc_wierszy * ilosc_rzedow);

	Block* blocks[n];



	for (int i = 0; i < n; i++)
	{
		if (i == 0)
			blocks[i] = new Block(150);
		else
			blocks[i] = new Block();
	}

	int z = 0;
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
				blocks[z]->y = blocks[z - ilosc_rzedow]->y-20;

				blocks[z]->x2 = blocks[z - ilosc_rzedow]->x2;
				blocks[z]->y2 = blocks[z - ilosc_rzedow]->y2 - 20;
				z++;
			}
		}
	}

	bool right = false;
	bool left = false;
	int pressed = false;
	al_start_timer(timer);

	while (true)
	{
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
	///Poruszanie siê pi³ki
			odbijanie(&b, p, blocks, n);


	/// Poruszanie paletki
			if (right)
			{
				/// Ograniczenie ruchu paletki na szerokoœæ ekranu z prawej strony
				if (p.x < SCREEN_WIDTH && p.x2 < SCREEN_WIDTH)
				{
					p.x += p.dx;
					p.x2 += p.dx;
				}
				
			}
				
			if (left)
			{
				/// Ograniczenie ruchu paletki na szerokoœæ ekranu z prawej strony
				if (p.x > 0 && p.x2 > 0)
				{
					p.x -= p.dx;
					p.x2 -= p.dx;
				}
				
			}
			redraw = true;
			break;

			/// Poruszanie paletki wykrywanie kierunku 
		case ALLEGRO_EVENT_KEY_DOWN:
			/// Wykrywanie wciœniêcia przycisku  
			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) 
				right = true;

			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
				left = true;


			/// Wyrzucenie pi³ki
			if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
			{
				
				if (!pressed)
				{
					b.set_speed(5);
				}
				
				
			}
			
		
			break;
			/// Wykrywanie puszczenia przycisku
		case ALLEGRO_EVENT_KEY_UP:
			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				right = false;

			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
				left = false;

			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		}

		if (done)
			break;

		if (redraw && al_is_event_queue_empty(queue))
		{
	///Odœwierzanie ekranu
			al_clear_to_color(al_map_rgb(255, 255, 255));
	/// Wyœwietlanie pi³ki 
			al_draw_filled_circle(b.x, b.y, b.promien, al_map_rgb(121, 29, 91));
	/// Wyœwietlanie paletki
			al_draw_rectangle(p.x, p.y, p.x2, p.y2, al_map_rgb(0, 0, 0), 2);

	///Wyœwietlanie bloków
			for (int i = 0; i < n; i++)
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