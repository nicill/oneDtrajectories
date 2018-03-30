//
// Created by yago on 16/04/20.
//

#ifndef ONEDTRAJECTORIES_EVENT_H
#define ONEDTRAJECTORIES_EVENT_H


#include "oneDTimePoint.h"
#include "athlete.h"

class event {

    // association between oneDtimePoints and athletes
    athlete *a;
    oneDTimePoint tp;

public:

    event(){};

    event(athlete *at,oneDTimePoint tpP)
    {
        a=at;
        tp=tpP;
    }

    athlete* getWho(){return a;};
    oneDTimePoint getPointAndTime(){return tp;};

    //define comparing function so a vector of these can be sorted
    bool operator < (const event& e) const
    {
        return (tp.getTime() < e.tp.getTime());
    }

    void write(std::ostream& os)
    {
        os << "Event: " << std::endl;;
        os << " Atleta: " << *a ;
        os << " temps: " << tp ;
    }

    friend std::ostream& operator<<(std::ostream& os, event e )
    {
        e.write(os);
        return os;
    }



};


#endif //ONEDTRAJECTORIES_EVENT_H
