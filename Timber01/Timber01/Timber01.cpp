#include<iostream>
#include"SFML/Graphics.hpp"
#include"SFML/Audio.hpp"
#include<sstream>
using namespace sf;


//Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

//Where is the player/branch?
//Left of right
enum class side{LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];
//-----------------------------------------------------------------------------

int main()
{
	//Create a video mode object
	sf::VideoMode vm(910, 540);

	//Create and open a window for the game RenderWindow
	sf::RenderWindow window(vm, "Timber!!!");

	//Create a texture to hold a graphic on the GPU
	sf::Texture *textureBackground = new Texture;

	//Load a graphic into the texture
	textureBackground->loadFromFile("graphics/background.png");

	//Create a sprite
	sf::Sprite spriteBackground;

	//Attach the texture to the sprite
	spriteBackground.setTexture(*textureBackground);

	//Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	//Set Scale
	spriteBackground.setScale(0.5f, 0.5f);

	// Make a tree sprite
	sf::Texture *textureTree = new Texture;
	textureTree->loadFromFile("graphics/tree.png");
	sf::Sprite spriteTree;
	spriteTree.setTexture(*textureTree);
	spriteTree.setPosition(410, 0);
	spriteTree.setScale(0.5f, 0.5f);

	// Prepare the bee
	sf::Texture *textureBee = new Texture;
	textureBee->loadFromFile("graphics/bee.png");
	sf::Sprite spriteBee;
	spriteBee.setTexture(*textureBee);
	spriteBee.setPosition(0, 400);
	spriteBee.setScale(0.5f, 0.5f);

	//Is the bee currently moving?
	bool beeActive = false;

	//How fast can the bee fly
	float beeSpeed = 0.0f;

	//Make 3 cloud sprites from 1 texture
	sf::Texture *textureCloud = new Texture;
	textureCloud->loadFromFile("graphics/cloud.png");
	sf::Sprite spriteCloud1;
	sf::Sprite spriteCloud2;
	sf::Sprite spriteCloud3;
	spriteCloud1.setTexture(*textureCloud);
	spriteCloud2.setTexture(*textureCloud);
	spriteCloud3.setTexture(*textureCloud);

	//Position the clounds off screen
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 120);
	spriteCloud3.setPosition(0, 240);

	//Set Scale
	spriteCloud1.setScale(0.5f, 0.5f);
	spriteCloud2.setScale(0.5f, 0.5f);
	spriteCloud3.setScale(0.5f, 0.5f);

	//Are the clouds currently on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	//How fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//Variable to control time itself
	Clock clock;

	//Time bar
	RectangleShape timeBar;
	float timeBarWidth = 200;
	float timeBarHeight = 40;
	timeBar.setSize(Vector2f(timeBarWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((910 / 2 - timeBarWidth / 2), 490.f);
	
	Time gameTimeTotal;
	float timeRemaining = 6.f;
	float timeBarWidthPerSecond = timeBarWidth / timeRemaining;

	//Track whether the game is running
	bool paused = true;

	//Draw some text
	int score = 0;

	sf::Text messageText;
	sf::Text scoreText;

	//We need to choose a font
	Font *font = new Font;
	font->loadFromFile("fonts/KOMIKAP_.ttf");

	//Set the font to our message
	messageText.setFont(*font);
	scoreText.setFont(*font);

	//Assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	//Make it really big
	messageText.setCharacterSize(35);
	scoreText.setCharacterSize(35);

	//Choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//Position the text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(
		(float)(textRect.left + textRect.width / 2.f),
		(float)(textRect.top + textRect.height / 2.f)
	);

	messageText.setPosition(910 / 2.0f, 540 / 2.0f);

	scoreText.setPosition(20.f, 20.f);

	//Prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	//Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setScale(0.5f, 0.5f);
		branches[i].setPosition(-1000, -1000);
		//Set the sprite's origin to dead center
		//We can th spin it round without changing its position
		branches[i].setOrigin(220, 40);
	}

	//Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(290,360);
	spritePlayer.setScale(0.5f, 0.5f);
	
	//The player starts on the left
	side playerSide = side::LEFT;

	//Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(3000,415);
	spriteRIP.setScale(0.5f, 0.5f);

	//Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(350,415);
	spriteAxe.setScale(0.5f, 0.5f);

	//Line the axe up with the tree
	const float AXE_POSITION_LEFT = 350;
	const float AXE_POSITION_RIGHT = 535;

	//Prepare the flying Log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(410, 360);
	spriteLog.setScale(0.5f, 0.5f);

	//Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 500;
	float logSpeedY = 750;

	//Control the player input
	bool acceptInput = false;
	
	//Prepare the sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	//death
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	//Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);


	//Event
	Event event;

	//Game loop
	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				//Listen for key presses again
				acceptInput = true;

				//Hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		//Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			//Reset the time and the score
			score = 0;
			timeRemaining = 5.f;

			//Make all the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			//Make sure the gravestore is hidden
			spriteRIP.setPosition(675, 2000);

			//Move the player into position
			spritePlayer.setPosition(290,360);

			acceptInput = true;
		}

		if (acceptInput)
		{
			//More code here next...
			//First handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				//Make sure the player is on the right
				playerSide = side::RIGHT;
				score++;

				//Add to the amount of time remaining
				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(600, 360);

				//update the branches
				updateBranches(score);

				//Set the log flying to the left
				spriteLog.setPosition(410, 360);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				chop.play();
			}

			//Handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				//Make sure the player in on the left
				playerSide = side::LEFT;
				score++;

				//Add to the amount of time remaining
				timeRemaining += (float)(2 / score) + 0.15;
				
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(290, 360);

				//Update the branches
				updateBranches(score);

				//set the log flying
				spriteLog.setPosition(410, 360);

				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				chop.play();
			}
		}

		if (!paused)
		{
			/*
			****************************************
			Update the scene
			****************************************
			*/

			//Measure time
			Time dt = clock.restart();
			
			//Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();

			//Size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.f)
			{
				//Pause the game
				paused = true;

				//Change the message show to the player
				messageText.setString("Out of time!");

				//Reposition the text based on its new size
				FloatRect textRec = messageText.getLocalBounds();
				messageText.setOrigin(
					textRec.left + textRec.width / 2.f,
					textRec.top + textRec.height / 2.f
					);

				messageText.setPosition(910/2.f, 540/2.f);

				//Play the out of time sound
				outOfTime.play();
			}

			//Setup	the bee
			if (!beeActive)
			{
				//How fast is the bee
				srand(static_cast<unsigned>(time(NULL)));
				beeSpeed = (float)(rand() % 100) + 100;

				//How high is the bee
				srand(static_cast<unsigned>(time(NULL)) * 10);
				float height = (float)(rand() % 250) + 250;
				spriteBee.setPosition(1000, height);
				beeActive = true;
			}
			else
			{
				//Move the bee
				spriteBee.setPosition(
					spriteBee.getPosition().x - (beeSpeed * dt.asSeconds())
					, spriteBee.getPosition().y);
				//Has the bee reached the right hand edge of the screen?
				if (spriteBee.getPosition().x < -50)
				{
					//Set it up ready to be a whole	new cloud next frame
					beeActive = false;
				}
			}

			//Manage the clouds
			//cloud 1
			if (!cloud1Active)
			{
				//How fast is the cloud
				srand((int)time(NULL) * 10);
				cloud1Speed = (float)(rand() % 200);

				//How high is the cloud
				srand((int)time(NULL) * 10);
				float height = (float)(rand() % 80);
				spriteCloud1.setPosition(-100, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y);

				//Has the cloud reached the right hand edge of the sreen?
				if (spriteCloud1.getPosition().x > 910)
				{
					//Set it up ready to be a whole new cloud next frame
					cloud1Active = false;
				}
			}

			//Cloud 2
			if (!cloud2Active)
			{
				//how fast is the cloud
				srand((int)time(NULL) * 20);
				cloud2Speed = (float)(rand() % 200);

				//how height is the cloud
				srand((int)time(NULL) * 20);
				float height = (float)((rand() % 150) - 80);
				spriteCloud2.setPosition(-100, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);
				//Has the cloud reached the right hand edge of the sreen?
				if (spriteCloud2.getPosition().x > 910)
				{
					//Set it up ready to be a whole new cloud next frame
					cloud2Active = false;
				}
			}

			//Cloud 3
			if (!cloud3Active)
			{
				// How fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (float)(rand() % 200);
				// How high is the cloud
				srand((int)time(0) * 30);
				float height = (float)((rand() % 250) - 150);
				spriteCloud3.setPosition(-100, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud3.getPosition().x > 910)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud3Active = false;
				}
			}

			//Update the score text	
			std::stringstream ss;
			ss << "Score  = " << score;
			scoreText.setString(ss.str());

			//update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = (float)i * 80;
				if (branchPositions[i] == side::LEFT)
				{
					//Move the sprite to the left side
					branches[i].setPosition(310,height);

					//Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if(branchPositions[i] == side::RIGHT)
				{
					//Move the sprite to the left side
					branches[i].setPosition(670, height);

					//Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					//Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			//Handle a flying log
			if (logActive)
			{
				//setPosition(spriteLog.getPosition().x, logSpeedX * dt);
				spriteLog.setPosition(spriteLog.getPosition().x + logSpeedX * dt.asSeconds(),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				//Has the log reached the right hand edge?
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					//set it up	ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(410, 360);
				}
			}

			//Has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				//Death
				paused = true;
				acceptInput = false;

				if (playerSide == side::LEFT)
				{
					//Draw the gravestore
					spriteRIP.setPosition(90, 380);
				}
				else
				{
					//Draw the gravestore
					spriteRIP.setPosition(410, 380);				
				}
				

				//Hide the player
				spritePlayer.setPosition(2000, 660);

				//Change the text of the message
				messageText.setString("SQUISHED!");

				//Center it on the screen
				FloatRect textRec = messageText.getLocalBounds();

				messageText.setOrigin(textRec.left + textRec.width / 2.f, textRec.top + textRec.height / 2.f);

				messageText.setPosition(910 / 2.f, 540 / 2.f);
				//Play the death sound
				death.play();
			}

		}
		/*
		****************************************
		Draw the scene
		****************************************
		*/
		//Clear everything from the last frame
		window.clear();

		//Draw our game scense here
		window.draw(spriteBackground);

		//Draw the clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		
		//Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		//Draw the tree
		window.draw(spriteTree);
		//Draw the player
		window.draw(spritePlayer);
		//Draw the axe
		window.draw(spriteAxe);
		//Draw the flying log
		window.draw(spriteLog);
		//Draw the gravestone
		window.draw(spriteRIP);
		//Draw the insect
		window.draw(spriteBee);
		//Now draw Score
		window.draw(scoreText);
		//Draw timeBar
		window.draw(timeBar);

		if (paused)
		{
			//Draw our message
			window.draw(messageText);
		}

		//Show everything we just drew
		window.display();
	}
	return 0;
}
//-----------------------------------------------------------------------------

// Function definition

void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}
	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}
