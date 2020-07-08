#include "Zombie.h"

bool Zombie::hit()
{
	this->m_health--;
	if (this->m_health < 0)
	{
		//Dead
		this->m_Alive = false;

		this->m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}

	//injured but not dead yet
	return false;
}

bool Zombie::isAlive()
{
	return this->m_Alive;
}

void Zombie::spawn(float startX, float startY, int type, int seed)
{
	switch (type)
	{
	case 0:
		// Bloater
		this->m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));

		this->m_speed = 40;
		this->m_health = 5;
		break;	
	case 1:
		//	Chaser
		this->m_Sprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));

		this->m_speed = 70;
		this->m_health = 1;
		break;	
	case 2:
		// Crawler
		this->m_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));

		this->m_speed = 20;
		this->m_health = 3;
		break;
	default:
		break;
	}

	// Modify the speed to make the zombie unique
	// Every zombie is unique.Create a speed modifier
	srand((int)time(NULL) * seed);
	// Somewhere between 80 an 100
	float modifier = (rand() % (int)this->MAX_VARRIANCE) + this->OFFSET;

	//Express this as a fraction of 1
	modifier /= 100; //Now equal between 0.7 to 1
	m_speed *= modifier;

	//Initialize its location
	m_Position.x = startX;
	m_Position.y = startY;

	//Set its origin to its center
	m_Sprite.setOrigin(25, 25);

	//Set its position
	m_Sprite.setPosition(this->m_Position);
}

FloatRect Zombie::getPosition()
{
	return FloatRect(this->m_Sprite.getGlobalBounds());
}

Sprite Zombie::getSprite()
{
	return Sprite(this->m_Sprite);
}

void Zombie::update(float elapsedTime, Vector2f playerLocation)
{
	float & playerX = playerLocation.x;
	float & playerY = playerLocation.y;

	if (playerX > m_Position.x)
	{
		this->m_Position.x = this->m_Position.x + this->m_speed * elapsedTime;
	}

	if (playerY > m_Position.y)
	{
		this->m_Position.y = this->m_Position.y + this->m_speed * elapsedTime;
	}

	if (playerX < m_Position.x)
	{
		this->m_Position.x = this->m_Position.x - this->m_speed * elapsedTime;
	}

	if (playerY > m_Position.y)
	{
		this->m_Position.y = this->m_Position.y - this->m_speed * elapsedTime;
	}

	//Move the sprite
	this->m_Sprite.setPosition(this->m_Position);

	// Face	the sprite in the correct direction
	float angle =  (atan2(playerY - this->m_Position.y, playerX - this->m_Position.y) * 180 / 3.141);
	this->m_Sprite.setRotation(angle);
}
