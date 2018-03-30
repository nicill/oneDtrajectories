//
// Created by yago on 16/04/19.
//

#include "athlete.h"

void athlete::addTimePoint(oneDTimePoint p)
{
    // if we get here we suppose that the athlete was not previously known to be a cheater
    //( raceResults.size() > 0 ) ? ( (raceResults[raceResults.size() - 1].getPos() > p.getPos() ) ? throw("Exception at athlete::void addTimePoint(oneDTimePoint p) timePoint is not in order  ") : raceResults.push_back(p)  ) : raceResults.push_back(p);};

    if ( ( raceResults.size() > 0 ) && (raceResults[raceResults.size() - 1].getPos() > p.getPos() ))
    {
        this->setCheater(true);
    }

    // Do this for everyone
    raceResults.push_back(p);
}




bool athlete::checkConsistency()// return true if the time points are properly sorted
{
    bool search=true;
    int i=1;

    if(raceResults.size()<1)return true;

    while(i<raceResults.size()&&search)
    {
       // std::cout<<"checking if "<<raceResults[i-1].getTime()<<" is bigger than" <<raceResults[i].getTime()<<std::endl;

        if(raceResults[i-1].getTime()>raceResults[i].getTime() ){search=false; }
        ++i;
    }

    return search;
}

void athlete::computeAllPaceIntervals() {
    // std::cout<<"checking PACE INTERVALS "<<std::endl;

    double topSpeed=200;
    double topAcceleration=3;
    double maxSuddenAccelaration=0.75;
    double accumulatedDistance=0;
    double accumulatedTime=0;

    for (int i = 0; i < raceResults.size(); i++) {
        oneDTimePoint previous;

        accumulatedDistance=accumulatedDistance+raceResults[i].getPos();
        accumulatedTime=accumulatedTime+raceResults[i].getTime().toSeconds();
        averagePace=1000*accumulatedTime/accumulatedDistance;

        // start at event 0 and go on
        if (i == 0) {
            previous = oneDTimePoint(0, 0, 0);
        }
        else {

            previous = raceResults[i - 1];
        }
        paceIntervals.push_back(raceResults[i].computePace(previous));
        // vigilar cheaters aqui

        // Check for possible cheaters
        if ((paceIntervals[i] < topSpeed) || ((paceIntervals.size() > 1) &&
                                                                     (paceIntervals[i-1] /
                                                                      paceIntervals[i] > topAcceleration)) || (paceIntervals[i]<averagePace*maxSuddenAccelaration) )
        {
            if ((paceIntervals[i] < topSpeed) && (paceIntervals[i] > 170)  ){std::cout<<name<<"FASTER THAN LIGHT at interval "<<paceIntervals.size()<<" speed "<<paceIntervals[paceIntervals.size() - 1]<<std::endl;}
            if ((paceIntervals[i-1] / paceIntervals[i] > topAcceleration)){std::cout<<name<<"MAD ACCELERATION at interval "<<paceIntervals.size()<<" ratio "<<paceIntervals[i - 1] / paceIntervals[i]<<std::endl;}
            if (paceIntervals[i]<averagePace*maxSuddenAccelaration){std::cout<<name<<"SUDDEN ACCELERATION at interval "<<paceIntervals.size()<<" ratio "<<paceIntervals[i] / averagePace<<std::endl;}


            cheater = true;
        }


    }

}

void athlete::computePathAtIthMat(int i)
{
    if(paceIntervals.size()>=i)
    {
        oneDTimePoint previous;

        // start at event 0 and go on
        if(i==0)
        {
            previous=oneDTimePoint(0,0,0);
        }
        else
        {
            previous=raceResults[i-1];
        }
        paceIntervals.push_back(raceResults[i].computePace(previous));
    }
    else
    {
        std::cout<<" athlete::computePathAtIthMat something Funky happening with previous pace intervals, not enough computed "<<std::endl;
        throw(" athlete::computePathAtIthMat something Funky happening with previous pace intervals, not enough computed ");
    }
}

double athlete::getPreviousPosition(double currentPos) {
    int size=raceResults.size();
    double returnValue=0;
    for (unsigned i=0; i < size; i++) {
       if(raceResults[i].getPos()>currentPos) return returnValue;
       else returnValue=raceResults[i].getPos();
    }
    return returnValue;
}

RunningTime athlete::getPreviousTime(double currentPos) {
    int size=raceResults.size();
    RunningTime returnValue=RunningTime(0,0);
    for (unsigned i=0; i < size; i++) {
        if(raceResults[i].getPos()>currentPos) return returnValue;
        else returnValue=raceResults[i].getTime();
    }
    return returnValue;
}
