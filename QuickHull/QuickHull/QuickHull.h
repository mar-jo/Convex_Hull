#pragma once

#include <vector>
#include <unordered_set>
#include "Field.h"

class QuickHull : public Point, PointHash
{
public:
    QuickHull(const std::vector<Point>& field);
    ~QuickHull() = default;

    Point minXCoordinate(const std::vector<Point>& points);
    Point maxXCoordinate(const std::vector<Point>& points);

    float crossProduct(const Point& A, const Point& B, const Point& P);

    std::vector<Point> pointsAboveLine(const Point& A, const Point& B, const std::vector<Point>& points);
    std::vector<Point> pointsBelowLine(const Point& A, const Point& B, const std::vector<Point>& points);

    Point findFurthestPoint(const Point& A, const Point& B, const std::vector<Point>& points);

    void quickHullHelper(const Point& A, const Point& B, const std::vector<Point>& points, std::unordered_set<Point, PointHash>& resultSet, std::vector<Action>& visualizationSteps, int depth);

    std::vector<Point> quickHullAlgorithm(std::vector<Action>& visualizationSteps);

private:
    std::vector<Point> field;
};

