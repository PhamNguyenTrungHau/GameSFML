
#include"Engine.h"

void Engine::loadLevel()
{
	m_Playing = false;

	//delete the previously allocated memory
	for (auto i = 0; i < m_LM.getLevelSize().y; i++)
	{
		delete[] m_ArrayLevel[i];
	}

	//load the next 2d array
	//repopulate the vertex array
	m_ArrayLevel = m_LM.nextLevel(m_VALevel);

	//This new time limit
	m_TimeRemaining = m_LM.getTimeLimit();

	//Spawn thomas and Bob
	m_Thomas.Spawn(m_LM.getStartPosition(), GRAVITY);
	m_Bob.Spawn(m_LM.getStartPosition(), GRAVITY);

	//code isn't run again
	m_NewLevelRequired = false;

}