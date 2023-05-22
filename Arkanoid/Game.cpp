#include "Game.h"

float x, y;

void Game::sprawdzenie_init(bool test, string opis){
	if (test)
		return;

	cout << "Nie udalo sie uruchomic " << opis << endl;
}

//Inicjalizacja ¿ycia
void initializeHealthBlocks(Block* health[], int health_z)
{
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
}

//Inicjalizacja bloków
Block** createBlocks(int n, int ilosc_wierszy, int ilosc_rzedow) {
	Block** blocks = new Block * [n];

	for (int i = 0; i < n; i++) {
		if (i == 0)
			blocks[i] = new Block(150);
		else
			blocks[i] = new Block();
	}

	int z = 0;
	for (int i = 0; i < ilosc_wierszy; i++) {
		for (int j = 0; j < ilosc_rzedow; j++) {
			if (i == 0) {
				blocks[j + 1]->x = blocks[j]->x2;
				blocks[j + 1]->y = blocks[j]->y2 - 20;

				blocks[j + 1]->x2 = blocks[j + 1]->x + 50;
				blocks[j + 1]->y2 = blocks[j + 1]->y + 20;
				z++;
			}
			else {
				blocks[z]->x = blocks[z - ilosc_rzedow]->x;
				blocks[z]->y = blocks[z - ilosc_rzedow]->y - 20;

				blocks[z]->x2 = blocks[z - ilosc_rzedow]->x2;
				blocks[z]->y2 = blocks[z - ilosc_rzedow]->y2 - 20;
				z++;
			}
		}
	}

	return blocks;
}




