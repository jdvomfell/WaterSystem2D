#include <SFML\Graphics.hpp>

#include "Water.h"

int main()
{

	srand((int)time(NULL));

	sf::RenderWindow window(sf::VideoMode(1500, 1200), "SFML works!");

	window.setFramerateLimit(60);

	sf::Clock clock;
	sf::Time delta;
	float dt;

	sf::Event event;
	sf::Vector2f clickPos;
	sf::Color surface;
	sf::Color floor;
	int colorCount = 0;

	/* Water Stuff */

	WaterHandler waterHandler;

	Water * water = waterHandler.insert(sf::Vector2f(10, window.getSize().y / 2), sf::Vector2f(window.getSize().x - 10, window.getSize().y - 10));
	Water * splashWater;

	bool needToRelease = false;

	window.setKeyRepeatEnabled(false);

	while (window.isOpen())
	{

		//HANDLE EVENTS

		window.pollEvent(event);

		switch (event.type){
			
		case sf::Event::Closed:
				window.close();
				exit(0);
				break;

		case sf::Event::MouseButtonPressed:

			clickPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			splashWater = nullptr;
			splashWater = waterHandler.inWater(clickPos);
			if (splashWater)
				splashWater->splash(clickPos.x, 100);
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Space) {
				water->adjustWaterLevel(10);
			}

		case sf::Event::KeyReleased:
			if(event.key.code == sf::Keyboard::Space) {
				needToRelease = false;
			}
		}

		if ((colorCount++) > 5) {
			surface.r = (surface.r + 1) % 255;
			surface.b = (surface.b + 2) % 255;
			surface.g = (surface.g + 3) % 255;

			floor.r = floor.r - 7;
			floor.b = floor.b - 6;
			floor.g = floor.g - 5;

			if (floor.r < 255) floor.r += 255;
			if (floor.g < 255) floor.r += 255;
			if (floor.b < 255) floor.r += 255;

			water->setColor(WATER_SURFACE, surface);
			water->setColor(WATER_FLOOR, floor);
			
			colorCount = 0;
		}

		delta = clock.getElapsedTime();
		clock.restart();
		dt = delta.asSeconds();

		//Rising Water Levels
		//water->adjustWaterLevel(0.5f);

		waterHandler.update(dt);
		window.clear(sf::Color::Black);

		waterHandler.draw(&window);

		window.display();
	}

	return 0;
}