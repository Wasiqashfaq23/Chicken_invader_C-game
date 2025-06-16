// Beam class for laser beam projectiles
class Beam : public Bomb {
public:
    Beam(const std::string& texturePath, float x, float y, float speed)
        : Bomb(texturePath, x, y, speed) {} // Constructor

    void move(float deltaTime) override {
        for (int i = 0; i < 50; i++) {
            position.y += speed * deltaTime / 10.0f; // Update Y position
            sprite.setPosition(position); // Set new position of sprite
        }
    }
};

