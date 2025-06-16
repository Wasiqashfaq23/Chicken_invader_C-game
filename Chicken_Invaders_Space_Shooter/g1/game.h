#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "menu.h"
using namespace std;
#include "player.h"
#include "Bomb.h"
#include "beam.h"
#include "enemy.h"
#include "level.h"
#include "lives.h"
#include "HighScore.h"
#include "badge.h"

const char title[] = "Chicken Invaders";
using namespace sf;

class Game {
public:
	Sprite background; // Game background sprite
	Texture bg_texture; // Texture for the background
	Player* p; // Pointer to the player object
	Texture explosionTexture; // Texture for explosion effect
	Sprite explosionSprite; // Sprite for explosion effect

	// Ship color
	string currentShipColor;

	// Add array for active bombs
	static const int MAX_ACTIVE_BOMBS = 100;
	Bomb* activeBombs[MAX_ACTIVE_BOMBS];
	int activeBombCount = 0;
	sf::Clock gameClock; // Add game clock for deltaTime

	Level* levels[3]; // Array of level pointers
	Level* currentLevel; // Pointer to the current level
	int currentLevelIndex; // Index of the current level

	Lives* lives; // Pointer to the lives object
	Texture heartTexture; // Texture for heart icons
	Sprite hearts[3]; // Array of heart sprites
	float timeSinceLastHit; // Time since the player was last hit
	string enteredName; // Name entered by the player
	int phases[3] = { 3, 3, 6 }; // Number of phases for each level
	Clock hitClock; // Clock for tracking hit timing

	bool shake; // Flag for screen shake effect
	Clock shakeClock; // Clock for screen shake timing
	Sound firesound; // Sound for firing
	Sound monsterbeam; // Sound for monster beam
	Sound hitSound; // Sound for bullet hitting enemy
	SoundBuffer buffer; // Buffer for sound
	SoundBuffer hitBuffer; // Buffer for hit sound

	SoundBuffer buffer2; // Buffer for second sound
	Sound destroyed; // Sound for destruction
	SoundBuffer buffer3; // Buffer for third sound

	// Existing ship textures
	Texture playerShipOriginal;
	Texture playerShipUpRight;
	Texture playerShipUpLeft;
	Texture playerShipDownRight;
	Texture playerShipDownLeft;
	Texture playerShipDown;
	Texture playerShipRight;
	Texture playerShipLeft;

	// Blue ship textures
	Texture playerShipOriginal_blue;
	Texture playerShipUpRight_blue;
	Texture playerShipUpLeft_blue;
	Texture playerShipDownRight_blue;
	Texture playerShipDownLeft_blue;
	Texture playerShipDown_blue;
	Texture playerShipRight_blue;
	Texture playerShipLeft_blue;
	Monster* monster; // Pointer to the monster object
	bool monsterDefeated; // Flag for monster defeat
	int monsterhits; // Number of hits on the monster
	bool musicStarted; // Flag for music start

	// Cooldown variables for shooting
	float dragonShootCooldown; // Cooldown for dragon shooting
	float monsterShootCooldown; // Cooldown for monster shooting
	const float MONSTER_SHOOT_INTERVAL = 0.5f; // Interval for monster shooting
	const int MAX_DRAGON_BULLETS = 15; // Maximum number of dragon bullets
	const int MAX_MONSTER_BULLETS = 8; // Maximum number of monster bullets

	const int MENU = 0; // Menu state
	const int PLAYING = 1; // Playing state
	const int PAUSED = 2; // Paused state
	float delay; // Delay between key presses
	static const int GAME_OVER = 3; // Game over state
	const int HIGH_SCORES = 4; // High scores state
	const int INSTRUCTIONS = 5; // Instructions state
	Dragon* dragon; // Pointer to the dragon object
	sf::Clock dragonClock; // Clock for dragon timing
	Menu menu; // Menu object
	int gameState; // Current game state
	Menu pauseMenu; // Pause menu object
	sf::Font font; // Font for text
	sf::Texture gameOverBackgroundTexture; // Texture for game over background
	sf::Sprite gameOverBackgroundSprite; // Sprite for game over background


	HighScore highScore; // High score object
	string playerName; // Player's name
	Sprite background1; // First background sprite
	Sprite background2; // Second background sprite
	bool nameEntered; // Flag for name entry
	bool highScoreAdded; // Flag for high score addition
	float dragonDelay = 2.0f; // Delay for dragon appearance
	int score; // Player's score

	Text scoreText; // Text for score display
	sf::Clock monsterClock; // Clock for monster timing
	Music music; // Music object
	Music gameOverMusic; // Game over music object
	Badge goldBadge{ "img/gold.png" }; // Gold badge
	Badge silverBadge{ "img/silver.png" }; // Silver badge
	Badge bronzeBadge{ "img/bronze.png" }; // Bronze badge
	Clock explosionClock; // Clock for explosion timing
	bool explode; // Flag for explosion

	const int WINDOW_WIDTH = 780;
	const int WINDOW_HEIGHT = 780;

	bool moved = false;

