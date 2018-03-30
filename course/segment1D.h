//
// Created by yago on 16/03/31.
//

#ifndef ONEDTRAJECTORIES_SEGMENT1D_H
#define ONEDTRAJECTORIES_SEGMENT1D_H


class segment1D {

    double start;
    double end;

public:

    segment1D(); //Default constructor
    segment1D(double init, double final); //constructor that receives two doubles

    segment1D(const segment1D &s); //Copy constructor

    // accessor methods
    double getStart()const;
    double getEnd()const;

    void setStart(double p);
    void setEnd(double p);

    //Operators
    void operator=(segment1D s); //assignment

};


#endif //ONEDTRAJECTORIES_SEGMENT1D_H
