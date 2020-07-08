#include"Engine.h"

void Engine::update(float dtAsSeconds)
{
	if (m_NewLevelRequired)
	{

		//These calls to spawn will be moved to a new
		//loadLevel() function soon
		
		//Spawn Thomas and Bob
		//m_Thomas.Spawn(Vector2f(0.f, 0.f), GRAVITY);
		//m_Bob.Spawn(Vector2f(100.f, 0.f), GRAVITY);
		

		//Make sure spawn is called only one
		//m_TimeRemaining = 10;
		//m_NewLevelRequired = false;

		//load a level
		loadLevel();

	}

	if(m_Playing)
	{
		//Update Thomas
		m_Thomas.update(dtAsSeconds);
		//Update Bob
		m_Bob.update(dtAsSeconds);

		// Detect collisions and see if characters
		// have reached the goal tile
		// The second part of the if condition is only executed
		// when thomas is touching the home tile
		if (detectCollisions(m_Thomas) && detectCollisions(m_Bob))
		{
			// New level required
			m_NewLevelRequired = true;
			// Play the reach goal sound
		}
		else
		{
			// Run bobs collision detection
			detectCollisions(m_Bob);
		}


		// Let bob and thomas jump on each others heads
		if (m_Bob.getFeet().intersects(m_Thomas.getHead()))
		{
			m_Bob.stopFalling(m_Thomas.getHead().top);
		}
		else if (m_Thomas.getFeet().intersects(m_Bob.getHead()))
		{
			m_Thomas.stopFalling(m_Bob.getHead().top);
		}


		//time the player has left
		m_TimeRemaining -= dtAsSeconds;
		
		//run out of time
		if(m_TimeRemaining <= 0)
		{
			m_NewLevelRequired = true;
		}		
	}//end if playing

	//Set the appropriate view around the appropriate character
	if (m_SplitScreen)
	{
		m_RightView.setCenter(m_Bob.getCenter());
		m_LeftView.setCenter(m_Thomas.getCenter());
		
	}
	else
	{
		//Centre full screen around appropriate character
		if (m_Character1)
			m_MainView.setCenter(m_Thomas.getCenter());
		else
			m_MainView.setCenter(m_Bob.getCenter());
	}


}