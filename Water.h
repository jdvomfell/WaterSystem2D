// File: Water.h
// Author: Jesse Vomfell
// Date: 17th March 2017

#ifndef WATER_H
#define WATER_H

#include <SFML\Graphics.hpp>
#include <vector>
#include <map>

// Desc: Represents a single spring
//		x: The springs x position
//		y: The current y position of the spring
//		targetY: The y position the spring should rest at
//		velocity: The velocity the spring is moving in, positive = up direction, negative = down direction
class Spring {

	friend class Water;

	float x;
	float y;
	float targetY;
	float velocity;

	Spring(float x, float targetY);

	void update();

};

// Desc: Represents a group of springs that mimic a water like effect in a rectangular area
//		topLeft: Vector containing the top left coordinates for the water body
//		bottomRight: Vector containing the bottom right coordinates for the water body
//		targetY: The y position that all springs should rest at
//		depth: The y position that represents the bottom of the body of water
//		waterShape: The vertex array used to visually represent the body of water
//		springs: The list of springs that make up a body of water
class Water {

	friend class WaterHandler;

	sf::Vector2f topLeft;
	sf::Vector2f bottomRight;
	float targetY;
	float depth;

	sf::VertexArray waterShape;
	std::vector<Spring *> springs;

	Water(sf::Vector2f corner1, sf::Vector2f corner2);

	void update();
	void wave(float waveOffset);
	bool contains(sf::Vector2f position);

public:

	void splash(float x, float velocity);
	void adjustWaterLevel(float amount);
	void draw(sf::RenderWindow * window);

};

// Desc: Responsible for maintaining all bodies of water in a given enviornment
//		waveOffset: The time based offset used to update waves
//		entities: A map containing bodies of water in the enviorment
class WaterHandler {

	float waveOffset;
	std::map<float, Water *> entities;

public:

	Water * inWater(sf::Vector2f position);
	Water * insert(sf::Vector2f topLeft, sf::Vector2f);
	void update(float dt);
	void draw(sf::RenderWindow * window);

};

#endif