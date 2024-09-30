#define SDL_MAIN_HANDLED
#include <iostream>
#include <chrono>
#include <string>

#include "QuickHull.h"
#include "JervisMarch.h"
#include "Field.h"
#include "Visualizer.h"
#include <SDL.h>

#include <algorithm>
#include <cmath>

#include <algorithm>
#include <cmath>

// Function to compute the centroid of the convex hull
Point computeCentroid(const std::vector<Point>& hull)
{
    float centroidX = 0, centroidY = 0;
    for (const auto& point : hull)
    {
        centroidX += point.x;
        centroidY += point.y;
    }
    centroidX /= hull.size();
    centroidY /= hull.size();
    return { centroidX, centroidY };
}

// Helper function to calculate the angle from the centroid
float calculateAngleFromCentroid(const Point& centroid, const Point& p)
{
    return std::atan2(p.y - centroid.y, p.x - centroid.x);
}

// Sort hull points based on the angle from the centroid
void sortHullPoints(std::vector<Point>& hull)
{
    // Compute the centroid of the hull
    Point centroid = computeCentroid(hull);

    // Sort the points based on their angle relative to the centroid
    std::sort(hull.begin(), hull.end(), [&centroid](const Point& a, const Point& b)
        {
            return calculateAngleFromCentroid(centroid, a) < calculateAngleFromCentroid(centroid, b);
        });
}


int main() 
{
    Field generator;
    std::vector<Point> field;
    int choice;

    // Generate Field
    std::cout << "Choose an option:\n"
              << "[1] Randomly generate field\n"
              << "[2] Load field from a text file\n"
              << "Enter your choice: ";
    std::cin >> choice;

    if (choice == 1)
    {
        field = generator.FieldGenerator();
    }
    else if (choice == 2)
    {
        std::string filename;
        std::cout << "Enter filename: ";
        std::cin >> filename;

        field = generator.readFromFile(filename);
    }
    else
    {
        std::cout << "Error: Invalid Input. Terminating Program.\n";
    }

    //TODO DRAW with abfrage

    char visualize;
    std::cout << "Would you like to visualize the convex hull? (y/n): ";
    std::cin >> visualize;
    bool draw = visualize == 'y' ? true : false;

    //QUICKHULL
    QuickHull quickHull(field);

    // START TIME
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Point> convexHullQuickHull = quickHull.quickHullAlgorithm();

    // END TIME
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "TIME TAKEN QUICKHULL: " << duration.count() << " ms\n\n";

    // Sort the points in the convex hull based on their angle from the centroid
    sortHullPoints(convexHullQuickHull);

    std::cout << "CONVEX HULL POINTS: " << std::endl;
    for (const auto& point : convexHullQuickHull)
    {
        std::cout << "[" << point.x << ", " << point.y << "]" << std::endl;
    }

    if (draw)
    {
        visualizeConvexHull(field, convexHullQuickHull, "QuickHull");
	}   

    //JERVIS MARCH
    JervisMarch jervisMarch(field);

    // START TIME
    start = std::chrono::high_resolution_clock::now();

    std::vector<Point> convexHullJervisMarch = jervisMarch.jervisMarchAlgorithm();

    // END TIME
    end = std::chrono::high_resolution_clock::now();

    duration = end - start;
    std::cout << "\n\nTIME TAKEN JERVIS MARCH: " << duration.count() << " ms\n\n";

    std::cout << "CONVEX HULL POINTS: " << std::endl;
    for (const auto& point : convexHullJervisMarch)
    {
        std::cout << "[" << point.x << ", " << point.y << "]" << std::endl;
    }

    if (draw)
    {
        visualizeConvexHull(field, convexHullJervisMarch, "JervisMarch");
    }

    return 0;
}