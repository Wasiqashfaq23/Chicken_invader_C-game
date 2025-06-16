#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>

// Main menu class that handles game menu interface and navigation
class Menu {
public:
	// Core menu elements
	sf::Sprite background;
	sf::Text options[5];
	sf::RectangleShape optionBoxes[6];
	int selected;
	sf::Font font;
	sf::Texture backgroundTexture;
	sf::Clock enterKeyCooldown;
	const float ENTER_KEY_DELAY = 0.2f;

	Menu() {
		// Initialize textures and sprites
		backgroundTexture.loadFromFile("img/menu_background.jpg");
		startButtonTexture.loadFromFile("img/start.png");
		exitButtonTexture.loadFromFile("img/start.png");

		background.setTexture(backgroundTexture);
		startButton.setTexture(startButtonTexture);
		exitButton.setTexture(exitButtonTexture);

		startButton.setPosition(300, 200);
		exitButton.setPosition(300, 300);

		font.loadFromFile("assets/arial.ttf");

		// Set up menu options
		options[0].setString("Start Game");
		options[1].setString("Resume");
		options[2].setString("High Scores");
		options[3].setString("Instructions");
		options[4].setString("Exit");

		// Configure menu option styling
		for (int i = 0; i < 5; i++) {
			options[i].setFont(font);
			options[i].setCharacterSize(24);
			options[i].setFillColor(sf::Color::White);
			
			optionBoxes[i].setFillColor(sf::Color::Black);
			optionBoxes[i].setOutlineColor(sf::Color::White);
			optionBoxes[i].setOutlineThickness(2);
		}

		selected = 0;
		enterKeyCooldown.restart();
	}

	// Input handling methods
	bool canProcessEnterKey() {
		return enterKeyCooldown.getElapsedTime().asSeconds() >= ENTER_KEY_DELAY;
	}

	void resetEnterKeyCooldown() {
		enterKeyCooldown.restart();
	}

	bool isMusicOn() const {
		return true;
	}

	// Menu rendering and layout
	void draw(sf::RenderWindow& window) {
		window.draw(background);

		unsigned int windowSizeX = window.getSize().x;
		unsigned int windowSizeY = window.getSize().y;
		float spacing = 20;
		float boxWidth = 250;
		float boxHeight = 40;

		float totalHeight = (boxHeight + spacing) * 6 - spacing;
		float startY = (windowSizeY - totalHeight) / 2;

		// Draw menu options with proper positioning and highlighting
		for (int i = 0; i < 5; i++) {
			sf::FloatRect textBounds = options[i].getLocalBounds();
			
			float boxX = (windowSizeX - boxWidth) / 2;
			float boxY = startY + i * (boxHeight + spacing);

			optionBoxes[i].setPosition(boxX, boxY);
			optionBoxes[i].setSize(sf::Vector2f(boxWidth, boxHeight));

			float textX = boxX + (boxWidth - textBounds.width) / 2;
			float textY = boxY + (boxHeight - textBounds.height) / 2 - textBounds.top;

			options[i].setPosition(textX, textY);

			if (i == selected) {
				optionBoxes[i].setFillColor(sf::Color(50, 50, 50));
				options[i].setStyle(sf::Text::Bold);
			} else {
				optionBoxes[i].setFillColor(sf::Color::Black);
				options[i].setStyle(sf::Text::Regular);
			}

			window.draw(optionBoxes[i]);
			window.draw(options[i]);
		}
	}

	// Menu navigation methods
	void moveUp() {
		if (selected > 0) {
			selected--;
		}
	}

	void moveDown() {
		if (selected < 5) {
			selected++;
		}
	}

	int select() {
		return selected;
	}

	void render(sf::RenderWindow& window) {
		window.clear();
		window.draw(background);
		window.draw(startButton);
		window.draw(exitButton);
		window.display();
	}

private:
	sf::Texture startButtonTexture, exitButtonTexture;
	sf::Sprite startButton, exitButton;
};
