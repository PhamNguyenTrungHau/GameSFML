#pragma once
#include"TextureHolder.h"
#include"SFML/Graphics.hpp"
#include<cstdlib>
#include<time.h>
using namespace sf;

class Zombie
{
	//How fast is each zombie type?
	const float BLOATER_SPEED = 40.f;
	const float CHASER_SPEED = 80.f;
	const float CRAWLER_SPEED = 20.f;
	//How touch is each zombie type?
	const float BLOATER_HEALTH = 5.f;
	const float CHASER_HEALTH = 1.f;
	const float CRAWLER_HEALTH = 3.f;
	//Make each zombie vary its speed slightly
	const float	MAX_VARRIANCE = 3.f;
	const int OFFSET = 101 - MAX_VARRIANCE;
	//Where is this zombie?
	Vector2f m_Position;

	//A sprite	 for the zombie
	Sprite m_Sprite;

	//How	fast can this one run/crawl?
	int m_speed;

	//How much health has it got?
	int m_health;

	//Is it still alive?
	bool m_Alive;
public:
	// Handle when a bullet hits a zombie	
	bool hit();
	//Find out if the zombie is alive
	bool isAlive();
	//Spawn a new zombie
	void spawn(float startX, float startY, int type, int seed);
	//Return a rectangle that is the position in the world
	FloatRect getPosition();
	//Get a copy of the sprite to draw
	Sprite getSprite();
	//Update the zombie each frame
	void update(float elapsedTime, Vector2f playerLocation);
};

