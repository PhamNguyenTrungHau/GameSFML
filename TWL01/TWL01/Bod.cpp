#include "Bod.h"
#include"TextureHolder.h"


Bod::Bod()
{
	m_Sprite.setTexture(TextureHolder::GetTexture("graphics/bob.png"));
	m_JumpDuration = .24;
}

bool Bod::handleInput()
{
	m_JustJumped = false;

	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		if (!m_IsFalling && !m_IsJumping)
		{
			m_IsJumping = true;
			m_TimeThisJump = 0;
			m_JustJumped = true;
		}
	}
	else
	{
		m_IsJumping = false;
		m_IsFalling = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Left))
		m_LeftPressed = true;
	else
		m_LeftPressed = false;

	if (Keyboard::isKeyPressed(Keyboard::Right))
		m_RightPressed = true;
	else
		m_RightPressed = false;


	return  m_JustJumped;
}
