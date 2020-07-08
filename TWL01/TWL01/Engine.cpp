#include"Engine.h"

Engine::Engine()
{

	//SFML Window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width - 300;
	resolution.y = VideoMode::getDesktopMode().height - 300;
	m_Window.create(VideoMode(resolution.x, resolution.y), "Thomas was late", Style::Default);
	
	//View
	m_MainView.setSize(resolution);
	m_HudView.reset(FloatRect(0, 0, resolution.x, resolution.y));
	m_LeftView.setViewport(FloatRect(0.001f, 0.001f,0.498f,0.998f));
	m_RightView.setViewport(FloatRect(0.5f, 0.001f, 0.499f, 0.998f));
	m_BGLeftView.setViewport(FloatRect(0.001f, 0.001f, 0.498f, 0.998f));
	m_BGRightView.setViewport(FloatRect(0.5f, 0.001f, 0.499f, 0.998f));
	
	//Sprite
	m_BackgroundTexture = TextureHolder::GetTexture("graphics/background1.png");
	m_BacgroundSprite.setTexture(m_BackgroundTexture);

	// Load the texture for the background vertex array
	m_TextureTiles = TextureHolder::GetTexture("graphics/tiles_sheet.png");

}

void Engine::run()
{
	//time
	Clock clock;
	//Loop
	while(m_Window.isOpen())
	{
		//restart time
		Time dt = clock.restart();
		//total time game.
		m_GameTimeTotal += dt;
		//delta time
		float dtAsSeconds = dt.asSeconds();

		input();
		update(dtAsSeconds);
		draw();
	}	
}