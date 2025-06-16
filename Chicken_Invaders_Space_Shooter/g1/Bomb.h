#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
using namespace std;

// Base class for all bomb types in the game
class Bomb {
public:
    Sprite sprite; // Sprite for the bomb
    Texture texture; // Texture for the bomb
    Vector2f position; // Position of the bomb
    float speed; // Speed of the bomb

    Bomb(const std::string& texturePath, float x, float y, float speed)
        : position(x, y), speed(speed) {
        texture.loadFromFile(texturePath); // Load texture from file
        sprite.setTexture(texture); // Set texture for sprite
        sprite.setPosition(position); // Set position of sprite
    }

    virtual void move(float deltaTime) {
        position.y += speed * deltaTime; // Update Y position
        sprite.setPosition(position); // Set new position of sprite
    }
};