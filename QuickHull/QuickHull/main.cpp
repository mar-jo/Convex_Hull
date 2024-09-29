#include <iostream>
#include <chrono>
#include <string>

#include "QuickHull.h"
#include "JervisMarch.h"
#include "Field.h"

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

    //QUICKHULL
    QuickHull quickHull(field);

    // START TIME
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Point> convexHullQuickHull = quickHull.quickHullAlgorithm();

    // END TIME
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "TIME TAKEN QUICKHULL: " << duration.count() << " ms\n\n";

    std::cout << "CONVEX HULL POINTS: " << std::endl;
    for (const auto& point : convexHullQuickHull)
    {
        std::cout << "[" << point.x << ", " << point.y << "]" << std::endl;
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

    return 0;
}