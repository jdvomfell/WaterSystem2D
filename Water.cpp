// File: Water.cpp
// Author: Jesse Vomfell
// Date: 17th March 2017

#include "Water.h"

#include <cmath>

#define PI 3.14159265f

#define PIXELS_PER_SPRING 20.0f
#define SPREAD 0.25f

void Spring::update() {

	/* Spring Constant - Determines Spring Stifness */
	float k = 0.015f;
	/* Dampening Factor - Determines How Fast Waves Die */
	float d = 0.05f;

	float targetYDistance = y - targetY;
	velocity += (-k * targetYDistance) - velocity * d;
	y += velocity;

}

Spring::Spring(float x, float targetY) {

	this->x = x;

	this->velocity = 0;

	this->y = targetY;
	this->targetY = targetY;

}

void Water::update() {

	for (size_t i = 0; i < springs.size(); i++) {
		springs[i]->update();
		if (springs[i]->y > depth)
			springs[i]->y = depth;
	}

	/* Have Springs Effect Each Other */

	std::vector<float> leftDeltas(springs.size());
	std::vector<float> rightDeltas(springs.size());

	for (size_t i = 0; i < springs.size(); i++) {

		if (i > 0) {
			leftDeltas[i] = SPREAD * (springs[i]->y - springs[i - 1]->y);
			springs[i - 1]->velocity += leftDeltas[i];
		}
		if (i < springs.size() - 1) {
			rightDeltas[i] = SPREAD * (springs[i]->y - springs[i + 1]->y);
			springs[i + 1]->velocity += rightDeltas[i];
		}

	}

	for (size_t i = 0; i < springs.size(); i++) {

		if (i > 0)
			springs[i - 1]->y += leftDeltas[i];
		if (i < springs.size() - 1)
			springs[i + 1]->y += rightDeltas[i];

	}

	/* Adjust the end springs so that they do not move */
	springs[0]->y = targetY;
	springs[springs.size() - 1]->y = targetY;

	for (size_t i = 0; i < springs.size(); i++)
		waterShape[i * 2].position = sf::Vector2f(springs[i]->x, springs[i]->y);

}

void Water::draw(sf::RenderWindow * window) {
	
	window->draw(waterShape);

}

void Water::setColor(WaterLayer waterLayer, sf::Color color) {
	
	for (int i = 0; i < springs.size(); i++) {
		if (waterLayer == WATER_SURFACE)
			waterShape[i * 2].color = color;
		else if(waterLayer == WATER_FLOOR)
			waterShape[i * 2 + 1].color = color;
	}

}

void Water::splash(float x, float velocity) {

	int index = (int)(x - topLeft.x) / PIXELS_PER_SPRING;

	if (index >= 0 && index < springs.size()) {
		springs[index]->velocity = velocity;
	}

}

void Water::adjustWaterLevel(float amount) {

	topLeft.y -= amount;
	depth -= amount;

	targetY -= amount;
	for (size_t i = 0; i < springs.size(); i++)
		springs[i]->targetY -= amount;

}

/* Need to fix upside down water */
Water::Water(sf::Vector2f topLeft, sf::Vector2f bottomRight) {

	Spring * spring;
	sf::ConvexShape * shape;

	this->topLeft = topLeft;
	this->bottomRight = bottomRight;

	float length = (bottomRight.x - topLeft.x);
	depth = bottomRight.y;
	targetY = topLeft.y;
	
	int numSprings = length / PIXELS_PER_SPRING;

	for (int i = 0; i < numSprings; i++) {
		spring = new Spring(topLeft.x + length * ((float)i / (float)numSprings), topLeft.y);
		springs.push_back(spring);
	}

	/* Insert Last Spring */

	spring = new Spring(topLeft.x + length, topLeft.y);
	springs.push_back(spring);

	/* Create Vertex Array Of Traiangles For Rendering */

	waterShape = sf::VertexArray(sf::TriangleStrip, springs.size() * 2);

	for (int i = 0; i < springs.size(); i++) {
		/* Make Water Surface */
		waterShape[i * 2].position = sf::Vector2f(springs[i]->x, springs[i]->y);
		waterShape[i * 2].color = sf::Color(0, 0, 0, 175);
		/* Make Bottom Of Water */
		waterShape[i * 2 + 1].position = sf::Vector2f(springs[i]->x, depth);
		waterShape[i * 2 + 1].color = sf::Color::Black;
	}

}

bool Water::contains(sf::Vector2f position) {

	if(position.x > topLeft.x && position.x < bottomRight.x && position.y > topLeft.y && position.y < bottomRight.y)
		return true;

	return false;

}

void Water::wave(float waveOffset) {

	for (size_t i = 1; i < springs.size() - 1; i++) {

		springs[i]->targetY = targetY + sin(springs[i]->x + waveOffset * 10) * 100;

	}

}

Water * WaterHandler::inWater(sf::Vector2f position) {

	std::map<float, Water *>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++) {
		if (it->second->contains(position))
			return it->second;
	}

}

Water * WaterHandler::insert(sf::Vector2f topLeft, sf::Vector2f bottomRight) {

	Water * water = new Water(topLeft, bottomRight);

	entities.insert(std::make_pair(topLeft.x, water));
	return water;

}

void WaterHandler::update(float dt) {

	waveOffset += dt;
	if (waveOffset > 2 * PI)
		waveOffset -= 2 * PI;

	std::map<float, Water *>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++) {
		it->second->update();
		it->second->wave(waveOffset);
	}

}

void WaterHandler::draw(sf::RenderWindow * window) {

	std::map<float, Water *>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++) {
		it->second->draw(window);
	}

}