#include <ctime>
#include <SFML/Graphics.hpp>
#include <string>
#include "Bullet.h"
#include "addons.h"
using namespace sf;

// Player class that handles player movement, shooting, and power-ups
class Player {
public:
	// Core player properties
	Texture tex;
	Sprite sprite;
	float speed = 1.2f;
	int x, y;
	
	// Combat and power-up related properties
	Bullet* bullet;
	Addons* addons[100];
	int addonCount;
	float timeSinceLastDrop;
	bool powerUp;
	Clock powerUpClock;
	bool invulnerable;
	bool fireAdd;
	Clock fireAddClock;

	// Constructor initializes player with texture and default values
	Player(const std::string& texturePath) {
		tex.loadFromFile(texturePath);
		sprite.setTexture(tex);
		x = 340; y = 700;
		sprite.setPosition(x, y);
		bullet = new Bullet();
		addonCount = 0;
		timeSinceLastDrop = 30.0f;
		powerUp = false;
		invulnerable = false;
		fireAdd = false;
	}

	// Reset player position to starting point
	void resetPosition() {
		sprite.setPosition(340, 700);
	}

	// Calculate time since last addon drop
	float getTimeSinceLastDrop() {
		static std::clock_t lastDropTime = std::clock();
		std::clock_t currentTime = std::clock();
		float deltaTime = static_cast<float>(currentTime - lastDropTime) / CLOCKS_PER_SEC;
		lastDropTime = currentTime;
		return deltaTime;
	}

	// Handle dropping of power-ups and addons
	void dropAddons() {
		static float timeSinceLastDrop = 0.0f;
		static const float dropInterval = 10.0f;

		float deltaTime = getTimeSinceLastDrop();
		timeSinceLastDrop += deltaTime;

		if (timeSinceLastDrop >= dropInterval) {
			timeSinceLastDrop = 0.0f;
			int randomX = rand() % 750;
			int randomAddon = rand() % 4;

			switch (randomAddon) {
			case 0:
				addons[addonCount] = new PowerUp(randomX, 0);
				break;
			case 1:
				addons[addonCount] = new Fire(randomX, 0);
				break;
			case 2:
				addons[addonCount] = new Danger(randomX, 0);
				break;
			case 3:
				addons[addonCount] = new LivesAddon(randomX, 0);
				break;
			default:
				break;
			}
			addonCount++;
		}
	}

	// Cleanup resources
	~Player() {
		delete bullet;
	}

	// Handle screen wrapping for player movement
	void wrapAround() {
		float posX = sprite.getPosition().x;
		float posY = sprite.getPosition().y;
		if (posX < 0)
			posX += 780;
		else if (posX > 780)
			posX -= 780;
		if (posY < 0)
			posY += 780;
		else if (posY > 780)
			posY -= 780;
		sprite.setPosition(posX, posY);
	}

	// Handle player movement in different directions
	void move(std::string s) {
		float delta_x = 0, delta_y = 0;
		if (s == "l")
			delta_x = -1;
		else if (s == "r")
			delta_x = 1;
		if (s == "u")
			delta_y = -1;
		else if (s == "d")
			delta_y = 1;

		if (s == "dr") {
			delta_x = 1;
			delta_y = 1;
		}

		if (s == "dl") {
			delta_x = -1;
			delta_y = 1;
		}

		if (s == "ur") {
			delta_x = 1;
			delta_y = -1;
		}
		delta_x *= speed;
		delta_y *= speed;

		sprite.move(delta_x, delta_y);
	}

	// Render player sprite to window
	void render(RenderWindow& window) {
		window.draw(sprite);
	}

private:
	Texture texture;
};