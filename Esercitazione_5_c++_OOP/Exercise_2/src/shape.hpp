#ifndef __SHAPE_H
#define __SHAPE_H

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace ShapeLibrary {

class Polygon {
private:
    MatrixXd points;
public:
    Polygon() = default;

    Polygon(MatrixXd& points) : points(points) {};

    string Show()
    {
        return "Polygon: " + to_string(Area());
    }

    double Area()
    {
        int n = points.cols();          // Numero di colonne della
        double sum = 0.0;
        for (int i = 0; i < n; i++)     // Calcolo dell'area del poligono
        {
            sum += points(0,i)*points(1,(i+1)%n)-points(0,(i+1)%n)*points(1,i);
        }
        return 0.5 * abs(sum);          // Restituisce l'area del poligono
    }

    MatrixXd getPoints() const
    {
        return points;
    }
};


class Triangle : public Polygon
{
private:
    double base=0, height=0;
public:
    Triangle() = default;

    Triangle(double& base,
               double& height) : base(base), height(height) {}

    Triangle(MatrixXd& points) : Polygon(points) {};

    double AreaTriangle()
    {
        return base * height / 2;
    }

    string Show()
    {
        if (base > 0 && height > 0)
        {
            return "Triangle: " + to_string(AreaTriangle());
        }
        else
        {
            return "Triangle As Polygon: " + to_string(Area());
        }
    }
};


class TriangleEquilateral final : public Triangle
{
private:
    double edgeLength=0;
public:
    TriangleEquilateral(double& edgeLength) : edgeLength(edgeLength) {};

    TriangleEquilateral(double& base,
                        double& height) : Triangle(base, height) {};

    double AreaTriangleEquilateral()
    {
        return sqrt(3) * edgeLength * edgeLength / 4;
    }

    string Show()
    {
        if (edgeLength > 0)
        {
            return "TriangleEquilateral: " + to_string(AreaTriangleEquilateral());
        }
        else
        {
            return "TriangleEquilateral As Triangle: " + to_string(AreaTriangle());
        }
    }
};

class Quadrilateral : public Polygon
{
private:
    ShapeLibrary::Triangle t1, t2;
public:
    Quadrilateral() = default;

    Quadrilateral(MatrixXd& points) : Polygon(points) {};

    Quadrilateral(Triangle& t1, Triangle& t2) : t1(t1), t2(t2) {};

    double AreaQuadrilateral(Triangle& t1, Triangle& t2)
    {
        return t1.Area()+t2.Area() ;
    }

    string Show()
    {
        if (t1.Area()>0 && t2.Area()>0)
        {
            return "Quadrilateral: " + to_string(AreaQuadrilateral(t1, t2));
        }
        else
        {
            return "Quadrilateral As Polygon: " + to_string(Area());
        }
    }
};

class Rectangle : public Quadrilateral
  {
private:
    double base=0, height=0;
    ShapeLibrary::Triangle t1, t2;
public:
    Rectangle() = default;

    Rectangle(MatrixXd& points) : Quadrilateral(points) {};

    Rectangle(Triangle& t1, Triangle& t2) : t1(t1), t2(t2) {};

    Rectangle(double& base, double& height) : base(base), height(height) {};

    double AreaRectangle(double& base, double& height)
    {
        return base * height ;
    }

    string Show()
    {
        if (t1.Area() > 0 && t2.Area() > 0)
        {
            return "Rectangle As Quadrilateral: " + to_string(AreaQuadrilateral(t1, t2));
        }
        else if (base > 0 && height > 0)
        {
            return "Rectangle: " + to_string(AreaRectangle(base, height));
        }
        else
        {
            return "Rectangle As Polygon: " + to_string(Area());
        }
    }
};

class Square final: public Rectangle
{
private:
    double base=0, height=0, edgeLength=0;
    ShapeLibrary::Triangle t1, t2;
public:
    Square(MatrixXd& points) : Rectangle(points) {};

    Square(Triangle& t1, Triangle& t2) : t1(t1), t2(t2) {};

    Square(double& base, double& height) : base(base), height(height) {};

    Square(double& edgeLength) : edgeLength(edgeLength) {};

    double AreaSquare()
    {
        return edgeLength * edgeLength ;
    }

    string Show()
    {
        if (t1.Area()>0 && t2.Area()>0)
        {
            return "Square As Quadrilateral: " + to_string(AreaQuadrilateral(t1, t2));
        }
        else if (base > 0 && height > 0)
        {
            return "Square As Rectangle: " + to_string(AreaRectangle(base, height));
        }
        else if (edgeLength > 0)
        {
            return "Square: " + to_string(AreaSquare());
        }
        else
        {
            return "Square As Polygon: " + to_string(Area());
        }
    }
};

}

#endif // __SHAPE_H
