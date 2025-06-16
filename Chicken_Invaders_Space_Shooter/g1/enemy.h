#include <SFML/Graphics.hpp>
#include <string>
#include <cstdlib>
#include <iostream>
using namespace sf;
using namespace std;

// Base class for all enemies
class Enemy {
public:
	Sprite sprite; // Sprite for the enemy
	Texture texture; // Texture for the enemy
	float positionX; // X position of the enemy
	float positionY; // Y position of the enemy
	float speed; // Speed of the enemy
	int score; // Score value of the enemy
	bool isAlive; // Flag to check if the enemy is alive
	float directionX; // Direction of movement on the X-axis
	float directionY; // Direction of movement on the Y-axis
	float timeSinceLastMove; // Time since the last move
	sf::Time timeInterval; // Time interval for actions
	float timeSinceLastBomb; // Time since the last bomb was dropped
	Bomb* bomb; // Pointer to the bomb object

	Enemy() {} // Default constructor

	Enemy(const std::string& texturePath, float x, float y, float speed, int score)
		: positionX(x), positionY(y), speed(speed), score(score), directionX(0.20f), directionY(0.0f),
		timeSinceLastMove(0.0f), timeInterval(sf::seconds(1.0f)), timeSinceLastBomb(0.0f),
		bomb(nullptr) {
		texture.loadFromFile(texturePath); // Load texture from file
		sprite.setTexture(texture); // Set texture for sprite
		sprite.setPosition(positionX, positionY); // Set position of sprite
		isAlive = true; // Set enemy as alive
	}

	// Basic movement and bomb dropping behavior
	virtual void move(float deltaTime) {
		positionX += directionX * speed * deltaTime;
		positionY += directionY * speed * deltaTime;
		sprite.setPosition(positionX, positionY);

		timeSinceLastMove += deltaTime;
		timeSinceLastBomb += deltaTime;

		if (timeSinceLastMove >= 40.0f) {
			timeSinceLastMove = 0.0f;
			directionX = -directionX;
			positionY += 1;
			dropBomb();
		}
	}

	virtual void dropBomb() = 0;
};

// Class for basic invaders
class Invader : public Enemy {
public:
	sf::Clock clock1; // Clock for timing

	Invader(const std::string& texturePath, float x, float y, float speed, int score)
		: Enemy(texturePath, x, y, speed, score), clock1() {} // Constructor

	// Random bomb dropping behavior
	void dropBomb() override {
		sf::Time elapsed = clock1.getElapsedTime();

		if (elapsed >= timeInterval) {
			clock1.restart();

			int random = rand() % 4 + 1;
			if (isAlive) {
				if (rand() % 100 < random * 10 && bomb == nullptr) {
					bomb = new Bomb("img/bomb.png", positionX + sprite.getGlobalBounds().width / 2,
						positionY + sprite.getGlobalBounds().height / 2 + 10,
						600.0f);
				}
			}
		}
	}
};

// Class for alpha invaders
class AlphaInvader : public Invader {
public:
	AlphaInvader(float x, float y)
		: Invader("img/enemy_1.png", x, y, 10.0f, 10) { // Constructor
		timeInterval = sf::seconds(5.0f); // Set time interval
	}
};

// Class for beta invaders
class BetaInvader : public Invader {
public:
	BetaInvader(float x, float y)
		: Invader("img/enemy_2.png", x, y, 25.0f, 20) { // Constructor
		timeInterval = sf::seconds(3.0f); // Set time interval
	}
	void move(float deltaTime) { // Override move function
		positionX += directionX * speed * deltaTime; // Update X position
		positionY += directionY * speed * deltaTime; // Update Y position
		sprite.setPosition(positionX, positionY); // Set new position of sprite

		timeSinceLastMove += deltaTime; // Update time since last move
		timeSinceLastBomb += deltaTime; // Update time since last bomb

		if (timeSinceLastMove >= 30.0f) { // Check if it's time to change direction
			timeSinceLastMove = 0.0f; // Reset time since last move
			directionX = -directionX; // Reverse direction
			dropBomb(); // Drop a bomb
		}
	}
};

// Class for gamma invaders
class GammaInvader : public Invader {
public:
	GammaInvader(float x, float y)
		: Invader("img/enemy_3.png", x, y, 35.0f, 30) { // Constructor
		timeInterval = sf::seconds(2.0f); // Set time interval
	}
	void move(float deltaTime) { // Override move function
		positionX += directionX * speed * deltaTime; // Update X position
		positionY += directionY * speed * deltaTime; // Update Y position
		sprite.setPosition(positionX, positionY); // Set new position of sprite

		timeSinceLastMove += deltaTime; // Update time since last move
		timeSinceLastBomb += deltaTime; // Update time since last bomb

		if (timeSinceLastMove >= 10.0f) { // Check if it's time to change direction
			timeSinceLastMove = 0.0f; // Reset time since last move
			directionX = -directionX; // Reverse direction
			dropBomb(); // Drop a bomb
		}
	}
};

