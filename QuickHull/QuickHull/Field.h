#pragma once

#include <vector>
#include <string>

#define FIELD_SIZE 1000000
#define NUMBER_OF_POINTS 1000000

struct Point
{
    float x, y;

    // needed for unordered_set
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct PointHash
{
    // hash function for Point in unordered_set
    std::size_t operator()(const Point& p) const
    {
        return std::hash<float>()(p.x) ^ (std::hash<float>()(p.y) << 1);
    }
};

class Field
{
public:
    Field() = default;
    ~Field() = default;

    std::vector<Point> FieldGenerator();
    std::vector<Point> readFromFile(const std::string& filename);
};

