#pragma once

#include "Field.h"

#include <vector>
#include <SDL.h>

class Visualizer
{
public:
	Visualizer() = default;
	~Visualizer() = default;

	bool initSDL(SDL_Window** window, SDL_Renderer** renderer, int screenWidth, int screenHeight);

	void drawPoints(SDL_Renderer* renderer, const std::vector<Point>& points, float scaleX, float scaleY, float offsetX, float offsetY);
	void drawPoint(SDL_Renderer* renderer, const Point& point, int scaleX, int scaleY, int offsetX, int offsetY);
	void drawEdge(SDL_Renderer* renderer, const Point& p1, const Point& p2, float scaleX, float scaleY, float offsetX, float offsetY);

	void calculateScalingFactors(const std::vector<Point>& points, int screenWidth, int screenHeight, float& scaleX, float& scaleY, float& offsetX, float& offsetY);

	void visualizeSteps(const std::vector<Point>& field, const std::vector<Action>& steps);
private:
};
