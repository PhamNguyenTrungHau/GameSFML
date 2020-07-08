#include "Player.h"
#include"TextureHolder.h"

//Contructors
Player::Player()
{
	this->m_Speed = this->START_SPEED;
	this->m_Health = this->START_HEALTH;
	this->m_MaxHealth = this->START_HEALTH;

	//Accociate  a texture with the sprite
	this->m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player.png"));

	//Set the origin of the sprite to the center
	// for smooth rotation
	this->m_Sprite.setOrigin(25, 25);
}

//Accessors
Time Player::getLastHitTime()
{
	return Time(this->m_LastHit);
}

FloatRect Player::getPosition()
{
	return FloatRect(this->m_Sprite.getGlobalBounds());
}

Vector2f Player::getCenter()
{
	return Vector2f(this->m_Sprite.getPosition());
}

float Player::geRotation()
{
	return this->m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return Sprite(this->m_Sprite);
}

int Player::getHealth()
{
	return m_Health;
}

//Functions
void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
	// Place the player in the middle of the arena
	this->m_Position.x = arena.width / 2;
	this->m_Position.y = arena.height / 2;

	// Copy the detals of the arena to the players m_Arena
	this->m_Arena.left	 = arena.left;
	this->m_Arena.top	 = arena.top;
	this->m_Arena.width	 = arena.width;
	this->m_Arena.height = arena.height;

	// Remenber how big the tiles are in this arena
	this->m_TileSize = tileSize;

	// Store the resolution for future use
	this->m_Resolution.x = resolution.x;
	this->m_Resolution.y = resolution.y;
}

void Player::resetPlayerStats()
{
	this->m_Speed = this->START_SPEED;
	this->m_Health = this->START_HEALTH;
	this->m_MaxHealth = this->START_HEALTH;
}

bool Player::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)
	{
		this->m_LastHit = timeHit;
		this->m_Health -= 10;
		return true;
	}
	return false;
}
void Player::moveLeft()
{
	this->m_LeftPressed = true;
}

void Player::moveRight()
{
	this->m_RightPressed = true;
}

void Player::moveUP()
{
	this->m_UpPressed = true;
}

void Player::moveDown()
{
	this->m_DownPressed = true;
}

void Player::stopLeft()
{
	this->m_LeftPressed = false;
}

void Player::stopRight()
{
	this->m_RightPressed = false;
}

void Player::stopUp()
{
	this->m_UpPressed = false;
}

void Player::stopDown()
{
	this->m_DownPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition)
{
	if (this->m_UpPressed)
		this->m_Position.y -= this->m_Speed * elapsedTime;
	if (this->m_DownPressed)
		this->m_Position.y += this->m_Speed * elapsedTime;
	if (this->m_LeftPressed)
		this->m_Position.x -= this->m_Speed * elapsedTime;
	if (this->m_RightPressed)
		this->m_Position.x += this->m_Speed * elapsedTime;

	this->m_Sprite.setPosition(this->m_Position);

	//Keep the player in the arena
	
	if (this->m_Position.x > this->m_Arena.width - this->m_TileSize)
	{
		this->m_Position.x = this->m_Arena.width - this->m_TileSize;
	}
	if (this->m_Position.x < this->m_Arena.left + this->m_TileSize)
	{
		this->m_Position.x = this->m_Arena.left + this->m_TileSize;
	}
	if (this->m_Position.y > this->m_Arena.height - this->m_TileSize)
	{
		this->m_Position.y = this->m_Arena.height - this->m_TileSize;
	}
	if (this->m_Position.y < this->m_Arena.top + this->m_TileSize)
	{
		this->m_Position.y = this->m_Arena.top + this->m_TileSize;
	}

	// Calculate the angle the player is facing
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
		mousePosition.x - m_Resolution.x / 2) * 180) / 3.141;

	m_Sprite.setRotation(angle);
}

void Player::upgradeSpeed()
{
	// 20% speed uprade
	this->m_Speed += (this->START_SPEED * 0.2f);
}

void Player::upgradeHealth()
{
	// 20% max health upgrade
	this->m_MaxHealth += (this->START_HEALTH * 0.2f);
}

void Player::increaseHealthLevel(int amount)
{
	this->m_Health += amount;

	//But not beyond the maximum
	if (this->m_Health > this->m_MaxHealth)
		this->m_Health = this->m_MaxHealth;
}
