#include "Bullet.h"

Bullet::Bullet()
{
	m_bulletShape.setSize(Vector2f(2,2));
}

void Bullet::stop()
{
	m_InFlight = false;
}

void Bullet::shoot(float startX, float startY, float xTarget, float yTarget)
{
	//Keep track of the bullet
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	// Calculate the gradient of the flight path
	float gradient = (xTarget - startX) / (yTarget - startY);

	if (gradient < 0)
		gradient *= -1;

	//Calculate the ration between x and y
	float rationXY = m_BulletSpeed / (1 + gradient);

	//Set the "speed" horizontally and vertically
	m_BulletDistanceY = rationXY;
	m_BulletDistanceX = rationXY * gradient;

	//Point the bullet in the right direction
	if (xTarget < startX)
	{
		m_BulletDistanceX *= -1;
	}

	if (yTarget < startY)
	{
		m_BulletDistanceY *= -1;
	}

	//Set a max range of 1000 pixels
	float range = 1000;

	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxX = startY + range;

	// Position the bullet ready to be drawn
	m_bulletShape.setPosition(m_Position);
}

void Bullet::update(float elapsedTime)
{
	//Update the bullet position variable
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	//Move the bullet
	m_bulletShape.setPosition(m_Position);

	//Has the bullet gone out of range?
	if (m_Position.x < 0 || m_Position.x > 1000 || m_Position.y < 0 || m_Position.y > 1000)
		m_InFlight = false;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}

FloatRect Bullet::getPosition()
{
	return m_bulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_bulletShape;
}


