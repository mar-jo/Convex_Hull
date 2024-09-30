// Visualizer.h
#pragma once

#include <vector>
#include "Field.h"  // Assuming Point is declared in Field.h
#include <SDL.h>

// SDL Initialization
bool initSDL(SDL_Window** window, SDL_Renderer** renderer, int screenWidth, int screenHeight);

// Function to draw points
void drawPoints(SDL_Renderer* renderer, const std::vector<Point>& points, int scaleX, int scaleY, int offsetX, int offsetY);

// Function to draw edges between two points
void drawEdge(SDL_Renderer* renderer, const Point& p1, const Point& p2, int scaleX, int scaleY, int offsetX, int offsetY);

// Generic visualization function to visualize any convex hull
void visualizeConvexHull(const std::vector<Point>& field, const std::vector<Point>& hull, const std::string& algorithmName);
