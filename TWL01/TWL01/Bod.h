#pragma once
#ifndef BOD_H_
#define BOD_H_

#include"PlayableCharacter.h"

class Bod : public PlayableCharacter
{
public:
	Bod();
	bool virtual handleInput();
};

#endif // !BOD_H_


