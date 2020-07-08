#pragma once
#ifndef PlayableCharacter_H_
#define PlayableCharacter_H_

#include"SFML/Graphics.hpp"

using namespace sf;

class PlayableCharacter
{

protected:

	Sprite m_Sprite;
	float m_JumpDuration;
	bool m_IsJumping;
	bool m_IsFalling;
	bool m_LeftPressed;
	bool m_RightPressed;
	float m_TimeThisJump;
	bool m_JustJumped;

private:

	float m_Gravity;
	float m_Speed;
	Vector2f m_Position;
	FloatRect m_Feet;
	FloatRect m_Head;
	FloatRect m_Left;
	FloatRect m_Right;
	Texture m_Texture;

public:

	void Spawn(Vector2f startPosition, float gravity);
	bool virtual handleInput() = 0;
	FloatRect getPosition();
	FloatRect getFeet();
	FloatRect getHead();
	FloatRect getLeft();
	FloatRect getRight();
	Sprite getSprite();
	Vector2f getCenter();
	void stopFalling(float position);
	void stopLeft(float position);
	void stopRight(float position);
	void stopJump();
	
	void update(float elapsedTime);

};

#endif // !PlayableCharacter_H_



