#include "Visualizer.h"

#include <iostream>
#include <algorithm>

const float MARGIN_PERCENTAGE = 0.1f; // 10% padding around convex hull

// init SDL
bool Visualizer::initSDL(SDL_Window** window, SDL_Renderer** renderer, int screenWidth, int screenHeight)
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

// calculate scaling factors and adjust for padding
void Visualizer::calculateScalingFactors(const std::vector<Point>& points, int screenWidth, int screenHeight, float& scaleX, float& scaleY, float& offsetX, float& offsetY)
{
    if (points.empty()) return;

    float minX = points[0].x, maxX = points[0].x;
    float minY = points[0].y, maxY = points[0].y;

    // find min and max x and y coordinates
    for (const Point& p : points)
    {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }

    // calculate width and height of point field
    float fieldWidth = maxX - minX;
    float fieldHeight = maxY - minY;

    // non-zero dimensions to avoid division by zero
    if (fieldWidth == 0) fieldWidth = 1;
    if (fieldHeight == 0) fieldHeight = 1;

    // add padding to field size
    fieldWidth *= (1 + MARGIN_PERCENTAGE);
    fieldHeight *= (1 + MARGIN_PERCENTAGE);

    // calculate scaling factors to fit the window
    scaleX = screenWidth / fieldWidth;
    scaleY = screenHeight / fieldHeight;

    // smaller scale to maintain aspect ratio
    float scale = std::min(scaleX, scaleY);

    // calculate offsets to center points within screen
    offsetX = -minX * scale + (screenWidth - fieldWidth * scale) / 2;
    offsetY = -minY * scale + (screenHeight - fieldHeight * scale) / 2;
}


// draw points
void Visualizer::drawPoints(SDL_Renderer* renderer, const std::vector<Point>& points, float scaleX, float scaleY, float offsetX, float offsetY)
{
    for (const Point& p : points)
    {
        int screenX = static_cast<int>(p.x * scaleX + offsetX);
        int screenY = static_cast<int>(p.y * scaleY + offsetY);
        SDL_RenderDrawPoint(renderer, screenX, screenY);
    }
}

void Visualizer::drawPoint(SDL_Renderer* renderer, const Point& point, int scaleX, int scaleY, int offsetX, int offsetY)
{
    int screenX = point.x / scaleX + offsetX;
    int screenY = point.y / scaleY + offsetY;
    SDL_RenderDrawPoint(renderer, screenX, screenY);
}

// draw edge between two points
void Visualizer::drawEdge(SDL_Renderer* renderer, const Point& p1, const Point& p2, float scaleX, float scaleY, float offsetX, float offsetY)
{
    int x1 = static_cast<int>(p1.x * scaleX + offsetX);
    int y1 = static_cast<int>(p1.y * scaleY + offsetY);
    int x2 = static_cast<int>(p2.x * scaleX + offsetX);
    int y2 = static_cast<int>(p2.y * scaleY + offsetY);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Visualizer::visualizeSteps(const std::vector<Point>& field, const std::vector<Action>& steps)
{
    // init SDL
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int screenWidth = 800, screenHeight = 600;

    if (!initSDL(&window, &renderer, screenWidth, screenHeight))
    {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return;
    }

    // calculate scaling and offsets
    float scaleX = 1, scaleY = 1, offsetX = 0, offsetY = 0;
    calculateScalingFactors(field, screenWidth, screenHeight, scaleX, scaleY, offsetX, offsetY);

    // draw field points
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    drawPoints(renderer, field, scaleX, scaleY, offsetX, offsetY);
    SDL_RenderPresent(renderer);

    // wait one second after opening screen
    SDL_Delay(1000);

    // visualize steps
    for (const auto& step : steps)
    {
        if (step.type == Action::POINT)
        {
            // visualize point
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
            drawPoint(renderer, step.p1, scaleX, scaleY, offsetX, offsetY);
        }
        else if (step.type == Action::EDGE)
        {
            // visualize edge
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
            drawEdge(renderer, step.p1, step.p2, scaleX, scaleY, offsetX, offsetY);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(500); // step delay
    }

    // keep window open, until "X" is pressed
    bool quit = false;
    SDL_Event e;
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

    // cleanup SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
