#pragma once
#include <allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include<math.h>
#include<iostream>
#include<vector>
#include "Pilka.h"
#include"Paletka.h"
#include"Block.h"
#include<iostream>
using namespace std;

class Game
{
public:
	
	void sprawdzenie_init(bool test, string opis);

	bool game_loop();
};

