#pragma once

#include <vector>
#include "Field.h"

// Gift Wrapping
class JervisMarch : public Point, PointHash
{
public:
    JervisMarch(const std::vector<Point>& field);
    ~JervisMarch() = default;

    std::vector<Point> jervisMarchAlgorithm(std::vector<Action>& visualizationSteps);
    float crossProduct(const Point& A, const Point& B, const Point& C);
    Point leftmostPoint(const std::vector<Point>& points);
private:
    std::vector<Point> field;
};

