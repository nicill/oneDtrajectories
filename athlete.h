//
// Created by yago on 16/04/19.
//

#ifndef ONEDTRAJECTORIES_ATHLETE_H
#define ONEDTRAJECTORIES_ATHLETE_H

#include <vector>
#include <exception>
#include "oneDTimePoint.h"
#include <string>

class athlete {

    std::vector< oneDTimePoint >  raceResults;
    std::vector< double >  paceIntervals;

    std::string gender;
    std::string name;
    std::string division;
    int id;
    bool cheater;
    double averagePace;

    std::vector<int> groupList; // the position stands for the mat number (in the component computation part) and the int is the identifier of the component it belongs to
    std::vector<int> classificationList; // the place in race order where the athlete is at every mat (following the order in the calculations)

public:

    athlete()
    {
        groupList=std::vector<int>(0);
        classificationList=std::vector<int>(0);
        paceIntervals=std::vector<double>(0);
        cheater=false;
        averagePace=-1;
    };//default constructor

    athlete(const athlete& other)
    {
        raceResults= other.raceResults;
        paceIntervals= other.paceIntervals;
        gender=other.gender;
        name=other.name;
        division=other.division;
        id=other.id;
        cheater=other.cheater;// indicates whether or not the current athlete has been found to be a cheater
        groupList=other.groupList;
        classificationList=other.classificationList;
        averagePace=other.averagePace;

    };//Copy constructor


    athlete& operator=(const athlete& other)
    {
        raceResults= other.raceResults;
        paceIntervals=other.paceIntervals;
        gender=other.gender;
        name=other.name;
        division=other.division;
        id=other.id;
        cheater=other.cheater;// indicates whether or not the current athlete has been found to be a cheater
        groupList=other.groupList;
        classificationList=other.classificationList;
        averagePace=other.averagePace;

        return *this;

    }
    // Setters and getters
    void setGender(std::string s){gender=s;};
    void setName(std::string s){name=s;};
    void setDivision(std::string s){ division =s;};
    void setId(int i){id=i;};

    bool isCheater() const {return cheater;}
    void setCheater(bool cheater) {athlete::cheater = cheater;}

    std::string getGender(){return gender;};
    std::string getName(){return name;};
    std::string getDivision(){return division;};
    int getId(){return id;};

    // other functions
    void addTimePoint(oneDTimePoint p);

    bool checkConsistency();// return true if the time points are properly sorted
    double paceAtIthMat(int i){return paceIntervals[i];};
    int groupAtIthMat(int i){return groupList[i];};
    int posAtIthMat(int i){return classificationList[i];};

    void setGroupAtIthMat(int i,int group){groupList[i]=group;};
    void setPaceAtIthMat(int i,double pace){paceIntervals[i]=pace;};

    void setNextGroup(int i)
    {
        groupList.push_back(i);
    };
    void setNextClassification(int i)
    {
       // std::cout<<"setting next classification "<<name<<" at mat "<<classificationList.size()<<" classification "<<i<<std::endl;
        classificationList.push_back(i);
    };

    double getAveragePace() const {
        return averagePace;
    }

    void setAveragePace(double averagePace) {
        athlete::averagePace = averagePace;
    }

    int getLastClassification(){return classificationList.back();}

    void computePathAtIthMat(int i);// should have already computed all the previous paces
    void computeAllPaceIntervals();

    double getPreviousPosition(double currentPos);
    RunningTime getPreviousTime(double currentPos);

    void write(std::ostream& os)
    {
        int size=raceResults.size();
        os << "Id: " << id ;
        os << " Name: " << name ;
        os << " Gender: " << gender ;
        os << " Division: " << division << std::endl;


        for (unsigned i=0; i < size; i++) {
            os << "Time Point: " << raceResults[i] <<std::endl;
        }
        size=classificationList.size();
        for (unsigned i=0; i < size; i++) {
            os << "Classification at Mat: "<<i<<" "<< classificationList[i] <<std::endl;
        }
        size=groupList.size();
        for (unsigned i=0; i < size; i++) {
            os << "Group at MAT: " <<i<<" "<< groupList[i] <<std::endl;
        }
        size=paceIntervals.size();
        for (unsigned i=0; i < size; i++) {
            os << "Pace at MAT: " <<i<<" "<< paceIntervals[i] <<std::endl;
        }

    }
    friend std::ostream& operator<<(std::ostream& os, athlete a )
    {
        a.write(os);
        return os;
    }


};


#endif //ONEDTRAJECTORIES_ATHLETE_H
