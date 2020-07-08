#pragma once
#ifndef ENGINE_H_
#define ENGINE_H_

#include"SFML/Graphics.hpp"
#include"TextureHolder.h"
#include"LevelManager.h"
#include"Thomas.h"
#include"Bod.h"

using namespace sf;

class Engine {
private:

	TextureHolder th;
	
	//Thomas and bod
	Thomas m_Thomas;
	Bod m_Bob;

	//manage all the levels
	LevelManager m_LM;

	const int TILE_SIZE = 50;
	const int  VERTS_IN_QUAD = 4;
	const int GRAVITY = 300;
	
	RenderWindow m_Window;
	
	View m_MainView;
	View m_LeftView;
	View m_RightView;

	View m_BGMainView;
	View m_BGLeftView;
	View m_BGRightView;

	View m_HudView;

	Sprite m_BacgroundSprite;
	Texture m_BackgroundTexture;

	bool m_Playing;
	bool m_Character1;
	bool m_SplitScreen;
	
	float m_TimeRemaining;
	Time m_GameTimeTotal;
	bool m_NewLevelRequired = true;

	//The vertex array
	VertexArray m_VALevel;
	//The 2d array
	int ** m_ArrayLevel = nullptr;
	//Texture for the levels Tile
	Texture m_TextureTiles;

	//Private functions
	void input();
	void update(float dtAsSeconds);
	void draw();

	//	Load a new level
	void loadLevel();

	// Run will call all the private functions
	bool detectCollisions(PlayableCharacter& character);
public:

	Engine();

	void run();
};
#endif // !ENGINE_H_
