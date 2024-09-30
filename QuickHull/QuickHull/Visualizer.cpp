// Visualizer.cpp
#include "Visualizer.h"
#include <iostream>

const float MARGIN_PERCENTAGE = 0.1f; // 10% margin for padding around the convex hull

// Initialize SDL
bool initSDL(SDL_Window** window, SDL_Renderer** renderer, int screenWidth, int screenHeight)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *window = SDL_CreateWindow("Convex Hull Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (!*window)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

// Calculate the scaling factors and adjust for padding
// Calculate the scaling factors and adjust for padding
void calculateScalingFactors(const std::vector<Point>& points, int screenWidth, int screenHeight, int& scaleX, int& scaleY, int& offsetX, int& offsetY)
{
    int64_t minX = FIELD_SIZE, maxX = 0;
    int64_t minY = FIELD_SIZE, maxY = 0;

    // Find min and max x and y coordinates
    for (const Point& p : points)
    {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }

    // Calculate the dimensions of the point field
    int64_t fieldWidth = maxX - minX;
    int64_t fieldHeight = maxY - minY;

    // Add padding (10%) to both dimensions
    fieldWidth = (fieldWidth == 0) ? 1 : fieldWidth * (1 + MARGIN_PERCENTAGE); // Prevent zero width
    fieldHeight = (fieldHeight == 0) ? 1 : fieldHeight * (1 + MARGIN_PERCENTAGE); // Prevent zero height

    // Calculate the scaling factors
    scaleX = fieldWidth / screenWidth;
    scaleY = fieldHeight / screenHeight;

    // Prevent zero scaling
    if (scaleX == 0) scaleX = 1;
    if (scaleY == 0) scaleY = 1;

    // Center the points by calculating offset (padding in both x and y)
    offsetX = (screenWidth - (maxX - minX) / scaleX) / 2;
    offsetY = (screenHeight - (maxY - minY) / scaleY) / 2;
}


// Draw points on the screen
void drawPoints(SDL_Renderer* renderer, const std::vector<Point>& points, int scaleX, int scaleY, int offsetX, int offsetY)
{
    for (const Point& p : points)
    {
        SDL_RenderDrawPoint(renderer, p.x / scaleX + offsetX, p.y / scaleY + offsetY);
    }
}

// Draw an edge between two points
void drawEdge(SDL_Renderer* renderer, const Point& p1, const Point& p2, int scaleX, int scaleY, int offsetX, int offsetY)
{
    SDL_RenderDrawLine(renderer, p1.x / scaleX + offsetX, p1.y / scaleY + offsetY, p2.x / scaleX + offsetX, p2.y / scaleY + offsetY);
}

// Generic visualization function to visualize any convex hull
void visualizeConvexHull(const std::vector<Point>& field, const std::vector<Point>& hull, const std::string& algorithmName)
{
    int screenWidth = 800;
    int screenHeight = 600;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Initialize SDL
    if (!initSDL(&window, &renderer, screenWidth, screenHeight))
    {
        return;
    }

    // Scaling factors and offsets for padding
    int scaleX = 1, scaleY = 1, offsetX = 0, offsetY = 0;
    calculateScalingFactors(field, screenWidth, screenHeight, scaleX, scaleY, offsetX, offsetY);

    bool quit = false;
    SDL_Event e;

    // Clear the window to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Draw all points in black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    drawPoints(renderer, field, scaleX, scaleY, offsetX, offsetY);

    // Title display for which algorithm is being visualized
    std::cout << "Visualizing " << algorithmName << " convex hull...\n";

    // Visualize step by step
    for (size_t i = 0; i < hull.size(); ++i)
    {
        // Draw the edge between the current point and the next in the hull
        if (i < hull.size() - 1)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for the convex hull edges
            drawEdge(renderer, hull[i], hull[i + 1], scaleX, scaleY, offsetX, offsetY);
        }
        else
        {
            // Connect the last point to the first to close the hull
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            drawEdge(renderer, hull[i], hull[0], scaleX, scaleY, offsetX, offsetY);
        }

        // Update the window to show the drawing
        SDL_RenderPresent(renderer);

        // Wait for a short period (e.g., 500 ms) to visualize step by step
        SDL_Delay(500); // Pause for visualization

        // Check for quit event
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
        }
        if (quit) break;
    }

    // Keep the window open until the user closes it
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
