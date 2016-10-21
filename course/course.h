//
// Created by yago on 16/03/31.
//

#ifndef ONEDTRAJECTORIES_COURSE_H
#define ONEDTRAJECTORIES_COURSE_H


#include "segment3D.h"
#include "segment1D.h"
#include <vector>

using namespace std;

class course {

    // these two vectors should have the same number of elements and the lenghts in each vecotr should be the same
    // 3D course, vector of 3D segments
    vector<segment3D> course3D;
    // 1D course, vector of 1D segments
    vector<segment1D> course1D;

    vector<double> slope; // store the average slope in the segment
    // store a data structure for 3D point search?

    public:

    course(); //Default constructor
    course(const course &s); //Copy constructor
// fer una creadora a partir d'un stream de punts 3D

    //Operators
    void operator=(course s); //assignment

    // accessor methods
    int inWhat3Dsegment(point3D p); // given a 3D point find which 3D segment is closest.
    double toOneD(point3D p); // given a point in 3D space, find it in the 3D course and then translate it to 1D space

};


#endif //ONEDTRAJECTORIES_COURSE_H