	int monsterWavesDefeated = 0; // Counter for defeated monster waves

	sf::RenderWindow window; // Game window
	sf::Texture backgroundTexture, playerTexture, enemyTexture, bulletTexture, bombTexture; // Textures
	sf::Sprite player, enemy, bullet, bomb; // Sprites

	float vulnerabilityTimer; // Add timer for vulnerability period
	bool isVulnerable; // Add flag for vulnerability state

	float spawnDelayTimer; // Add timer for spawn delay
	bool waitingForSpawn; // Add flag for spawn delay state

	Game() : highScore("assets/highscores.txt")
	{
		// Initialize textures
		backgroundTexture.loadFromFile("img/chickenback.png"); // Load the background texture
		playerTexture.loadFromFile("img/player_ship.png"); // Load the player texture
		enemyTexture.loadFromFile("img/enemy_1.png"); // Load the enemy texture
		bulletTexture.loadFromFile("img/enemy_laser.png"); // Load the bullet texture
		bombTexture.loadFromFile("img/bomb.png"); // Load the bomb texture

		// Set textures to sprites
		background.setTexture(backgroundTexture); // Set the background texture
		player.setTexture(playerTexture); // Set the player texture
		enemy.setTexture(enemyTexture); // Set the enemy texture
		bullet.setTexture(bulletTexture); // Set the bullet texture
		bomb.setTexture(bombTexture); // Set the bomb texture

		// Set initial positions
		player.setPosition(400, 500); // Set the player's initial position
		enemy.setPosition(400, 100); // Set the enemy's initial position

	

		// Load ship textures based on color
		playerShipOriginal.loadFromFile("img/player_ship.png");
		playerShipUpRight.loadFromFile("img/1.png");
		playerShipUpLeft.loadFromFile("img/2.png");
		playerShipDownRight.loadFromFile("img/1.png");
		playerShipDownLeft.loadFromFile("img/2.png");
		playerShipRight.loadFromFile("img/player_right.png");
		playerShipLeft.loadFromFile("img/player_left.png");

		// Initialize player with correct ship texture
		p = new Player("img/player_ship.png");

		bg_texture.loadFromFile("img/chickenback.png");
		background1.setTexture(bg_texture);
		background2.setTexture(bg_texture);
		background1.setPosition(0, 0); //set first sprite at top
		background2.setPosition(0, -780); //set second sprite above first
		buffer.loadFromFile("audio/shot.mp3"); //set music
		firesound.setBuffer(buffer);
		firesound.setVolume(80.0f);  // Set fire sound to 50%

		hitBuffer.loadFromFile("audio/hit.mp3"); // Load hit sound
		hitSound.setBuffer(hitBuffer);
		hitSound.setVolume(140.0f);  // Set hit sound to 140%

		buffer2.loadFromFile("audio/beam.wav");
		monsterbeam.setBuffer(buffer2);
		monsterbeam.setVolume(50.0f);  // Set monster beam sound to 50%

		buffer3.loadFromFile("audio/destroyed.wav");
		destroyed.setBuffer(buffer3);
		destroyed.setVolume(50.0f);  // Set destroyed sound to 50%
		explosionTexture.loadFromFile("img/explosion.png");
		explosionSprite.setTexture(explosionTexture);

		// Initialize cooldowns
		dragonShootCooldown = 0.0f;
		monsterShootCooldown = 0.0f;

		// Initialize and start music
		music.openFromFile("audio/chicken_music.mp3");
		music.setVolume(25.0f);  // Set volume to 25% of maximum
		gameOverMusic.openFromFile("audio/gameover.mp3");
		gameOverMusic.setVolume(60.0f);  // Set game over music volume to 60%
		music.play();
		musicStarted = true;

		levels[0] = new Level1();
		levels[1] = new Level2();
		levels[2] = new Level3();
		currentLevelIndex = 0;
		currentLevel = levels[currentLevelIndex];
		lives = new Lives();

		heartTexture.loadFromFile("img/heart.png");
		for (int i = 0; i < 3; i++) {
			hearts[i].setTexture(heartTexture);
			hearts[i].setPosition(700 - i * 50 + 20, 10);
		}
		timeSinceLastHit = 0;
		shake = false;
		gameState = MENU;
		delay = 0.2;

		// Initialize pause menu options
		pauseMenu.options[0].setString("Resume");
		pauseMenu.options[1].setString("High Scores");
		pauseMenu.options[2].setString("Exit");
		pauseMenu.options[3].setString("Instructions");
		pauseMenu.options[4].setString("Quit");

		font.loadFromFile("assets/arial.ttf");

		score = 0;

		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(Color::White);
		scoreText.setPosition(10, 10);

		highScore.loadHighScores();
		nameEntered = false;
		highScoreAdded = false;

		monster = new Monster(50.0f, 50.0f);
		monsterDefeated = false;
		monsterhits = 0;

		dragon = new Dragon(250.0f, 50.0f, p);
		explode = false;

		gameOverBackgroundTexture.loadFromFile("img/background.PNG");
		gameOverBackgroundSprite.setTexture(gameOverBackgroundTexture);

		vulnerabilityTimer = 0.0f;
		isVulnerable = false;

		spawnDelayTimer = 0.0f;
		waitingForSpawn = false;
	}