// Class for monsters
class Monster : public Enemy {
public:
	sf::Clock mclock; // Clock for timing
	int beamCount; // Count of beams
	bool isOnScreen; // Flag to check if monster is on screen
	bool isFiring; // Flag to check if monster is firing
	Texture fireTexture; // Texture for fire
	Sprite fireSprites[100]; // Array of fire sprites
	int fireCount = 0; // Count of fires
	float fireTimer = 0.0; // Timer for fire
	float fireInterval = 0.6; // Interval between fires
	float fireSpeed = 0.3; // Speed of fire
	int i = 1; // Counter
	float timeSinceStartedFiring; // Time since firing started
	bool isFlashing; // Flag for flash effect
	sf::Clock flashClock; // Clock for flash duration
	
	Monster(float x, float y)
		: Enemy("img/monster1.png", x, y, 3.0f, 10), beamCount(0), isOnScreen(false), isFiring(false), 
		timeSinceStartedFiring(0.0f), isFlashing(false) { // Constructor
		directionX = 10.0f; // Set direction X
		directionY = 0.0f; // Set direction Y
		fireTexture.loadFromFile("img/enemy_laser2.jpg"); // Load fire texture
	}

	// Complex movement and attack patterns
	void move(float deltaTime) override {
		if (isOnScreen) { // Check if monster is on screen
			if (!isFiring || timeSinceStartedFiring >= 4.0f) { // Check if monster is not firing or firing time is over
				positionX += directionX * speed * deltaTime; // Update X position
				positionY += directionY * speed * deltaTime; // Update Y position
				sprite.setPosition(positionX, positionY); // Set new position of sprite

				timeSinceLastMove += deltaTime; // Update time since last move

				if (positionX < 0 || positionX > 500) { // Check if monster is out of bounds
					timeSinceLastMove = 0.0f; // Reset time since last move
					directionX = -directionX; // Reverse direction
				}
			} else {
				timeSinceStartedFiring += deltaTime; // Update time since firing started
			}

			sf::Time elapsed = mclock.getElapsedTime(); // Get elapsed time

			if (elapsed.asSeconds() >= 6.0f) { // Check if it's time to drop a bomb
				dropBomb(); // Drop a bomb
				mclock.restart(); // Restart the clock
			}

			if (timeSinceStartedFiring >= 4.0f) { // Check if firing time is over
				isFiring = false; // Set firing flag to false
				timeSinceStartedFiring = 0.0f; // Reset time since firing started
			}
		}
	}

	void dropBomb() override { // Override dropBomb function
		if (fireCount >= 100) return; // Check if fire count is at maximum
		
		isFlashing = true; // Set flashing flag to true
		flashClock.restart(); // Restart the flash clock
		
		fireTexture.loadFromFile("img/enemy_laser2.jpg"); // Load fire texture
		fireSprites[fireCount].setTexture(fireTexture); // Set texture for fire sprite

		float posX = sprite.getPosition().x; // Get X position of sprite
		float posY = sprite.getPosition().y; // Get Y position of sprite

		posX += sprite.getGlobalBounds().width / 2 - 13; // Calculate X position for fire
		posY += (sprite.getGlobalBounds().height / 2) + 70; // Calculate Y position for fire

		fireSprites[fireCount].setPosition(posX, posY); // Set position of fire sprite
		fireCount++; // Increment fire count
	}

	void updateFlash() { // Function to update flash effect
		if (isFlashing && flashClock.getElapsedTime().asSeconds() >= 0.1f) { // Check if flashing time is over
			isFlashing = false; // Set flashing flag to false
		}
	}

	void firemove() { // Function to move fire
		int j = 0; // Counter
		for (int i = 0; i < fireCount; i++) { // Loop through fire sprites
			float posX = fireSprites[i].getPosition().x; // Get X position of fire sprite
			float posY = fireSprites[i].getPosition().y; // Get Y position of fire sprite

			posY += fireSpeed * mclock.getElapsedTime().asSeconds(); // Update Y position of fire sprite

			fireSprites[i].setPosition(posX, posY); // Set new position of fire sprite

			if (posX > 0 && posX < 780 && posY > 0 && posY < 780) { // Check if fire sprite is within bounds
				fireSprites[j] = fireSprites[i]; // Move fire sprite
				j++; // Increment counter
			}
		}
		fireCount = j; // Update fire count
	}

	void update(float deltaTime) {
		if (isOnScreen) {
			firemove();
			updateFlash();
		}
	}
};

