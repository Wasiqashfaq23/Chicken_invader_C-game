// Class definition for the Badge
class Badge
{
public:
    sf::Texture texture; // Texture for the badge
    sf::Sprite sprite; // Sprite for the badge

    Badge(const std::string& textureFile)
    {
        texture.loadFromFile(textureFile); // Load texture from file
        sprite.setTexture(texture); // Set texture for sprite
    }
};
