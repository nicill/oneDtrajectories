//
// Created by yago on 16/03/31.
//

#ifndef ONEDTRAJECTORIES_SEGMENT3D_H
#define ONEDTRAJECTORIES_SEGMENT3D_H


#include "point3D.h"

class segment3D {

    // start and end point
    point3D start;
    point3D end;

public:

    segment3D(); //Default constructor
    segment3D(double i, double j, double k, double l,double m,double n); //constructor that receives the coordinates of the points
    segment3D(point3D init, point3D final); //constructor that receives two points

    segment3D(const segment3D &s); //Copy constructor

    // accessor methods
    point3D getStart()const;
    point3D getEnd()const;

    void setStart(point3D p);
    void setEnd(point3D p);

    //Operators
    void operator=(segment3D s); //assignment

};


#endif //ONEDTRAJECTORIES_SEGMENT3D_H