// Class for dragons
class Dragon : public Enemy {
public:
	// Basic properties
	bool isOnScreen;
	Player* player;
	int health;
	const int MAX_HEALTH = 5;
	
	// Shooting properties
	sf::Clock dClock;
	bool isFiring;
	Texture fireTexture;
	Sprite fireSprites[10000];  // Increased to match new logic
	int fireCount;
	float fireTimer;
	float fireInterval;
	float fireSpeed;
	float timeSinceStartedFiring;
	
	// Visual effects
	bool isFlashing;
	Clock flashClock;
	const float FLASH_DURATION = 0.1f;

	Dragon(float x, float y, Player* player) 
		: Enemy("img/dragon.png", x, y, 3.0f, 10),
		  isOnScreen(false),
		  player(player),
		  health(MAX_HEALTH),
		  fireCount(0),
		  fireTimer(0.0f),
		  fireInterval(0.6f),
		  fireSpeed(0.425f),
		  timeSinceStartedFiring(0.0f),
		  isFiring(false),
		  isFlashing(false) {
		directionX = 10.0f;
		directionY = 0.0f;
		fireTexture.loadFromFile("img/enemy_laser.png");
	}

	void takeDamage() {
		if (!isOnScreen || health <= 0) return; // Prevent further damage if dead
		
		health--;
		std::cout << "Dragon took damage! Health: " << health << std::endl;  // Debug print
		isFlashing = true;
		flashClock.restart();

		if (health <= 0) {
			std::cout << "Dragon health reached 0!" << std::endl;  // Debug print
			isOnScreen = false;
			fireCount = 0;
			health = 0;  // Ensure health stays at 0
		}
	}

	// Complex movement and targeting behavior
	void update(float deltaTime) {
		if (!isOnScreen) return;

		// Update shooting
		fireTimer += deltaTime;
		if (fireTimer >= fireInterval) {
			dropBomb();  // Use the new shooting logic
			fireTimer = 0.0f;
		}

		// Update bullets
		for (int i = 0; i < fireCount; i++) {
			// Move bullet towards player
			float posX = fireSprites[i].getPosition().x;
			float posY = fireSprites[i].getPosition().y;
			
			float playerX = player->sprite.getPosition().x;
			float playerY = player->sprite.getPosition().y;
			
			float dx = playerX - posX;
			float dy = playerY - posY;
			float length = sqrt(dx * dx + dy * dy);
			
			if (length > 0) {
				dx /= length;
				dy /= length;
			}
			
			posX += dx * 300.0f * deltaTime;  // 300 is bullet speed
			posY += dy * 300.0f * deltaTime;
			
			fireSprites[i].setPosition(posX, posY);
		}

		// Remove bullets that are off screen
		int j = 0;
		for (int i = 0; i < fireCount; i++) {
			float posX = fireSprites[i].getPosition().x;
			float posY = fireSprites[i].getPosition().y;
			
			if (posX >= 0 && posX <= 780 && posY >= 0 && posY <= 780) {
				if (i != j) {
					fireSprites[j] = fireSprites[i];
				}
				j++;
			}
		}
		fireCount = j;

		// Update flash effect
		if (isFlashing && flashClock.getElapsedTime().asSeconds() >= FLASH_DURATION) {
			isFlashing = false;
		}
	}

	bool isDead() const {
		return health <= 0;
	}

	void reset() {
		if (isDead()) return;  // Don't reset if already dead
		health = MAX_HEALTH;
		fireCount = 0;
		fireTimer = 0.0f;
		isOnScreen = true;
		isFlashing = false;
	}

	// Advanced targeting and attack pattern
	void dropBomb() override {
		if (!isOnScreen) return;

		//initial pos
		float posX = sprite.getPosition().x;
		float posY = sprite.getPosition().y;

		posX += sprite.getGlobalBounds().width / 2 + 20;
		posY += (sprite.getGlobalBounds().height / 2) + 100;

		//follow direction
		float directionX = 0.0f;
		float directionY = 1.0f;
		float playerPosX = player->sprite.getPosition().x;
		float playerPosY = player->sprite.getPosition().y;
		float dragonPosX = sprite.getPosition().x;
		float dragonPosY = sprite.getPosition().y;
		float diffX = playerPosX - dragonPosX;
		float diffY = playerPosY - dragonPosY;
		directionX = diffX / sqrt(diffX * diffX + diffY * diffY);
		directionY = diffY / sqrt(diffX * diffX + diffY * diffY);

		// Create multiple beams
		for (int i = 0; i < 100 && fireCount < 10000; i++) {
			fireSprites[fireCount].setTexture(fireTexture);
			fireSprites[fireCount].setPosition(posX, posY);
			fireSprites[fireCount].setRotation(atan2(directionY, directionX) * (180 / 3.14));
			fireCount++;
		}
	}
};
