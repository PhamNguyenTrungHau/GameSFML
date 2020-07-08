#pragma once
#include"SFML/Graphics.hpp"
#include"Zombie.h"
using namespace sf;

int createBackground(VertexArray& vVA, IntRect arena);
Zombie * createHorde(int numZombies, IntRect arena);