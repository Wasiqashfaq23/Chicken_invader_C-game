#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
using namespace sf;

// Handles player shooting
class Bullet
{
public:
	Texture fireTexture; // Texture for the fire
	Sprite fireSprites[1000]; // Array of fire sprites
	string fireDirections[1000]; // Array of fire directions
	int fireCount = 0; // Count of fires
	float fireTimer = 0.0; // Timer for fire
	float fireInterval = 0.6; // Interval between fires
	float fireSpeed = 2; // Speed of fire
	Clock clock; // Clock for timing
	string direction; // Direction of fire
	int i = 1; // Counter

	void fire(float time, Sprite sprite, string direction) {
		string filename; // Filename for the fire texture
		fireTimer += time; // Update fire timer
		if (fireTimer >= fireInterval) { // Check if it's time to fire
			fireTimer = 0; // Reset fire timer
			if (i < 4) // Check if the counter is within bounds
				filename = "img/PNG/Effects/fire0" + to_string(i) + ".png"; // Set filename

			fireTexture.loadFromFile(filename); // Load fire texture
			fireSprites[fireCount].setTexture(fireTexture); // Set texture for fire sprite

			// Initial position
			float posX = sprite.getPosition().x; // Get X position of sprite
			float posY = sprite.getPosition().y; // Get Y position of sprite

			posX += sprite.getGlobalBounds().width / 2 - 6; // Calculate X position for fire
			posY += (sprite.getGlobalBounds().height / 2) - 50; // Calculate Y position for fire

			fireSprites[fireCount].setPosition(posX, posY); // Set position of fire sprite

			// Set fire direction
			fireDirections[fireCount] = direction; // Set direction of fire

			i++; // Increment counter
			if (i >= 4) // Check if counter exceeds bounds
				i = 1; // Reset counter
			fireCount++; // Increment fire count
		}
	}

	float fireTimes[1000]; // Array of fire times

	void firePowerUp(float time, Sprite sprite, float fireInterval) {
		string filename; // Filename for the fire texture
		string directions[7] = { "u", "l", "r", "ul", "ur", "dl", "dr" }; // Array of fire directions

		fireTimer += time; // Update fire timer
		if (fireTimer >= fireInterval) { // Check if it's time to fire
			fireTimer = 0; // Reset fire timer
			if (i < 4) // Check if the counter is within bounds
				filename = "img/PNG/Effects/fire0" + to_string(i) + ".png"; // Set filename

			fireTexture.loadFromFile(filename); // Load fire texture

			for (int j = 0; j < 7; j++) { // Loop through directions
				fireSprites[fireCount].setTexture(fireTexture); // Set texture for fire sprite

				float posX = sprite.getPosition().x; // Get X position of sprite
				float posY = sprite.getPosition().y; // Get Y position of sprite

				posX += sprite.getGlobalBounds().width / 2 - 6; // Calculate X position for fire
				posY += (sprite.getGlobalBounds().height / 2) - 50; // Calculate Y position for fire

				fireSprites[fireCount].setPosition(posX, posY); // Set position of fire sprite
				fireDirections[fireCount] = directions[j]; // Set direction of fire
				fireCount++; // Increment fire count
			}

			i++; // Increment counter
			if (i >= 4) // Check if counter exceeds bounds
				i = 1; // Reset counter
		}
	}

	void firemovePowerUp(float fireSpeed) {
		for (int i = 0; i < fireCount; i++) { // Loop through fire sprites
			float posX = fireSprites[i].getPosition().x; // Get X position of fire sprite
			float posY = fireSprites[i].getPosition().y; // Get Y position of fire sprite

			string dir = fireDirections[i]; // Get direction of fire

			if (dir == "u") posY -= fireSpeed; // Move up
			else if (dir == "d") posY += fireSpeed; // Move down
			else if (dir == "l") posX -= fireSpeed; // Move left
			else if (dir == "r") posX += fireSpeed; // Move right
			else if (dir == "ul") { posX -= fireSpeed; posY -= fireSpeed; } // Move up-left
			else if (dir == "ur") { posX += fireSpeed; posY -= fireSpeed; } // Move up-right
			else if (dir == "dl") { posX -= fireSpeed; posY += fireSpeed; } // Move down-left
			else if (dir == "dr") { posX += fireSpeed; posY += fireSpeed; } // Move down-right

			fireSprites[i].setPosition(posX, posY); // Set new position of fire sprite
		}
	}

	void fireAddEffect(float time, Sprite sprite, string direction, float fireInterval = 0.0) {
		string filename; // Filename for the fire texture
		fireTimer += time; // Update fire timer
		if (fireTimer >= fireInterval) { // Check if it's time to fire
			fireTimer = 0; // Reset fire timer
			if (i < 4) // Check if the counter is within bounds
				filename = "img/PNG/Effects/fire0" + to_string(i) + ".png"; // Set filename

			fireTexture.loadFromFile(filename); // Load fire texture
			fireSprites[fireCount].setTexture(fireTexture); // Set texture for fire sprite

			// Initial position
			float posX = sprite.getPosition().x; // Get X position of sprite
			float posY = sprite.getPosition().y; // Get Y position of sprite

			posX += sprite.getGlobalBounds().width / 2 - 6; // Calculate X position for fire
			posY += (sprite.getGlobalBounds().height / 2) - 50; // Calculate Y position for fire

			fireSprites[fireCount].setPosition(posX, posY); // Set position of fire sprite

			// Set fire direction
			fireDirections[fireCount] = direction; // Set direction of fire

			i++; // Increment counter
			if (i >= 4) // Check if counter exceeds bounds
				i = 1; // Reset counter
			fireCount++; // Increment fire count
		}
	}

	void firemove() {
		int j = 0; // Counter
		for (int i = 0; i < fireCount; i++) { // Loop through fire sprites
			float posX = fireSprites[i].getPosition().x; // Get X position of fire sprite
			float posY = fireSprites[i].getPosition().y; // Get Y position of fire sprite
			string dir = fireDirections[i]; // Get direction of fire

			if (dir == "u") posY -= fireSpeed; // Move up
			else if (dir == "d") posY -= fireSpeed; // Move up
			else if (dir == "l") posY -= fireSpeed; // Move up
			else if (dir == "r") posY -= fireSpeed; // Move up
			else if (dir == "ul") { posX -= fireSpeed; posY -= fireSpeed; } // Move up-left
			else if (dir == "ur") { posX += fireSpeed; posY -= fireSpeed; } // Move up-right
			else if (dir == "dl") { posX -= fireSpeed; posY += fireSpeed; } // Move down-left
			else if (dir == "dr") { posX += fireSpeed; posY += fireSpeed; } // Move down-right

			fireSprites[i].setPosition(posX, posY); // Set new position of fire sprite

			if (posX > 0 && posX < 780 && posY > 0 && posY < 780) { // Check if fire sprite is within bounds
				fireSprites[j] = fireSprites[i]; // Move fire sprite
				fireDirections[j] = fireDirections[i]; // Set direction of fire
				j++; // Increment counter
			}
		}
		fireCount = j; // Update fire count
	}
};