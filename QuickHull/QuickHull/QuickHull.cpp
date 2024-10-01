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

void QuickHull::quickHullHelper(const Point& A, const Point& B, const std::vector<Point>& points, std::unordered_set<Point, PointHash>& resultSet, std::vector<Action>& visualizationSteps, int depth)
{
    if (points.empty()) return;

    Point C = findFurthestPoint(A, B, points);

    // avoid duplicates
    if (resultSet.find(C) == resultSet.end()) 
    {
        resultSet.insert(C);

        // log point action
        visualizationSteps.push_back({ Action::POINT, C });
    }

    std::vector<Point> pointsAboveAC, pointsAboveCB;
    for (const auto& point : points) 
    {
        if (crossProduct(A, C, point) > 0)
            pointsAboveAC.push_back(point);

        else if (crossProduct(C, B, point) > 0)
            pointsAboveCB.push_back(point);
    }

    // log edge action between A and C
    visualizationSteps.push_back({ Action::EDGE, A, C });

    quickHullHelper(A, C, pointsAboveAC, resultSet, visualizationSteps, depth + 1);

    // log edge action between C and B
    visualizationSteps.push_back({ Action::EDGE, C, B });

    quickHullHelper(C, B, pointsAboveCB, resultSet, visualizationSteps, depth + 1);
}

std::vector<Point> QuickHull::quickHullAlgorithm(std::vector<Action>& visualizationSteps)
{
    std::unordered_set<Point, PointHash> resultSet;

    Point minX = minXCoordinate(this->field);
    Point maxX = maxXCoordinate(this->field);

    resultSet.insert(minX);
    resultSet.insert(maxX);

    auto pointsAbove = pointsAboveLine(minX, maxX, this->field);
    auto pointsBelow = pointsBelowLine(minX, maxX, this->field);

    // start logging and recursion
    auto future1 = std::async(std::launch::async, &QuickHull::quickHullHelper, this, minX, maxX, pointsAbove, std::ref(resultSet), std::ref(visualizationSteps), 0);
    auto future2 = std::async(std::launch::async, &QuickHull::quickHullHelper, this, maxX, minX, pointsBelow, std::ref(resultSet), std::ref(visualizationSteps), 0);

    future1.wait();
    future2.wait();

    return std::vector<Point>(resultSet.begin(), resultSet.end());
}