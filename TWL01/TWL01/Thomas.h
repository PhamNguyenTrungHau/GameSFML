#pragma once
#ifndef THOMAS_H_
#define THOMAS_H_

#include"PlayableCharacter.h"

class Thomas : public PlayableCharacter
{
public:
	Thomas();
	bool virtual handleInput();
};

#endif // !THOMAS_H_



