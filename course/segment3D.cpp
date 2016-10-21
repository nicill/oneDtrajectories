//
// Created by yago on 16/03/31.
//

#include "segment3D.h"

segment3D::segment3D() //Default constructor
{


}
segment3D::segment3D(double i, double j, double k, double l,double m,double n) //constructor that receives the coordinates of the points
{
    start=point3D(i,j,k);
    end=point3D(l,m,n);
}
segment3D::segment3D(point3D init, point3D final) //constructor that receives two points
{
    start=point3D(init);
    end=point3D(final);
}

segment3D::segment3D(const segment3D &s) //Copy constructor
{
    start = point3D(s.start);
    end = point3D(s.end);
}

// accessor methods
point3D segment3D::getStart()const
{
    return start;
}

point3D segment3D::getEnd()const
{
    return end;
}

void segment3D::setStart(point3D p)
{
    start=p;
}

void segment3D::setEnd(point3D p)
{
    end=p;
}

//Operators
void segment3D::operator=(segment3D s) //assignment
{
    start=s.start;
    end=s.end;
}