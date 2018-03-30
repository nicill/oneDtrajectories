//
// Created by yago on 16/04/20.
//

#ifndef ONEDTRAJECTORIES_GROUPCOMPUTATIONS_H
#define ONEDTRAJECTORIES_GROUPCOMPUTATIONS_H


#include "oneDTimePoint.h"
#include "event.h"
#include "mat.h"
#include "groupRelations.h"
#include <algorithm>
#include <vector>
#include <list>

class groupComputations {

    std::vector<event> events; //sort this in terms of time,
   // http://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects

    std::vector<mat> mats;
    std::vector< std::list< groupRelations* > > relationsBackward; // maintain the relations between every groups and groups from the previous mat,sorted according to the mats in the list for every mat we have
    std::vector< std::list< groupRelations* > > relationsForward; // maintain the relations between every groups and groups from the next mat,sorted according to the mats in the list for every mat we have

    int groupThreshold;
  //  double jackardThreshold;
    double inclusionThreshold;
    double epsilon;

    public:

    typedef std::vector<event>::iterator iterator;
    typedef std::vector<event>::const_iterator const_iterator;

    iterator begin() { return events.begin(); }
    iterator end() { return events.end(); }


    groupComputations()
    {
        events=std::vector<event>();
        mats=std::vector<mat>();
        relationsBackward=std::vector< std::list< groupRelations* > >();
        relationsForward=std::vector< std::list< groupRelations* > >();
    }


    double getEpsilon() const {
        return epsilon;
    }

    void setEpsilon(double epsilon) {
        groupComputations::epsilon = epsilon;
    }

    int getGroupThreshold() const {return groupThreshold;}
    void setGroupThreshold(int groupThreshold) {groupComputations::groupThreshold = groupThreshold;}

    double getInclusionThreshold() const {return inclusionThreshold;}
    void setInclusionThreshold(double inclusionThreshold) {groupComputations::inclusionThreshold = inclusionThreshold;}
//    double getJackardThreshold() const {return jackardThreshold;}
 //   void setJackardThreshold(double jackardThreshold) {groupComputations::jackardThreshold = jackardThreshold;}

    void addEvent(event e){events.push_back(e);};
    void sortEvents(){std::sort(events.begin(), events.end());}

    int findMat(double m); //return the position of the mat if it is there, if it is not, just add it and return the new index, if the new mat would not be the last, throws exception

    std::list<event> upsampleEvent(groupComputations::iterator it, double upsampleStep);

    void processEvent(groupComputations::iterator it);

    int numberOfEvents(){return events.size();};

    void computeGroupRelations();
    void computeIndividualGroupBehaviour();

    int numberOfMats(){return mats.size();}

    std::list<component*>* longestSurvivingGroups();// return all the groups that survive for a longer interval anywhere
    std::list<component*>* longestSurvivingGroups(int matNumber);// given a starting mat, return all the groups that survive for a longer interval

    std::list<component*>* longestRelatedGroupForward();// return all the groups that are included forward for any interval
    std::list<component*>* longestRelatedGroupBackward();// return all the groups that are included backward for any interval

    int countSurvives();
    int countMerges();
    int countSplits();
    int countExpands();
    int countShrinks();
    int countCoheres();
    int countDisbands();
    int countAppears();
    int countDissappears();

    event lastEvent()
    {
        if(events.size()==0)throw("Asking for the last event in an empty group computation!!!!");
        return events[events.size()-1];
    }

    void closeMats();// add one last runner to every mat so it closes all groups

    void write(std::ostream& os)
    {
        int size=events.size();
        os <<"Writing component computations with "<<size<<" events"<<std::endl;
        //for (unsigned i=0; i < size; i++) {
        //    os << "Athlete : " << events[i].getWho().getId()<<" ( "<<events[i].getWho().getGender()<<" )" << "passing " << events[i].getPointAndTime() << std::endl;
        //}
        int sizeMats=mats.size();
        os << "MAT Positions: ";
        for (unsigned i=0; i < size; i++) {
            os << mats[i].getId()<<" at position  "<<mats[i].getPosition() << std::endl;
        }

    }

    friend std::ostream& operator<<(std::ostream& os, groupComputations gc )
    {
        gc.write(os);
        return os;
    }
};


#endif //ONEDTRAJECTORIES_GROUPCOMPUTATIONS_H
