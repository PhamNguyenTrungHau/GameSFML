#include"Engine.h"

void Engine::input()
{
	Event event;
	while(m_Window.pollEvent(event))
	{
		if(event.type == Event::KeyPressed)
		{
			//handle the player quitting
			if(Keyboard::isKeyPressed(Keyboard::Escape))
				m_Window.close();
			
			//Starting the game
			if(Keyboard::isKeyPressed(Keyboard::Return))
				m_Playing = true;
			
			//Switch between Thomas and Bod
			if(Keyboard::isKeyPressed(Keyboard::Q))
				m_Character1 = !m_Character1;
			
			//Swith betwent full and split screen
			if(Keyboard::isKeyPressed(Keyboard::E))
				m_SplitScreen = !m_SplitScreen;
		}

		//Handle input specific	 to Thomas and Bob
		if (m_Thomas.handleInput())
		{
			//Play  a jump sound
		}

		if (m_Bob.handleInput())
		{
			//Play a jump sound
		}
	}
}