	float generateRandomInterval() {
		return 0.5f + static_cast<float>(rand() % 150) / 100.0f;  // 0.5 - 2.0s range
	}

	bool checkIntersection(Sprite s1, Sprite s2)
	{
		float s1PositionX = s1.getPosition().x;
		float s1PositionY = s1.getPosition().y;
		float s2PositionX = s2.getPosition().x;
		float s2PositionY = s2.getPosition().y;
		float s1SizeX = s1.getTexture()->getSize().x;
		float s1SizeY = s1.getTexture()->getSize().y;
		float s2SizeX = s2.getTexture()->getSize().x;
		float s2SizeY = s2.getTexture()->getSize().y;

		if (s1PositionX < s2PositionX + s2SizeX &&
			s1PositionX + s1SizeX > s2PositionX &&
			s1PositionY < s2PositionY + s2SizeY &&
			s1PositionY + s1SizeY > s2PositionY) {
			return true;
		}
		return false;
	}

	void handleMonsterCollisions() {
		if (monster->isOnScreen && !monsterDefeated) {
			// Check player bullets hitting monster
			for (int j = 0; j < p->bullet->fireCount; j++) {
				if (checkIntersection(p->bullet->fireSprites[j], monster->sprite)) {
					hitSound.play();  // Play hit sound
					monsterhits++;
					score = score + 5;
					if (monsterhits >= 8) {
						monsterDefeated = true;
						monster->isOnScreen = false;
						monsterhits = 0;
						monster->fireCount = 0; // Clear all monster bullets when defeated
					}
					// Remove the bullet that hit
					for (int k = j; k < p->bullet->fireCount - 1; k++) {
						p->bullet->fireSprites[k] = p->bullet->fireSprites[k + 1];
					}
					p->bullet->fireCount--;
					j--;
				}
			}

			// Check monster colliding with player
			if (checkIntersection(p->sprite, monster->sprite)) {
				if (timeSinceLastHit > 0.5) {
					destroyed.play();
					lives->decreaseLives();
					shake = 1;
					shakeClock.restart();
					cout << "Lives: " << lives->getLives() << std::endl;
					timeSinceLastHit = 0;
				}
			}

			// Check monster bullets hitting player
			for (int i = 0; i < monster->fireCount; i++) {
				if (checkIntersection(p->sprite, monster->fireSprites[i])) {
					if (timeSinceLastHit > 0.5) {
						destroyed.play();
						lives->decreaseLives();
						shake = 1;
						shakeClock.restart();
						cout << "Lives: " << lives->getLives() << endl;
						timeSinceLastHit = 0;
					}
				}
			}
		}
	}

	void handleDragonCollisions() {
		if (dragon->isOnScreen) {
			// Check dragon beam hitting player
			for (int i = 0; i < dragon->fireCount; i++) {
				if (checkIntersection(p->sprite, (dragon->fireSprites[i]))) {
					hitSound.play();  // Play hit sound
					if (timeSinceLastHit > 0.5) {
						destroyed.play();
						lives->decreaseLives();
						shake = 1;
						shakeClock.restart();
						cout << "Lives: " << lives->getLives() << endl;
						timeSinceLastHit = 0;
					}
				}
			}

			// Check player bullets hitting dragon
			for (int j = 0; j < p->bullet->fireCount; j++) {
				if (checkIntersection(p->bullet->fireSprites[j], dragon->sprite)) {
					hitSound.play();  // Play hit sound
					cout << "Dragon hit! Health: " << dragon->health << endl;  // Debug print
					dragon->takeDamage();
					score = score + 10;

					// Remove the bullet that hit
					for (int k = j; k < p->bullet->fireCount - 1; k++) {
						p->bullet->fireSprites[k] = p->bullet->fireSprites[k + 1];
					}
					p->bullet->fireCount--;
					j--;

					// If dragon is dead, update game state
					if (dragon->isDead()) {
						cout << "Dragon defeated!" << endl;  // Debug print
						dragon->isOnScreen = false;
						monster->isOnScreen = false;
						currentLevel->showEnemies = true;
					}
				}
			}
		}
	}

