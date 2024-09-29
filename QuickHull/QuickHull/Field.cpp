#include "Field.h"

#include <iostream>
#include <random>
#include <unordered_set>
#include <fstream>
#include <sstream>

std::vector<Point> Field::FieldGenerator()
{
    std::vector<Point> field;

    field.reserve(NUMBER_OF_POINTS);

    std::unordered_set<Point, PointHash> uniquePoints;
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, FIELD_SIZE);

    while (field.size() < NUMBER_OF_POINTS)
    {
        Point point;
        point.x = dist(rng); // generate X coord
        point.y = dist(rng); // generate Y coord

        // check if the point is unique
        if (uniquePoints.insert(point).second)
            field.push_back(point);
    }

    return field;
}

std::vector<Point> Field::readFromFile(const std::string& filename)
{
    std::vector<Point> field;

    std::ifstream file(filename);

    if (!file.is_open()) 
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return field;
    }

    std::string line;
    while (std::getline(file, line)) 
    {
        std::stringstream ss(line);
        std::string xStr, yStr;

        if (std::getline(ss, xStr, ',') && std::getline(ss, yStr)) 
        {
            try 
            {
                float x = std::stof(xStr);
                float y = std::stof(yStr);

                field.push_back(Point{ x, y });
            }
            catch (const std::invalid_argument& e) 
            {
                std::cerr << "Error: Invalid float value in line: " << line << std::endl;
            }
            catch (const std::out_of_range& e) 
            {
                std::cerr << "Error: Float value out of range in line: " << line << std::endl;
            }
        }
    }

    file.close();

    return field;
}