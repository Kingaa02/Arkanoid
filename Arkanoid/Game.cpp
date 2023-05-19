#include "Game.h"

void Game::sprawdzenie_init(bool test, string opis){
	if (test)
		return;

	cout << "Nie udalo sie uruchomic " << opis << endl;
}


bool Game::game_loop() {

	sprawdzenie_init(al_init(), "allegro");
	sprawdzenie_init(al_install_keyboard(), "klawiatura");
	sprawdzenie_init(al_init_primitives_addon(), "primitives addon");
	sprawdzenie_init(al_init_font_addon(), "czcionka");
	sprawdzenie_init(al_init_ttf_addon(), "TTF");



	ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	sprawdzenie_init(display, "Obraz");



	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	sprawdzenie_init(timer, "Zegar");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	sprawdzenie_init(queue, "Kolejka");

	ALLEGRO_FONT* font = al_load_font("FjallaOne.ttf", 20, 0);
	ALLEGRO_FONT* font2 = al_load_font("FjallaOne.ttf", 100, 0);


	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));



	Pilka b;
	Paletka p;

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
				blocks[z]->y = blocks[z - ilosc_rzedow]->y - 20;

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

	int health_z = 2;
	Block* health[3];

	for (int i = 0; i < 3; i++)
	{
		health[i] = new Block();
	}

	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			health[i]->x = 10;
			health[i]->y = 30;
			health[i]->x2 = 30;
			health[i]->y2 = 50;
		}
		else
		{
			health[i]->x = health[i - 1]->x2;
			health[i]->y = health[i - 1]->y;
			health[i]->x2 = health[i - 1]->x2 + 20;
			health[i]->y2 = health[i - 1]->y2;
		}

	}

	while (true)
	{
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			///Poruszanie si� pi�ki
			b.odbijanie(&b, &p, blocks, n, health, &health_z);


			/// Poruszanie paletki
			if (right)
			{
				/// Ograniczenie ruchu paletki na szeroko�� ekranu z prawej strony
				if (p.x < SCREEN_WIDTH && p.x2 < SCREEN_WIDTH)
				{
					p.x += p.dx;
					p.x2 += p.dx;
				}

			}

			if (left)
			{
				/// Ograniczenie ruchu paletki na szeroko�� ekranu z prawej strony
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
			/// Wykrywanie wci�ni�cia przycisku  
			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				right = true;

			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
				left = true;


			/// Wyrzucenie pi�ki
			if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
			{
				if (!pressed)
				{
					b.set_speed(4);
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
			///Od�wierzanie ekranu
			al_clear_to_color(al_map_rgb(28, 28, 28));


			///Wy�wietlanie blok�w

			if (health_z >= 0)
			{
				/// Wy�wietlanie pi�ki 
				al_draw_filled_circle(b.x, b.y, b.promien, al_map_rgb(255, 33, 33));
				/// Wy�wietlanie paletki
				al_draw_filled_rectangle(p.x, p.y, p.x2, p.y2, al_map_rgb(66, 218, 245));

				al_draw_text(font, al_map_rgb(12, 213, 123), 10, 0, 0, "HEALTH");

				for (int i = 0; i < n; i++)
				{
					al_draw_filled_rectangle(blocks[i]->x, blocks[i]->y, blocks[i]->x2, blocks[i]->y2, al_map_rgb(245, 102, 66));
					al_draw_rectangle(blocks[i]->x, blocks[i]->y, blocks[i]->x2, blocks[i]->y2, al_map_rgb(255, 255, 255), 2);
				}

				for (int i = 0; i < 3; i++)
				{
					al_draw_filled_rectangle(health[i]->x, health[i]->y, health[i]->x2, health[i]->y2, al_map_rgb(12, 213, 123));
					al_draw_rectangle(health[i]->x, health[i]->y, health[i]->x2, health[i]->y2, al_map_rgb(255, 255, 255), 2);


				}
			}
			else
			{

				al_draw_text(font2, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER");
			}



			al_flip_display();

			redraw = false;
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);




	return 1;

}