	void handleRegularEnemyCollisions(float deltaTime) {
		for (int i = 0; i < currentLevel->enemyCount; i++) {
			if (!monster->isOnScreen && currentLevel->showEnemies == true && currentLevel->enemies[i]->isAlive) {
				// Check player bullets hitting enemy
				for (int j = 0; j < p->bullet->fireCount; j++) {
					if (checkIntersection(p->bullet->fireSprites[j], currentLevel->enemies[i]->sprite)) {
						// Play hit sound when bullet hits enemy
						hitSound.play();

						// Instead of deleting the bomb, add it to active bombs if it exists
						if (currentLevel->enemies[i]->bomb != nullptr) {
							if (activeBombCount < MAX_ACTIVE_BOMBS) {
								activeBombs[activeBombCount++] = currentLevel->enemies[i]->bomb;
								currentLevel->enemies[i]->bomb = nullptr; // Detach from enemy but don't delete
							}
						}
						int earnedScore = currentLevel->enemies[i]->score;
						currentLevel->removeEnemy(i);
						score += earnedScore;
						if (!checkIntersection(p->bullet->fireSprites[j], p->sprite)) {
							for (int k = j; k < p->bullet->fireCount - 1; k++) {
								p->bullet->fireSprites[k] = p->bullet->fireSprites[k + 1];
							}
							p->bullet->fireCount--;
							j--;
						}
						break;
					}
				}

				// Check enemy bombs hitting player
				if (currentLevel->enemies[i]->bomb != nullptr && currentLevel->showEnemies) {
					if (checkIntersection(currentLevel->enemies[i]->bomb->sprite, p->sprite)) {
						if (timeSinceLastHit > 0.5) {
							explosionClock.restart();
							hitClock.restart();
							shake = 1;
							shakeClock.restart();
							lives->decreaseLives();
							destroyed.play();
							cout << "Lives: " << lives->getLives() << endl;
							timeSinceLastHit = 0;
						}
						delete currentLevel->enemies[i]->bomb;
						currentLevel->enemies[i]->bomb = nullptr;
					}
				}

				// Check enemy colliding with player
				if (currentLevel->showEnemies && currentLevel->enemies[i]->isAlive) {
					if (checkIntersection(p->sprite, currentLevel->enemies[i]->sprite)) {
						if (timeSinceLastHit > 0.5) {
							destroyed.play();
							lives->decreaseLives();
							shake = 1;
							shakeClock.restart();
							cout << "Lives: " << lives->getLives() << endl;
							timeSinceLastHit = 0;
						}
					}
				}
			}
		}

		// Handle active bombs - ONLY check collisions, don't move them
		int j = 0;
		for (int i = 0; i < activeBombCount; i++) {
			if (activeBombs[i] != nullptr) {
				// Check if bomb hits player
				if (checkIntersection(p->sprite, activeBombs[i]->sprite)) {
					if (timeSinceLastHit > 0.5) {
						explosionClock.restart();
						hitClock.restart();
						shake = 1;
						shakeClock.restart();
						lives->decreaseLives();
						destroyed.play();
						cout << "Lives: " << lives->getLives() << endl;
						timeSinceLastHit = 0;
					}
					delete activeBombs[i];
					activeBombs[i] = nullptr;
					continue;
				}

				// Keep valid bombs
				if (j != i) {
					activeBombs[j] = activeBombs[i];
				}
				j++;
			}
		}
		activeBombCount = j;
	}

	void checkCollisions() {
		float deltaTime = gameClock.restart().asSeconds();
		handleRegularEnemyCollisions(deltaTime);
		handleMonsterCollisions();
		handleDragonCollisions();
	}

	void checkAddonCollisions() {
		for (int i = 0; i < p->addonCount; i++) {
			if (checkIntersection(p->sprite, p->addons[i]->sprite)) {
				if (p->addons[i]->type == "PowerUp") {
					p->powerUp = true;
					p->invulnerable = true;
					p->powerUpClock.restart();
				}
				else if (p->addons[i]->type == "Fire") {
					p->fireAdd = true;
					p->fireAddClock.restart();
				}
				else if (p->addons[i]->type == "Danger")
				{
					if (timeSinceLastHit > 0.5) {
						lives->decreaseLives();
						shake = 1;
						shakeClock.restart();
						cout << "Lives: " << lives->getLives() << endl;
						timeSinceLastHit = 0;
					}
				}
				else if (p->addons[i]->type == "LivesAddon")
				{
					if (lives->getLives() < 3)
						lives->increaseLives();
				}

				// Remove addon from array
				for (int j = i; j < p->addonCount - 1; j++) {
					p->addons[j] = p->addons[j + 1];
				}
				p->addonCount--;
				i--;
			}
		}
	}

	void nextLevel() {
		currentLevelIndex++;
		if (currentLevelIndex < 3) {
			currentLevel = levels[currentLevelIndex];
			dragon->health = dragon->MAX_HEALTH; // Ensure dragon can reappear in new level
			dragon->reset();
			dragon->isOnScreen = false; // Only appear when triggered
		}
		else
		{
			gameState = GAME_OVER;
			music.stop();  // Immediately stop game music
			gameOverMusic.play();  // Immediately start game over music
			gameOverMusic.setLoop(true);
		}
	}

