#include"Player.h"
#include"SFML/Graphics.hpp"
#include"ZombieArena.h"
#include"TextureHolder.h"
#include"Bullet.h"
#include"Pickup.h"
#include<sstream>
#include<iostream>

int main()
{
	//Here is the instance of TextureHolder
	TextureHolder holder;

	//The game will always be in one of four states
	enum class State
	{
		PAUSE, LEVELING_UP, GAME_OVER, PLAYING
	};

	//Start with the GAME_OVER state
	State state = State::GAME_OVER;

	//Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

	// Create a an SFML view	for the main funtion
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	// Here	is our clock for timing everything
	Clock clock;

	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;

	//Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	//Create an instance of the Player class
	Player player;

	//The boundaries of the arena
	IntRect arena;

	//Create the backgroundVertexArray background;
	//Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	// Prepare	 for a horde of zombie
	int numZombies;
	int numZombiesAlive;
	Zombie * zombies = nullptr;

	// 100 bullets should do
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	//When was the fire button last pressed?
	Time lastPressed;

	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25,25);
	VertexArray background;

	//Create	 a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// About the game
	int score = 0;
	int hiScore = 0;

	//Position/Size of some text/elements
	//For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
	spriteGameOver.setScale(Vector2f(0.8,0.7));
	//Create a view for the HUD
	View hudView(FloatRect(0,0, resolution.x, resolution.y));
	//Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(30, 680);
	//Load the font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	//Pause
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \n to continue");
	//Game over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(150, 850);
	gameOverText.setString("Press Enter to play");
	//LEVELING up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(60);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream s;
	s <<"1-Increased rate of fire \n" 
		<<"2-Increased clip size(next reload) \n"
		<<"3-Increased maxd health \n"
		<<"4-Increased run speed \n"
		<<"5-More and beter health pickups \n"
		<<"6-More and better ammo puckups \n";
	levelUpText.setString(s.str());
	//Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(22);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 680);
	//Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
	//Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(22);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1200, 0);
	s << "Hi Score::hiScore";
	hiScoreText.setString(s.str());
	//Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(22);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1100, 680);
	zombiesRemainingText.setString("Zombies: 100");
	//Wave number
	int wave = 0;
	Text waveText;
	waveText.setFont(font);
	waveText.setCharacterSize(22);
	waveText.setFillColor(Color::White);
	waveText.setPosition(900, 680);
	waveText.setString("wave: 0");
	//Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 680);
	healthBar.setScale(0.5f, 0.5f);
	//when did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;
	//How often(in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;
	//The main game loop
	while (window.isOpen())
	{
		Event ev;
		while (window.pollEvent(ev))
		{

			/*
			************
			Handle input
			************
			*/

			if (ev.type == Event::KeyPressed)
			{
				if (ev.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSE;
				}
				else if (ev.key.code == Keyboard::Return && state == State::PAUSE)
				{
					state = State::PLAYING;
					//Reset the clock so there isn't a frame jump
					clock.restart();
				}
				else if (ev.key.code == Keyboard::Return && state == State::LEVELING_UP)
				{
					state = State::GAME_OVER;
				}
				else if (ev.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}
				if (state == State::PLAYING)
				{
					//Reloading
					if (ev.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							// Plently of bullets. Reload.
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if (bulletsSpare > 0)
						{
							//Only few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else
						{
							//More here soon ?

						}
					}
				}
			}
		}//End event polling

		// Handle the player quiting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		// Handle WASD while playing
		if (state == State::PLAYING)
		{
			//Handle the pressing and releasing of the WASD keys.
			if (Keyboard::isKeyPressed(Keyboard::W))
				player.moveUP();
			else
				player.stopUp();

			if (Keyboard::isKeyPressed(Keyboard::A))
				player.moveLeft();
			else
				player.stopLeft();

			if (Keyboard::isKeyPressed(Keyboard::S))
				player.moveDown();
			else
				player.stopDown();

			if (Keyboard::isKeyPressed(Keyboard::D))
				player.moveRight();
			else
				player.stopRight();

			//Fire a bullet
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (gameTimeTotal.asMicroseconds() - lastPressed.asMicroseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					//Pass the center of the player
					//and the center of the crosshair
					//to the shoot function
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}

			}//End fire a bullet
		}//End WASD while playing

		// Handle the LEVELING up state
		if (state == State::LEVELING_UP)
		{
			if (Keyboard::isKeyPressed(Keyboard::Num1))
				state = State::PLAYING;

			if (Keyboard::isKeyPressed(Keyboard::Num2))
				state = State::PLAYING;

			if (Keyboard::isKeyPressed(Keyboard::Num3))
				state = State::PLAYING;

			if (Keyboard::isKeyPressed(Keyboard::Num4))
				state = State::PLAYING;

			if (Keyboard::isKeyPressed(Keyboard::Num5))
				state = State::PLAYING;

			if (Keyboard::isKeyPressed(Keyboard::Num6))
				state = State::PLAYING;

			if (state == State::PLAYING)
			{
				// Prepare the level
				// We will modify the next two line later
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				//Pass the vertex array by reference
				//to the createBackground function
				int tileSize = createBackground(background, arena);

				// We will modify this line of code player	
				// int tileSize = 50;

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				//Configure the pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//Create a horde of zombie
				numZombies = 10;

				//Delete the previously  allocated menory(if it  exits)
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				// Reset the clock so there isn't a frame jump
				clock.restart();
			}
		}//End LEVELING UP

		/*
		****************
		UPDATE THE FRAME
		****************
		*/
		if (state == State::PLAYING)
		{
			//Update the delta time
			Time dt = clock.restart();

			//Update the total game time
			gameTimeTotal += dt;

			//Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			//Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert mouse position to World coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			//set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			//Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around	 the player
			mainView.setCenter(player.getCenter());

			// Loop through each Zombie and update them
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			//Update any bullets that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
					bullets[i].update(dtAsSeconds);
			}

			//Update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			// Collision detection
			// Have any zombies been shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() &&
						zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects
						(zombies[j].getPosition()))
						{
							// Stop the bullet
							bullets[i].stop();
							// Register the hit and see if it was a kill
							if (zombies[j].hit())
							{
								// Not just a hit but a kill too
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}
								numZombiesAlive--;
								// When all the zombies are dead (again)
								if (numZombiesAlive == 0)
								{
									state = State::LEVELING_UP;
								}
							}
						}
					}
				}
			}// End zombie being shot

			// Have any zombies touched the player
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects
				(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{
						// More here later
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
					}
				}
			}// End player touched

			// Has the player touched health pickup
			if (player.getPosition().intersects
			(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
			}
			// Has the player touched ammo pickup
			if (player.getPosition().intersects
			(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
			}

			// Size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			//Increment the number of frame since the previous update
			framesSinceLastHUDUpdate++;
			if (framesSinceLastHUDUpdate >= fpsMeasurementFrameInterval)
			{
				//Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				//Update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());
				//Update the score text
				ssScore << "Score : " << score;
				scoreText.setString(ssScore.str());
				//Update the high score text
				ssHiScore << "Hi score : " << hiScore;
				hiScoreText.setString(ssHiScore.str());
				//Update the wave
				ssWave << "Wave :" << wave;
				waveText.setString(ssWave.str());
				//Update the zombies alives text
				ssZombiesAlive << "Zombies: " << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;

			}//End HUD update

		}// End updating the scene

		 /*
		 **************
		 Draw the scene
		 **************
		 */

		if (state == State::PLAYING)
		{
			window.clear();

			//set the mainview to be displayed in the window
			//And draw everything related to it
			window.setView(mainView);

			//Draw the background
			window.draw(background, &textureBackground);

			//Draw the zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}

			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			//Draw the player
			window.draw(player.getSprite());

			//Draw the pickups, if currently spawned
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}

			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}

			//Draw the crosshair
			window.draw(spriteCrosshair);

			//Switch to the HUD view
			window.setView(hudView);

			//Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveText);
			window.draw(zombiesRemainingText);

		}

		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSE)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		
		window.display();

	}// End game loop

	//Delete the previously allocated memory (if it exits)
	delete[] zombies;

	return 0;
}