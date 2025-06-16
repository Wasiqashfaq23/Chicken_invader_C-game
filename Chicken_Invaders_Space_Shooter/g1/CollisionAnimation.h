#include <SFML/Graphics.hpp>

// Handles collision effects
class CollisionAnimation {
public:
    CollisionAnimation(sf::Sprite& enemy1, sf::Sprite& enemy2) : enemy1(enemy1), enemy2(enemy2), state(MOVING) {} // Constructor

    void update(float deltaTime) {
        if (state == MOVING) { // Check if the state is moving
            // Move the enemies towards each other
            enemy1.move(velocity1 * deltaTime); // Move enemy1
            enemy2.move(velocity2 * deltaTime); // Move enemy2

            // Check for collision
            if (enemy1.getGlobalBounds().intersects(enemy2.getGlobalBounds())) { // Check if enemies collide
                // Collision detected
                state = SHRINKING; // Change state to shrinking
            }
        } else if (state == SHRINKING) { // Check if the state is shrinking
            // Scale down enemies
            enemy1.scale(1 - shrinkSpeed * deltaTime, 1 - shrinkSpeed * deltaTime); // Scale down enemy1
            enemy2.scale(1 - shrinkSpeed * deltaTime, 1 - shrinkSpeed * deltaTime); // Scale down enemy2

            // Check if enemies have disappeared
            if (enemy1.getScale().x <= 0 || enemy2.getScale().x <= 0) { // Check if enemies are gone
                // Animation complete
            }
        }
    }

private:
    sf::Sprite& enemy1; // Reference to enemy1 sprite
    sf::Sprite& enemy2; // Reference to enemy2 sprite
    sf::Vector2f velocity1 = {-100, 0}; // Velocity of enemy1
    sf::Vector2f velocity2 = {100, 0}; // Velocity of enemy2
    float shrinkSpeed = 2; // Speed of shrinking
    enum State { MOVING, SHRINKING }; // Enum for animation states
    State state; // Current state of the animation
};