	string getUserName(sf::RenderWindow& window)
	{
		sf::Texture backgroundTexture;
		backgroundTexture.loadFromFile("img/menu_background.jpg");

		// create background sprite
		sf::Sprite background(backgroundTexture);
		// draw background
		std::string name;
		sf::Font font;
		font.loadFromFile("assets/arial.ttf");
		sf::Text text("", font);
		text.setCharacterSize(56);
		text.setFillColor(sf::Color::White);
		text.setPosition(100, 250);
		//gameOverBackgroundTexture.loadFromFile("img/background.PNG");
		//gameOverBackgroundSprite.setTexture(gameOverBackgroundTexture);
		sf::Vector2u windowSize = window.getSize();
		sf::Vector2u textureSize = gameOverBackgroundTexture.getSize();
		float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
		float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
		gameOverBackgroundSprite.setScale(1, 1);
		gameOverBackgroundSprite.setPosition(0, 0);
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::TextEntered)
				{
					if (event.text.unicode == '\b' && name.size() > 0)
						name.erase(name.size() - 1, 1);
					else if (event.text.unicode < 128 && event.text.unicode != '\b')
						name += static_cast<char>(event.text.unicode);
				}
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Enter)
					{
						return name;
					}
				}
			}
			text.setString("Enter username: ""\n" "\t" + name);
			window.clear();
			window.draw(background);

			window.draw(text);
			window.display();
		}
		return name;
	}


	void Instructions(RenderWindow& window)
	{
		Texture backgroundTexture;
		if (!backgroundTexture.loadFromFile("img/menu_background.jpg")) {
			// Handle error
		}

		// Create background sprite
		Sprite background(backgroundTexture);

		// Load font
		Font font;
		if (!font.loadFromFile("assets/arial.ttf")) {
			// Handle error
		}

		// Set up text
		sf::Text text("", font);
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);
		text.setPosition(20, 10);

		string instructions = "\n\n\n\n\n\n\n\n\n\n\n"
			"1.The spaceship can be moved in all directions.\n"
			"2.Firing is automatic. The spaceship will be destroyed\n"
			"  if it collides with an object (enemy ships)\n"
			"  or is hit by a bomb fired by an enemy ship.\n"
			"3.The dragon appears at random intervals and remains in a fixed position,\tbut it can fire in three directions:\n"
			"  downward, right-downward,\n"
			"  and left-downward. The direction of its attack\n"
			"  is determined based on the current location of the spaceship,\n"
			"  i.e., the dragon targets the zone where the spaceship is located.\n"
			"4.On the other hand, the monster's vertical position is fixed,\n"
			"  but it moves horizontally (left and right).\n"
			"5.At regular intervals, the monster will release a beam of lightning\n"
			"  toward the spaceship.\n"
			"\n\n\n\n\n\n\n"
			"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tHave fun.";

		text.setString(instructions);

		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
					return;  // Exit the function if window is closed
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
					return;  // Return to the menu if Escape is pressed
				}
			}

			window.clear();
			window.draw(background);
			window.draw(text);
			window.display();
		}
	}



	void start_game() {
		srand(time(0));
		RenderWindow window(VideoMode(780, 780), title);
		Clock clock;
		Clock clock2;
		float timer = 0;
		while (window.isOpen()) {
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;

			Event e;
			while (window.pollEvent(e)) {
				if (e.type == Event::Closed) // If cross/close is clicked/pressed
					window.close(); //close the game
			}
			if (gameState == MENU) {
				// handle menu input
				if (clock2.getElapsedTime().asSeconds() > delay) {
					if (Keyboard::isKeyPressed(Keyboard::Up)) {
						menu.moveUp();
						clock2.restart();
					}
					if (Keyboard::isKeyPressed(Keyboard::Down)) {
						menu.moveDown();
						clock2.restart();
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Return) && menu.canProcessEnterKey()) {
					int selected = menu.select();
					if (selected == 0) { // "Start Game" option
	
						gameState = PLAYING;
					}
					if (selected == 1) //resume game 
						gameState = PLAYING;

					if (selected == 2) { // "Show High Score" option
						gameState = HIGH_SCORES;
					}
					if (selected == 3)
						gameState = INSTRUCTIONS;
					
					if (selected == 4)
						window.close(); //close the game

					menu.resetEnterKeyCooldown();
				}


			}

			if (gameState == PLAYING && nameEntered == false)
			{
				playerName = getUserName(window);
				nameEntered = true;
			}

			else if (gameState == PLAYING && nameEntered == true)
			{
				if (Keyboard::isKeyPressed(Keyboard::Left)) { p->move("l"); moved = true; }
				if (Keyboard::isKeyPressed(Keyboard::Right)) { p->move("r"); moved = true; }
				if (Keyboard::isKeyPressed(Keyboard::Up)) { p->move("u"); moved = true; }
				if (Keyboard::isKeyPressed(Keyboard::Down)) { p->move("d"); moved = true; }

				if (moved)
					p->sprite.setTexture(playerShipOriginal);

				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))) {
					p->sprite.setTexture(playerShipUpRight);
				}
				else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))) {
					p->sprite.setTexture(playerShipUpLeft);
				}
				else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))) {
					p->sprite.setTexture(playerShipDownRight);
				}
				else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))) {
					p->sprite.setTexture(playerShipDownLeft);

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
					p->sprite.setTexture(playerShipRight);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
					p->sprite.setTexture(playerShipLeft);
				}


				if (Keyboard::isKeyPressed(Keyboard::Escape))
				{
					gameState = PAUSED;
				}

				if (lives->getLives() == 0) {
					gameState = GAME_OVER;
					music.stop();  // Immediately stop game music
					gameOverMusic.play();  // Immediately start game over music
					gameOverMusic.setLoop(true);
				}

				if (!Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Right) && !Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Down) && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D))
					p->sprite.setTexture(playerShipOriginal);


				////////////////////////////////////////////////
				///// Call your functions here               ///
				//////////////////////////////////////////////

				bool allEnemiesDefeated = true;
				for (int i = 0; i < currentLevel->enemyCount; i++) {
					if (currentLevel->enemies[i]->isAlive) {
						allEnemiesDefeated = false;
						break;
					}
				}
				if (allEnemiesDefeated) {
					// Clear all bombs when all enemies are defeated
					activeBombCount = 0;  // Reset active bomb count
					for (int i = 0; i < currentLevel->enemyCount; i++) {
						if (currentLevel->enemies[i]->bomb != nullptr) {
							delete currentLevel->enemies[i]->bomb;
							currentLevel->enemies[i]->bomb = nullptr;
						}
					}

					if (!waitingForSpawn) {
						waitingForSpawn = true;
						spawnDelayTimer = 0.0f;
						currentLevel->showEnemies = false;
					}
				}

				// Update spawn delay timer
				if (waitingForSpawn) {
					spawnDelayTimer += time;
					if (spawnDelayTimer >= 2.0f) {
						waitingForSpawn = false;
						currentLevel->showEnemies = true;

						if (currentLevel->phase < phases[currentLevelIndex]) {
							currentLevel->nextPhase();
							resetPlayerPosition();
						}
						else {
							nextLevel();
							resetPlayerPosition();
						}
					}
				}

				float deltaTime = clock.restart().asSeconds();
				if (timeSinceLastHit < 0.5f) {
					timeSinceLastHit += deltaTime;
				}

				for (int i = 0; i < currentLevel->enemyCount; i++) {
					currentLevel->enemies[i]->move(0.05);

					// Only move bombs that are still attached to enemies
					if (currentLevel->enemies[i]->bomb != nullptr && currentLevel->enemies[i]->isAlive) {
						currentLevel->enemies[i]->bomb->move(deltaTime);

						// Check if the bomb has gone off the screen
						if (currentLevel->enemies[i]->bomb->position.y > WINDOW_HEIGHT ||
							currentLevel->enemies[i]->bomb->position.y < 0 ||
							currentLevel->enemies[i]->bomb->position.x > WINDOW_WIDTH ||
							currentLevel->enemies[i]->bomb->position.x < 0) {
							delete currentLevel->enemies[i]->bomb;
							currentLevel->enemies[i]->bomb = nullptr;
						}
					}
				}

				p->wrapAround();
				p->dropAddons();

				checkCollisions();
				checkAddonCollisions();

				// Move active bombs (these are bombs from destroyed enemies)
				for (int i = 0; i < activeBombCount; i++) {
					if (activeBombs[i] != nullptr) {
						activeBombs[i]->move(deltaTime);

						// Check if bomb is off screen
						if (activeBombs[i]->position.y > WINDOW_HEIGHT ||
							activeBombs[i]->position.y < 0 ||
							activeBombs[i]->position.x > WINDOW_WIDTH ||
							activeBombs[i]->position.x < 0) {
							delete activeBombs[i];
							activeBombs[i] = nullptr;
						}
					}
				}

				// Move addons
				for (int i = 0; i < p->addonCount; i++) {
					p->addons[i]->move(deltaTime);

					if (p->addons[i]->positionY > WINDOW_HEIGHT) {
						// Remove addon from array
						for (int j = i; j < p->addonCount - 1; j++) {
							p->addons[j] = p->addons[j + 1];
						}
						p->addonCount--;
						i--;
					}
				}

				// check if it's time for dragon
				if (monsterWavesDefeated >= 2 && !dragon->isOnScreen && currentLevel->showEnemies && !dragon->isDead()) {
					dragon->reset();  // Reset dragon's health and state
					dragon->isOnScreen = true;
					currentLevel->showEnemies = false;  // Hide normal enemies when dragon appears
				}

				// Only update and draw dragon if it is on screen and not dead
				if (dragon->isOnScreen && !dragon->isDead()) {
					dragon->update(time);
				}

				if (monster->isOnScreen) {
					if (monsterShootCooldown >= MONSTER_SHOOT_INTERVAL && monster->fireCount < MAX_MONSTER_BULLETS) {
						monster->dropBomb();
						monsterShootCooldown = 0.0f;
					}
					monster->update(time);

					// Clean up bullets that go off screen
					for (int i = 0; i < monster->fireCount; i++) {
						if (monster->fireSprites[i].getPosition().y > WINDOW_HEIGHT ||
							monster->fireSprites[i].getPosition().y < 0 ||
							monster->fireSprites[i].getPosition().x > WINDOW_WIDTH ||
							monster->fireSprites[i].getPosition().x < 0) {
							// Remove bullet from array
							for (int j = i; j < monster->fireCount - 1; j++) {
								monster->fireSprites[j] = monster->fireSprites[j + 1];
							}
							monster->fireCount--;
							i--;
						}
					}
				}

				// Update cooldowns
				dragonShootCooldown += deltaTime;
				monsterShootCooldown += deltaTime;

				//shake screen
				if (shake) {
					if (shakeClock.getElapsedTime().asSeconds() >= 0.2f) {
						shake = false;

						window.setView(window.getDefaultView());
					}
					else
					{
						int randomX = rand() % 20 - 10;
						int randomY = rand() % 20 - 10;
						sf::View view;
						view.setCenter(400 + randomX, 400 + randomY);
						view.setSize(800, 800);
						window.setView(view);

					}
				}

				std::string fireDirection = "u";  // default

				bool up = Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W);
				bool down = Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S);
				bool left = Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A);
				bool right = Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D);

				if (up && right) fireDirection = "ur";
				else if (up && left) fireDirection = "ul";
				else if (up) fireDirection = "u";

				if (timeSinceLastHit < 0.1)
				{
					if (lives->getLives() < 3) {  // Only show explosion if player lost a life
						p->sprite.setTexture(explosionTexture);
						destroyed.play();
					}
				}

				if (timeSinceLastHit == 0.1)
				{
					p->sprite.setTexture(playerShipOriginal);
				}
				if (p->powerUp)
				{
					if (p->powerUpClock.getElapsedTime().asSeconds() >= 5) {
						p->powerUp = false;
						p->invulnerable = false;
					}
				}

				if (p->fireAdd)
				{
					if (p->fireAddClock.getElapsedTime().asSeconds() >= 5) {
						p->fireAdd = false;
					}
				}

				if (p->powerUp) {
					p->bullet->firePowerUp(time, p->sprite, 0.6);
					p->bullet->firemovePowerUp(0.5);
				}
				else if (p->fireAdd)
				{
					p->bullet->fireAddEffect(time, p->sprite, fireDirection);
					p->bullet->firemove();
				}
				else
				{
					p->bullet->fire(time, p->sprite, fireDirection);
					p->bullet->firemove();
				}
				scoreText.setString("Score: " + to_string(score));

				if (currentLevelIndex == 0 || currentLevelIndex == 1) {
					if (currentLevel->phase == 3 && !monsterDefeated) {
						monster->isOnScreen = true;
						currentLevel->showEnemies = false;
					}
					if (currentLevelIndex == 2)
					{
						if (currentLevel->phase == 6 && !monsterDefeated) {
							monster->isOnScreen = true;
							currentLevel->showEnemies = false;
						}
					}
				}

				if (monsterDefeated && currentLevelIndex < 3) {
					currentLevel->nextPhase();
					monsterDefeated = false;
					currentLevel->showEnemies = true;
					monsterWavesDefeated++; // Increment counter when a monster wave is defeated
				}

				if (monster->isOnScreen)
				{
					monster->move(0.015);
					currentLevel->showEnemies = false;
				}

				// Update vulnerability timer
				if (isVulnerable) {
					vulnerabilityTimer += deltaTime;
					if (vulnerabilityTimer >= 1.5f) {
						isVulnerable = false;
						p->invulnerable = true;
					}
				}

				for (int i = 0; i < currentLevel->enemyCount; i++) {
					if (!monster->isOnScreen) {
						currentLevel->enemies[i]->move(deltaTime);
					}
				}


			}

			else if (gameState == PAUSED) {
				// handle pause menu input
				if (clock2.getElapsedTime().asSeconds() > delay) {
					if (Keyboard::isKeyPressed(Keyboard::Up)) {
						pauseMenu.moveUp();
						clock2.restart();
					}
					if (Keyboard::isKeyPressed(Keyboard::Down)) {
						pauseMenu.moveDown();
						clock2.restart();
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Return) && pauseMenu.canProcessEnterKey()) {
					int selected = pauseMenu.select();
					if (selected == 0) { // "Resume" 
						gameState = PLAYING;
					}
					else if (selected == 1) { // "Highscore" 
						gameState = HIGH_SCORES;
					}
								
					else if (selected == 3) { // "Instructions"
						gameState = INSTRUCTIONS;
					}
					else if (selected == 2) { // "Exit" 
						window.close();
					}
					else if (selected == 4) { // "Quit" 
						
					}
					pauseMenu.resetEnterKeyCooldown();
				}
			}



			window.clear(Color::Black);

			if (gameState == MENU) {
				menu.draw(window);
			}
			else if (gameState == PLAYING) {

				window.clear(Color::Black);

				//window.draw(background); // setting background
				float bgSpeed = 100;
				background1.move(0, bgSpeed * time);
				background2.move(0, bgSpeed * time);

				if (background1.getPosition().y > 780)
				{
					background1.setPosition(0, -780);
				}
				if (background2.getPosition().y > 780)
				{
					background2.setPosition(0, -780);
				}

				window.draw(background1);
				window.draw(background2);
				window.draw(p->sprite);

				for (int i = 0; i < p->bullet->fireCount; i++) {
					window.draw(p->bullet->fireSprites[i]);
					firesound.play();
				}

				if (monster->isOnScreen) {
					// Draw monster with flash effect
					if (monster->isFlashing) {
						monster->sprite.setColor(sf::Color::Red);
					}
					else {
						monster->sprite.setColor(sf::Color::White);
					}
					window.draw(monster->sprite);
					for (int i = 0; i < monster->fireCount; i++) {
						window.draw(monster->fireSprites[i]);
					}
					monster->update(time);

				}

				else {
					for (int i = 0; i < currentLevel->enemyCount; i++) {
						if (currentLevel->enemies[i]->isAlive && currentLevel->showEnemies == 1)
							window.draw(currentLevel->enemies[i]->sprite);
						if (currentLevel->enemies[i]->bomb != nullptr && currentLevel->showEnemies == 1) {
							window.draw(currentLevel->enemies[i]->bomb->sprite);
						}
					}
				}

				for (int i = 0; i < lives->getLives(); i++) {
					hearts[i].setScale(0.75, 0.75);
					window.draw(hearts[i]);
				}

				for (int i = 0; i < p->addonCount; i++) {
					window.draw(p->addons[i]->sprite);
				}

				window.draw(scoreText);
				if (!playerName.empty())
				{
					for (int i = 0; i < 3; i++)
					{
						if (playerName == highScore.highScores[i].name)
						{
							sf::Sprite badgeSprite;
							switch (i)
							{
							case 0:
								badgeSprite = goldBadge.sprite;
								break;
							case 1:
								badgeSprite = silverBadge.sprite;
								break;
							case 2:
								badgeSprite = bronzeBadge.sprite;
								break;
							}
							badgeSprite.setPosition(15, 40);
							window.draw(badgeSprite);
							break;
						}
					}

				}

				// Draw dragon with flash effect
				if (dragon->isOnScreen) {
					if (dragon->isFlashing) {
						dragon->sprite.setColor(sf::Color::Red);
					}
					else {
						dragon->sprite.setColor(sf::Color::White);
					}
					window.draw(dragon->sprite);
					for (int i = 0; i < dragon->fireCount; i++) {
						window.draw(dragon->fireSprites[i]);
					}
				}
				else {
					// Make enemies reappear
					for (int i = 0; i < currentLevel->enemyCount; i++) {
						currentLevel->showEnemies = true;
					}
				}

				// Draw active bombs
				for (int i = 0; i < activeBombCount; i++) {
					if (activeBombs[i] != nullptr) {
						window.draw(activeBombs[i]->sprite);
					}
				}

			}


			else if (gameState == PAUSED) {
				pauseMenu.draw(window);
			}

			else if (gameState == HIGH_SCORES) {
				highScore.displayHighScores(window);

				if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					gameState = MENU;
				}
			}

			else if (gameState == INSTRUCTIONS)
			{
				Instructions(window);
				if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					gameState = MENU;
				}
			}
			if (gameState == GAME_OVER)
			{
				if (!playerName.empty() && !highScoreAdded && highScore.isHighScore(score)) {
					highScore.addHighScore(playerName, score);
					highScoreAdded = true;
				}

				// Remove the status check since we want immediate music transition
				window.clear(sf::Color::Black);

				// Draw the background sprite
				window.draw(gameOverBackgroundSprite);

				scoreText.setCharacterSize(48);
				scoreText.setPosition(280.0f, 10.0f);
				scoreText.setFillColor(sf::Color::White);
				window.draw(scoreText);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					gameOverMusic.stop();  // Immediately stop game over music
					music.play();  // Immediately start game music
					gameState = MENU;
				}
			}

			window.display(); //Displaying the game

		}
	}

	void update(float deltaTime) {
		// ... existing code ...
		if (dragon->isOnScreen) {
			dragon->update(deltaTime);
		}
		// ... existing code ...
	}

	void draw() {
		// ... existing drawing code ...

		// Draw active bombs
		for (int i = 0; i < activeBombCount; i++) {
			if (activeBombs[i] != nullptr) {
				window.draw(activeBombs[i]->sprite);
			}
		}

		// ... rest of drawing code ...
	}

	void resetPlayerPosition() {
		// Reset player to bottom middle of screen
		p->sprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100);
		// Make player vulnerable
		isVulnerable = true;
		vulnerabilityTimer = 0.0f;
		p->invulnerable = false;
	}

private:
	
	//clear bomb when round ends
	void clearAllBombs() {
		// Clear active bombs
		for (int i = 0; i < activeBombCount; i++) {
			if (activeBombs[i] != nullptr) {
				delete activeBombs[i];
				activeBombs[i] = nullptr;
			}
		}
		activeBombCount = 0;

		// Clear enemy bombs
		for (int i = 0; i < currentLevel->enemyCount; i++) {
			if (currentLevel->enemies[i]->bomb != nullptr) {
				delete currentLevel->enemies[i]->bomb;
				currentLevel->enemies[i]->bomb = nullptr;
			}
		}
	}
};