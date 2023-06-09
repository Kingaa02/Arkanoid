﻿#include "Game.h"

float x, y;

///Sprawdzanie inicjalizacji
void Game::sprawdzenie_init(bool test, string opis) {
	if (test)
		return;

	cout << "Nie udalo sie uruchomic " << opis << endl;
}

///Inicjalizacja zycia
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

///Inicjalizacja blokow do zbijania
Block** createBlocks(int n, int ilosc_wierszy, int ilosc_rzedow) {
	Block** blocks = new Block * [n];

	for (int i = 0; i < n; i++) {
		if (i == 0) {
			blocks[i] = new Block(290);
		}
		else {
			blocks[i] = new Block();
		}
	}

	int z = 0;
	for (int i = 0; i < ilosc_wierszy; i++) {
		for (int j = 0; j < ilosc_rzedow; j++) {
			if (i == 0) {
				blocks[j + 1]->x = blocks[j]->x2;
				blocks[j + 1]->y = blocks[j]->y2 - 20;
				blocks[j + 1]->x2 = blocks[j + 1]->x + 65;
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

	///Wyswietlanie obrazu
	ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	sprawdzenie_init(display, "Obraz");

	///Muzyka
	al_reserve_samples(1);
	ALLEGRO_SAMPLE* song = al_load_sample("song.wav");
	ALLEGRO_SAMPLE_INSTANCE* songInstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());

	///Zegar
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	sprawdzenie_init(timer, "Zegar");

	///Bitmapy do tla w menu i grze
	ALLEGRO_BITMAP* background = al_load_bitmap("background.png");
	sprawdzenie_init(background, "background");

	ALLEGRO_BITMAP* background2 = al_load_bitmap("background2.png");
	sprawdzenie_init(background, "background2");

	///Kursor myszki
	ALLEGRO_BITMAP* cursorImage = al_load_bitmap("cursor.png");
	ALLEGRO_MOUSE_STATE mouseState;
	
	///Kolejka
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	sprawdzenie_init(queue, "Kolejka");

	///Czcionki uzywane w grze
	ALLEGRO_FONT* font = al_load_font("FjallaOne.ttf", 20, 0);
	ALLEGRO_FONT* font2 = al_load_font("FjallaOne.ttf", 100, 0);
	ALLEGRO_FONT* font3 = al_load_font("FjallaOne.ttf", 36, 0);
	ALLEGRO_FONT* font4 = al_load_font("FjallaOne.ttf", 70, 0);

	///Zarejestrowanie eventow
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	Pilka b;
	Paletka p;

	///Wszystkie flagi uzywane w grze
	bool done = false;
	bool redraw = true;
	bool menu = true;
	bool running = true;
	bool new_game = false;
	bool space = false;
	bool level2Clicked = false;
	bool right = false;
	bool left = false;
	bool returnToMenu = false;

	ALLEGRO_EVENT event;

	const int ilosc_wierszy = 4;
	const int ilosc_rzedow = 10;
	const int n = (ilosc_wierszy * ilosc_rzedow);

	Block** blocks = createBlocks(n, ilosc_wierszy, ilosc_rzedow);

	int pressed = false;
	int mouse_x, mouse_y;

	///Odtwarzanie muzyki
	al_play_sample_instance(songInstance);

	///Rozpoczecie dzialania zegara
	al_start_timer(timer);

	int health_z = 2;
	Block* health[3];

	initializeHealthBlocks(health, health_z);

	///Stworzenie kursora
	ALLEGRO_MOUSE_CURSOR* cursor = al_create_mouse_cursor(cursorImage, 0, 0);
	al_set_mouse_cursor(display, cursor);

	int buttonX = SCREEN_WIDTH / 2 - 100;
	int buttonY = SCREEN_HEIGHT / 2 + 150;
	int buttonWidth = 200;
	int buttonHeight = 50;

	srand(time(NULL));

	// Randomizacja koloru bloków do zbijania
	vector<ALLEGRO_COLOR> blockColors;
	for (int i = 0; i < ilosc_wierszy; i++)
	{
    for (int j = 0; j < ilosc_rzedow; j++)
    {
        ALLEGRO_COLOR randomColor = al_map_rgb(rand() % 256, rand() % 100, rand() % 256);
        blockColors.push_back(randomColor);
    }
}
	///Rozpoczecie glownej petli gry
	while (true && running)
	{
		al_wait_for_event(queue, &event);
		al_get_mouse_state(&mouseState);

		int windowX, windowY;
		al_get_window_position(display, &windowX, &windowY);

		int mouseX = mouseState.x - windowX;
		int mouseY = mouseState.y - windowY;

		al_set_target_bitmap(al_get_backbuffer(display));

		///Menu
		if (menu)
		{
			al_draw_bitmap(background, 0, 0, 0);
			al_draw_text(font4, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 230, ALLEGRO_ALIGN_CENTER, "ARKANOID");
			al_draw_text(font3, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 30, ALLEGRO_ALIGN_CENTER, "New Game");
			al_draw_text(font3, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 30, ALLEGRO_ALIGN_CENTER, "Exit");
		}

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			///Poruszanie sie pilki
			b.odbijanie(&b, &p, blocks, n, health, &health_z);


			/// Poruszanie paletki
			if (right)
			{
				/// Ograniczenie ruchu paletki na szerokosc ekranu z prawej strony
				if (p.x < SCREEN_WIDTH && p.x2 < SCREEN_WIDTH)
				{
					p.x += p.dx;
					p.x2 += p.dx;
				}

			}

			if (left)
			{
				/// Ograniczenie ruchu paletki na szerokosc ekranu z prawej strony
				if (p.x > 0 && p.x2 > 0)
				{
					p.x -= p.dx;
					p.x2 -= p.dx;
				}

			}
			redraw = true;
			break;


		case ALLEGRO_EVENT_KEY_DOWN:

			/// Wykrywanie wcisniecia przyciskow do poruszania paletka 
			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				right = true;

			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
				left = true;


			/// Wyrzucenie pilki
			if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
			{
				if (b.level == 1)
				{
					b.set_speed(6);
				}
				else if (b.level == 2)
				{
					b.set_speed(7);
				}
				space = true;
			}

			break;

		/// Wykrywanie puszczenia przycisku
		case ALLEGRO_EVENT_KEY_UP:
			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				right = false;

			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
				left = false;

			break;

		///Obsluga zdarzenia zamkniecia okna
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;

		///Obsluga zdarzen myszki
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
					b.set_speed(0);
					b.x = SCREEN_WIDTH / 2 - 50;
					b.y = SCREEN_HEIGHT / 2;
				}

				if (!new_game)
				{
					// Wyjscie
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

				// Obsluga przycisku "LEVEL 2"
				if (b.level != 2)
				{
					if (event.mouse.x >= buttonX && event.mouse.x <= buttonX + buttonWidth &&
						event.mouse.y >= buttonY + 60 && event.mouse.y <= buttonY + buttonHeight + 60)
					{
						level2Clicked = true;
						b.level = 2;
						new_game = true;
					}
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

		///Reset przed rozpoczeciem nowej gry
		if (returnToMenu)
		{
			health_z = 2;
			*health[3];
			initializeHealthBlocks(health, health_z);
			b.blocksDestroyed = 0;
			b.points = 0;
			blocks = createBlocks(n, ilosc_wierszy, ilosc_rzedow);
			menu = true;
			new_game = false;
			returnToMenu = false;
			level2Clicked = false;
			b.level = 1;
		}

		///Level 2
		if (level2Clicked)
		{
			health_z = 2;
			*health[3];
			initializeHealthBlocks(health, health_z);
			b.blocksDestroyed = 0;
			blocks = createBlocks(n, ilosc_wierszy, ilosc_rzedow);
			menu = false;
			new_game = true;
			level2Clicked = false; // Zresetowanie wartosci level2Clicked
			b.set_speed(0);
			b.x = SCREEN_WIDTH / 2 - 50;
			b.y = SCREEN_HEIGHT / 2;

		}




		if (redraw && al_is_event_queue_empty(queue) && new_game)
		{
			///Odswiezanie ekranu
			al_draw_bitmap(background2, 0, 0, 0);

			string pointsText = to_string(b.points);
			string levelText = to_string(b.level);

			if ((health_z >= 0) && (b.blocksDestroyed < ilosc_rzedow * ilosc_wierszy))
			{
				///Komunikat o wcisnieciu spacji
				if (!pressed && !space)
				{
					al_draw_text(font3, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20, ALLEGRO_ALIGN_CENTER, "Press 'Space' to start");
				}
				/// Wyswietlanie pilki 
				al_draw_filled_circle(b.x, b.y, b.promien, al_map_rgb(255, 255, 255));
				al_draw_circle(b.x, b.y, b.promien, al_map_rgb(0, 0, 0), 3);

				/// Wyswietlanie paletki
				al_draw_filled_rectangle(p.x, p.y, p.x2, p.y2 - 5, al_map_rgb(255, 0, 0));
				al_draw_rectangle(p.x, p.y, p.x2, p.y2 - 5, al_map_rgb(9, 0, 0), 3);

				al_draw_text(font, al_map_rgb(0, 255, 0), 10, 0, 0, "HEALTH");
				al_draw_text(font, al_map_rgb(0, 255, 0), 450, 0, 0, "LEVEL");
				al_draw_text(font, al_map_rgb(0, 255, 0), 910, 0, 0, "POINTS");

				///Wyswietlanie liczby punktów
				al_draw_text(font, al_map_rgb(0, 255, 0), 930, 30, 0, pointsText.c_str());

				///Wyswietlanie poziomu
				al_draw_text(font, al_map_rgb(0, 255, 0), 465, 30, 0, levelText.c_str());

				// Wyswietlanie bloków do zbijania
				for (int i = 0; i < n; i++)
				{
					if (b.level == 2)
					{
						if (blocks[i]->getHitCount() == 0)
						{
							al_draw_filled_rectangle(blocks[i]->x, blocks[i]->y, blocks[i]->x2, blocks[i]->y2, blockColors[i]);
							al_draw_rectangle(blocks[i]->x, blocks[i]->y, blocks[i]->x2, blocks[i]->y2, al_map_rgb(0, 0, 0), 3);
						}
						else if(blocks[i]->getHitCount() == 1)
						{
							al_draw_filled_rectangle(blocks[i]->x, blocks[i]->y, blocks[i]->x2, blocks[i]->y2, al_map_rgb(0, 255, 0));
							al_draw_rectangle(blocks[i]->x, blocks[i]->y, blocks[i]->x2, blocks[i]->y2, al_map_rgb(0, 0, 0), 3);
						}
					}
					else if (b.level != 2)
					{
						al_draw_filled_rectangle(blocks[i]->x, blocks[i]->y, blocks[i]->x2, blocks[i]->y2, al_map_rgb(0, 255, 0));
						al_draw_rectangle(blocks[i]->x, blocks[i]->y, blocks[i]->x2, blocks[i]->y2, al_map_rgb(0, 0, 0), 3);
					}
				}

				//Wyswietlanie blokow zycia
				for (int i = 0; i < 3; i++)
				{
					al_draw_filled_rectangle(health[i]->x, health[i]->y, health[i]->x2, health[i]->y2, al_map_rgb(0, 255, 0));
					al_draw_rectangle(health[i]->x, health[i]->y, health[i]->x2, health[i]->y2, al_map_rgb(0, 0, 0), 3);
				}

			}
			///Ekran wygranej
			else if (b.blocksDestroyed == ilosc_rzedow * ilosc_wierszy) {
				al_draw_text(font2, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 200, ALLEGRO_ALIGN_CENTER, "YOU WIN");
				al_draw_text(font3, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, ALLEGRO_ALIGN_CENTER, "Total points:");
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
						b.level = 1;
						level2Clicked = false;
						new_game = true;
						returnToMenu = false; // Zresetowanie wartosci returnToMenu
					}
				}

				if (b.level != 2)
				{
					al_draw_filled_rectangle(buttonX, buttonY + 60, buttonX + buttonWidth, buttonY + buttonHeight + 60, al_map_rgb(66, 218, 245));
					al_draw_rectangle(buttonX, buttonY + 60, buttonX + buttonWidth, buttonY + buttonHeight + 60, al_map_rgb(255, 255, 255), 2);
					al_draw_text(font3, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 215, ALLEGRO_ALIGN_CENTER, "Level 2");

				}

			}
			else
			{
				///Ekran przegranej
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
						returnToMenu = false; // Zresetowanie wartosci returnToMenu
					}
				}
			}



			redraw = false;
		}
		///Aktualizacja zawartości bufora graficznego
		al_flip_display();
	}

	///Zwolnienie pamieci
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_destroy_bitmap(background);
	al_destroy_font(font);
	al_destroy_font(font2);
	al_destroy_bitmap(cursorImage);
	al_destroy_bitmap(background2);

	return 1;

}
	

		