#include <iostream>
#include <future>
#include <execution>

#include "JervisMarch.h"

JervisMarch::JervisMarch(const std::vector<Point>& field) : field(field) {};

Point JervisMarch::leftmostPoint(const std::vector<Point>& points) 
{
    return *std::min_element(points.begin(), points.end(), [](const Point& a, const Point& b) 
    {
        return a.x < b.x;
    });
}

float JervisMarch::crossProduct(const Point& A, const Point& B, const Point& C) 
{
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

std::vector<Point> JervisMarch::jervisMarchAlgorithm(std::vector<Action>& visualizationSteps)
{
    if (this->field.size() < 3)
    {
        return {};
    }

    std::vector<Point> hull;

    Point leftmost = leftmostPoint(this->field);
    Point current = leftmost;

    do 
    {
        hull.push_back(current);

        // log current point
        visualizationSteps.push_back({ Action::POINT, current });

        Point next = this->field[0];

        std::for_each(std::execution::par, this->field.begin(), this->field.end(), [&](const Point& candidate) 
        {
            if (next == current || crossProduct(current, next, candidate) < 0)
            {
                next = candidate;
            }
        });

        // log edge between current and next point
        visualizationSteps.push_back({ Action::EDGE, current, next });

        current = next;

    } while (current != leftmost);

    return hull;
}