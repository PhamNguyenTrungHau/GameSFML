#include"Engine.h"

void Engine::draw()
{
	m_Window.clear(Color::White);
	
	if(!m_SplitScreen)
	{
		//Switch to background View
		m_Window.setView(m_BGMainView);
		//Draw background
		m_Window.draw(m_BacgroundSprite);
		//switch to m_MainView
		m_Window.setView(m_MainView);

		//Draw level
		m_Window.draw(m_VALevel, &m_TextureTiles);

		//Draw thomas
		m_Window.draw(m_Thomas.getSprite());
		//Draw Bod
		m_Window.draw(m_Bob.getSprite());
	}
	else
	{
		m_Window.setView(m_BGLeftView);
		m_Window.draw(m_BacgroundSprite);
		m_Window.setView(m_LeftView);

		//Draw level
		m_Window.draw(m_VALevel, &m_TextureTiles);

		//Draw Bod
		m_Window.draw(m_Bob.getSprite());
		//Draw thomas
		m_Window.draw(m_Thomas.getSprite());

		m_Window.setView(m_BGRightView);
		m_Window.draw(m_BacgroundSprite);
		m_Window.setView(m_RightView);

		//Draw level
		m_Window.draw(m_VALevel, &m_TextureTiles);

		//Draw thomas
		m_Window.draw(m_Thomas.getSprite());
		//Draw Bod
		m_Window.draw(m_Bob.getSprite());
	}

	m_Window.setView(m_HudView);
	m_Window.display();
}