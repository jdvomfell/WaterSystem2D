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

	/* Water Stuff */

	WaterHandler waterHandler;

	Water * water = waterHandler.insert(sf::Vector2f(10, window.getSize().y / 2), sf::Vector2f(window.getSize().x - 10, window.getSize().y - 10));
	Water * splashWater;

	bool needToRelease = false;

	float waterSpikeTime = 0;
	bool waterSpike = false;

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

		//UPDATE
		delta = clock.getElapsedTime();
		clock.restart();
		dt = delta.asSeconds();

		//Rising Water Levels
		water->adjustWaterLevel(0.5f);

		/*waterSpikeTime += dt;
		if (waterSpikeTime > 5) {
			waterSpike = !waterSpike;
			waterSpikeTime = 0;
		}

		if (waterSpike) {
			water->splash(25, -30);
		}*/


		waterHandler.update(dt);

		//RENDER
		window.clear(sf::Color::White);

		waterHandler.draw(&window);

		window.display();
	}

	return 0;
}