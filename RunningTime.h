//
// Created by yago on 16/04/19.
//

#ifndef ONEDTRAJECTORIES_RUNNINGTIME_H
#define ONEDTRAJECTORIES_RUNNINGTIME_H

#include <iostream>
#include <fstream>

class RunningTime {

    int minutes;
    int seconds;

public:

    RunningTime(){}; //Default constructor
    RunningTime(int m, int s) //parameter constructor
    {
          minutes=m;
          seconds=s;
    }

    RunningTime(int t) //constructor only with seconds
    {
        //std::cout<<"running time constructor "<<t<<std::endl;
        minutes=t/60;
        seconds=t-(t/60)*60;
        //std::cout<<"min sec"<<minutes<<" "<<seconds<<std::endl;

    }

    RunningTime(const RunningTime &tprima) //Copy constructor
    {
        minutes=tprima.minutes;
        seconds=tprima.seconds;
    }

    int getM(){return minutes;};
    int getS(){return seconds;};

    void setM(int m){minutes=m;};
    void setS(int s){seconds=s;};

    //Operators
    void operator=(RunningTime tprima) //assignment operator
    {
        minutes=tprima.getM();
        seconds=tprima.getS();
    }

    bool operator < (const RunningTime& rt) const
    {
        if(minutes==rt.minutes){return seconds<rt.seconds;}
        else{return minutes<rt.minutes;}
    }

    bool operator > (const RunningTime& rt) const
    {
        if(minutes==rt.minutes){return seconds>rt.seconds;}
        else{return minutes>rt.minutes;}
    }

    bool operator == (const RunningTime& rt) const
    {
        return ((minutes==rt.minutes)&&(seconds==rt.seconds));
    }

    int operator-(const RunningTime& rt) const
    {
        int firstValue=minutes*60+seconds;
        int secondValue=rt.minutes*60+rt.seconds;

        return firstValue-secondValue;
    }

    void write(std::ostream& os)
    {
        os << "[" <<  minutes << ":" << seconds << "]";
    }
    friend std::ostream& operator<<(std::ostream& os, RunningTime t )
    {
        t.write(os);
        return os;
    }

    int difference(RunningTime t); // return the difference in seconds between two time points

    int toSeconds(){return minutes*60;seconds;}

};


#endif //ONEDTRAJECTORIES_TIME_H
