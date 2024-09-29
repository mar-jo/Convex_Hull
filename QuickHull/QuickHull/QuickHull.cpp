#include <iostream>
#include <future>
#include <execution>

#include "QuickHull.h"

QuickHull::QuickHull(const std::vector<Point>& field) : field(field) {};

Point QuickHull::minXCoordinate(const std::vector<Point>& points)
{
    return *std::min_element(points.begin(), points.end(), [&points](const Point& a, const Point& b)
        {
            return a.x < b.x;
        });
}

Point QuickHull::maxXCoordinate(const std::vector<Point>& points)
{
    return *std::max_element(points.begin(), points.end(), [&points](const Point& a, const Point& b)
        {
            return a.x < b.x;
        });
}

float QuickHull::crossProduct(const Point& A, const Point& B, const Point& P)
{
    return (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
}

std::vector<Point> QuickHull::pointsAboveLine(const Point& A, const Point& B, const std::vector<Point>& points)
{
    std::vector<Point> result;

    for (const auto& point : points)
    {
        if (crossProduct(A, B, point) > 0)
            result.push_back(point);
    }

    return result;
}

std::vector<Point> QuickHull::pointsBelowLine(const Point& A, const Point& B, const std::vector<Point>& points)
{
    std::vector<Point> result;

    for (const auto& point : points)
    {
        if (crossProduct(A, B, point) < 0)
            result.push_back(point);
    }

    return result;
}

Point QuickHull::findFurthestPoint(const Point& A, const Point& B, const std::vector<Point>& points)
{
    Point furthestPoint = Point();
    float maxDistance = -1;

    for (const auto& point : points)
    {
        float distance = std::abs(crossProduct(A, B, point));

        if (distance > maxDistance)
        {
            maxDistance = distance;
            furthestPoint = point;
        }
    }

    return furthestPoint;
}

void QuickHull::quickHullHelper(const Point& A, const Point& B, const std::vector<Point>& points, std::vector<Point>& result)
{
    if (points.empty()) return;

    Point C = findFurthestPoint(A, B, points);

    if (std::find(result.begin(), result.end(), C) == result.end()) 
    {
        result.push_back(C);
    }

    auto points1 = pointsAboveLine(A, C, points);
    auto points2 = pointsAboveLine(C, B, points);

    // Async Tasks
    std::vector<Point> result1, result2;

    auto future1 = std::async(std::launch::async, &QuickHull::quickHullHelper, this, A, C, points1, std::ref(result1));
    auto future2 = std::async(std::launch::async, &QuickHull::quickHullHelper, this, C, B, points2, std::ref(result2));

    future1.wait();
    future2.wait();

    result.insert(result.end(), result1.begin(), result1.end());
    result.insert(result.end(), result2.begin(), result2.end());
}


std::vector<Point> QuickHull::quickHullAlgorithm()
{
    std::vector<Point> hull;

    Point minX = minXCoordinate(this->field);
    Point maxX = maxXCoordinate(this->field);

    hull.push_back(minX);
    hull.push_back(maxX);

    auto pointsAbove = pointsAboveLine(minX, maxX, this->field);
    auto pointsBelow = pointsBelowLine(minX, maxX, this->field);

    // Async Tasks
    auto future1 = std::async(std::launch::async, &QuickHull::quickHullHelper, this, minX, maxX, pointsAbove, std::ref(hull));
    auto future2 = std::async(std::launch::async, &QuickHull::quickHullHelper, this, maxX, minX, pointsBelow, std::ref(hull));


    future1.wait();
    future2.wait();

    return hull;
}