#ifndef ConvexContains_h
#define ConvexContains_h
#include <SFML/Graphics.hpp>
#include <vector>
#include "math.h"

#endif /* ConvexContains_h */

float distance(sf::Vector2f p1, sf::Vector2f p2){
    float delX = p1.x - p2.x;
    float delY = p1.y - p2.y;
    return sqrt(delX * delX + delY * delY);
}

struct Line{
    sf::Vector2f p1;
    sf::Vector2f p2;

    // Checks if s line contains a point (within a certain margin)
    bool contains(sf::Vector2f point) const{
        float margin = 1;
        return std::abs((distance(p1, point) + distance(point, p2)) - distance(p1, p2)) < margin;
    }
};

bool contains(sf::ConvexShape shape, sf::Vector2f point);
std::vector<sf::Vector2f> getIntersectionPoints(sf::ConvexShape shape, sf::Vector2f point);
sf::Vector2f intersection(Line lineA, Line lineB);

bool contains(sf::ConvexShape shape, sf::Vector2f point){
    std::vector<sf::Vector2f> intersectPoints = getIntersectionPoints(shape, point);
    
    // Rule for collision with convex shapes: if we draw a horizontal line through a point
    // and the line intersects on the left of the point an odd number of times and intersects
    // on the right an odd number if times, the point is inside the shape. If not, it isn't
    int nodesAtLeft = 0;
    int nodesAtRight = 0;
    for (sf::Vector2f p : intersectPoints){
        if (p.x < point.x){
            nodesAtLeft++;
        }
        else if(p.x > point.x){
            nodesAtRight++;
        }
    }
    return ((nodesAtLeft % 2) == 1) && ((nodesAtRight % 2) == 1);
}

std::vector<sf::Vector2f> getIntersectionPoints(sf::ConvexShape shape, sf::Vector2f point){
    std::vector<sf::Vector2f> intersectPoints;
    sf::Vector2f p;
    bool crossingLine;  // This will be used to avoid duplicated points on special cases

    if (shape.getPointCount() < 3){
        return intersectPoints;
    }

    sf::FloatRect bounds = shape.getLocalBounds();

    // To determine horizontal line, we use two points, one at leftmost side of the shape (in fact, its bound) and the other at rightmost side
    Line pointLine, shapeLine;
    pointLine.p1 = sf::Vector2f(bounds.left, point.y);
    pointLine.p2 = sf::Vector2f(bounds.left + bounds.width, point.y);

    unsigned int nPoints = shape.getPointCount();

    for (int i = 0; i < nPoints; ++i){
        shapeLine.p1 = shape.getPoint(i % nPoints);         // Last one will be nPoints-1
        shapeLine.p2 = shape.getPoint((i + 1) % nPoints);   // So this one must be 0 in order to check last side (returning to origin)
        crossingLine = (shapeLine.p1.y >= point.y && shapeLine.p2.y <= point.y) || (shapeLine.p2.y >= point.y && shapeLine.p1.y <= point.y);
        p = intersection(shapeLine, pointLine);
        if (crossingLine && shapeLine.contains(p))
            intersectPoints.push_back(p);
    }
    return intersectPoints;
}

sf::Vector2f intersection(Line lineA, Line lineB){
    int x1 = lineA.p1.x;
    int y1 = lineA.p1.y;
    int x2 = lineA.p2.x;
    int y2 = lineA.p2.y;

    int x3 = lineB.p1.x;
    int y3 = lineB.p1.y;
    int x4 = lineB.p2.x;
    int y4 = lineB.p2.y;

    double retX = 0.0;
    double retY = 0.0;
    
    if(((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4)) != 0) // Ensures that we don't get a divide by zero error
        retX = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4)) / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
    if((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4) != 0)   // Ensures that we don't get a divide by zero error
        retY = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4)) / ((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
    
    return sf::Vector2f(retX, retY);
}
