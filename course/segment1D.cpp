//
// Created by yago on 16/03/31.
//

#include "segment1D.h"

segment1D::segment1D() //Default constructor
{

}

segment1D::segment1D(double init, double final) //constructor that receives two doubles
{
    start=init;
    end=final;
}

segment1D::segment1D(const segment1D &s) //Copy constructor
{
    start=s.start;
    end=s.end;
}

// accessor methods
double segment1D::getStart()const
{
    return start;
}

double segment1D::getEnd()const
{
    return end;
}

void segment1D::setStart(double p)
{
    start=p;
}

void segment1D::setEnd(double p)
{
    end=p;
}

//Operators
void segment1D::operator=(segment1D s) //assignment
{
    start=s.start;
    end=s.end;
}