bool Game::game_loop() {

	sprawdzenie_init(al_init(), "allegro");
	sprawdzenie_init(al_install_keyboard(), "klawiatura");
	sprawdzenie_init(al_init_primitives_addon(), "primitives addon");
	sprawdzenie_init(al_init_font_addon(), "czcionka");
	sprawdzenie_init(al_init_ttf_addon(), "TTF");
	sprawdzenie_init(al_init_image_addon(), "image addon");
	sprawdzenie_init(al_install_mouse(), "mouse");
	sprawdzenie_init(al_install_audio(), "audio");
	sprawdzenie_init(al_init_acodec_addon(), "acodec");


	ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	sprawdzenie_init(display, "Obraz");

	//MUZYKA
	al_reserve_samples(1);
	ALLEGRO_SAMPLE* song = al_load_sample("song.wav");
	ALLEGRO_SAMPLE_INSTANCE* songInstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());


	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	sprawdzenie_init(timer, "Zegar");

	ALLEGRO_BITMAP* background = al_load_bitmap("background.png");
	sprawdzenie_init(background, "background");

	//KURSOR
	ALLEGRO_BITMAP* cursorImage = al_load_bitmap("cursor.png");
	ALLEGRO_MOUSE_STATE mouseState;

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	sprawdzenie_init(queue, "Kolejka");

	ALLEGRO_FONT* font = al_load_font("FjallaOne.ttf", 20, 0);
	ALLEGRO_FONT* font2 = al_load_font("FjallaOne.ttf", 100, 0);
	ALLEGRO_FONT* font3 = al_load_font("FjallaOne.ttf", 36, 0);
	ALLEGRO_FONT* font4 = al_load_font("FjallaOne.ttf", 70, 0);

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));




	Pilka b;
	Paletka p;

	bool done = false;
	bool redraw = true;
	bool menu = true;
	bool running = true;
	bool new_game = false;

	ALLEGRO_EVENT event;

	const int ilosc_wierszy = 6;
	const int ilosc_rzedow = 13;
	const int n = (ilosc_wierszy * ilosc_rzedow);

	Block** blocks = createBlocks(n, ilosc_wierszy, ilosc_rzedow);



	bool right = false;
	bool left = false;
	int pressed = false;
	int mouse_x, mouse_y;

	al_play_sample_instance(songInstance);

	al_start_timer(timer);

	int health_z = 2;
	Block* health[3];

	initializeHealthBlocks(health, health_z);

	ALLEGRO_MOUSE_CURSOR* cursor = al_create_mouse_cursor(cursorImage, 0, 0);
	al_set_mouse_cursor(display, cursor);

	bool returnToMenu = false;
	int buttonX = SCREEN_WIDTH / 2 - 100; 
	int buttonY = SCREEN_HEIGHT / 2 + 150; 
	int buttonWidth = 200; 
	int buttonHeight = 50; 

	while (true && running)
	{
		al_wait_for_event(queue, &event);
		al_get_mouse_state(&mouseState);

		int windowX, windowY;
		al_get_window_position(display, &windowX, &windowY);

		int mouseX = mouseState.x - windowX;
		int mouseY = mouseState.y - windowY;

		al_set_target_bitmap(al_get_backbuffer(display));

		if (menu)
		{
			al_draw_bitmap(background, 0, 0, 0);
			al_draw_text(font4, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 230, ALLEGRO_ALIGN_CENTER, "ARKANOID");
			al_draw_text(font3, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 30, ALLEGRO_ALIGN_CENTER, "Nowa Gra");
			al_draw_text(font3, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 30, ALLEGRO_ALIGN_CENTER, "Wyjscie");
		}

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			///Poruszanie siê pi³ki
			b.odbijanie(&b, &p, blocks, n, health, &health_z);


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

		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

			if (event.mouse.button & 1)
			{
				mouse_x = event.mouse.x;
				mouse_y = event.mouse.y;
			}
			if (event.mouse.x >= 0 && event.mouse.x <= 430 &&
				event.mouse.y >= 0 && event.mouse.y <= 50) {

			}

			break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:

			if (event.mouse.button & 1)
			{
				// Nowa gra
				if (event.mouse.x > SCREEN_WIDTH / 2 - 75 && event.mouse.x < SCREEN_WIDTH / 2 + 65 && event.mouse.y > SCREEN_HEIGHT / 2 - 30 && event.mouse.y < SCREEN_HEIGHT / 2 - 5)
				{
					menu = false;
					new_game = true;
				}

				if (!new_game)
				{
					// Wyjœcie
					if (event.mouse.x > SCREEN_WIDTH / 2 - 65 && event.mouse.x < SCREEN_WIDTH / 2 + 55 && event.mouse.y > SCREEN_HEIGHT / 2 + 30 && event.mouse.y < SCREEN_HEIGHT / 2 + 70)
					{
						menu = false;
						running = false;
						done = true; // Poprawka: Przypisanie wartoœci true do zmiennej done
					}
				}

				// Powrót do menu
				if (event.mouse.x >= buttonX && event.mouse.x <= buttonX + buttonWidth &&
					event.mouse.y >= buttonY && event.mouse.y <= buttonY + buttonHeight)
				{
					returnToMenu = true;
				}
			}
			break;

		case ALLEGRO_EVENT_MOUSE_AXES:

			x = event.mouse.x;
			y = event.mouse.y;

			break;
		}


		if (done)
			break;

		///reset przed rozpoczêciem nowej gry
		if (returnToMenu)
		{
			health_z = 2;
			* health[3];
			initializeHealthBlocks(health, health_z); 
			b.points = 0;
			blocks = createBlocks(n, ilosc_wierszy, ilosc_rzedow); 
			menu = true;
			new_game = false;
			returnToMenu = false; 
		}


		if (redraw && al_is_event_queue_empty(queue) && new_game)
		{
			///Odœwie¿anie ekranu
			al_clear_to_color(al_map_rgb(28, 28, 28));

			///Wyœwietlanie bloków

			string pointsText = to_string(b.points);
			if (health_z >= 0)
			{
				/// Wyœwietlanie pi³ki 
				al_draw_filled_circle(b.x, b.y, b.promien, al_map_rgb(255, 33, 33));
				/// Wyœwietlanie paletki
				al_draw_filled_rectangle(p.x, p.y, p.x2, p.y2, al_map_rgb(66, 218, 245));

				al_draw_text(font, al_map_rgb(12, 213, 123), 10, 0, 0, "HEALTH");
				al_draw_text(font, al_map_rgb(12, 213, 123), 910, 0, 0, "POINTS");

				///Wyœwietlanie liczby punktów
				al_draw_text(font, al_map_rgb(12, 213, 123), 930, 30, 0, pointsText.c_str());

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
				al_draw_text(font2, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + -200, ALLEGRO_ALIGN_CENTER, "GAME OVER");
				al_draw_text(font3, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + -50, ALLEGRO_ALIGN_CENTER, "Total points:");
				al_draw_text(font3, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, pointsText.c_str());


				al_draw_filled_rectangle(buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight, al_map_rgb(66, 218, 245));
				al_draw_rectangle(buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight, al_map_rgb(255, 255, 255), 2);
				al_draw_text(font3, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 155, ALLEGRO_ALIGN_CENTER, "Menu");

				if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && event.mouse.button & 1)
				{
					if (event.mouse.x >= buttonX && event.mouse.x <= buttonX + buttonWidth &&
						event.mouse.y >= buttonY && event.mouse.y <= buttonY + buttonHeight)
					{
						menu = true;
						new_game = false;
						returnToMenu = false; // Zresetowanie wartoœci returnToMenu
					}
				}
			}

			redraw = false;
		}
		al_flip_display();
}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_destroy_bitmap(background);
	al_destroy_font(font);
	al_destroy_font(font2);
	al_destroy_bitmap(cursorImage);

	return 1;

}