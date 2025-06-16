#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
using namespace std;

// Base class for game power-ups
class Addons {
public:
    Sprite sprite; // Sprite for the addon
    Texture texture; // Texture for the addon
    float positionX; // X position of the addon
    float positionY; // Y position of the addon
    float speed; // Speed of the addon
    float timeSinceLastDrop; // Time since the last drop
    Clock clock; // Clock for timing
    string type; // Type of the addon

    // Constructor initializes addon with position and speed
    Addons(const string& texturePath, float x, float y, float speed)
        : positionX(x), positionY(y), speed(speed), timeSinceLastDrop(0.0f) {
        texture.loadFromFile(texturePath); // Load texture from file
        sprite.setTexture(texture); // Set texture for sprite
        sprite.setPosition(positionX, positionY); // Set position of sprite
    }

    // Base movement function for all addons
    virtual void move(float deltaTime) {
        positionY += speed * deltaTime; // Update Y position
        sprite.setPosition(positionX, positionY); // Set new position of sprite
    }
};

// Power-up that increases player's power
class PowerUp : public Addons {
public:
    PowerUp(float x, float y)
        : Addons("img/PNG/Power-ups/bolt_gold.png", x, y, 1200.0f) {
        type = "PowerUp"; // Set type of addon
    }
};

// Power-up that enhances player's fire rate
class Fire : public Addons {
public:
    Fire(float x, float y)
        : Addons("img/PNG/Power-ups/star_gold.png", x, y, 1000.0f) {
        type = "Fire"; // Set type of addon
    }
};

// Power-up that increases game difficulty
class Danger : public Addons {
public:
    Danger(float x, float y)
        : Addons("img/PNG/Power-ups/powerupRed_bolt.png", x, y, 1000.0f) {
        type = "Danger"; // Set type of addon
    }
};

// Power-up that adds extra lives
class LivesAddon : public Addons {
public:
    LivesAddon(float x, float y)
        : Addons("img/heart2.png", x, y, 1000.0f) {
        type = "LivesAddon"; // Set type of addon
    }
};
