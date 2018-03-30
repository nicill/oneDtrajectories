//
// Created by yago on 16/04/21.
//

#ifndef ONEDTRAJECTORIES_GROUP_H
#define ONEDTRAJECTORIES_GROUP_H


#include "event.h"
#include <vector>
#include <list>

class component {

    int id;
    std::vector<event *> groupedEvents; //sorted collection of events
    int groupThreshold; //the number that will be used to define a group threshold
    double epsilon;
    double inclusionT;

    double matPosition; // what mat are we in (its position)

    bool thisAGroup=false;

    std::list<component*> relatedGroupsBefore;
    std::list<component*> relatedGroupsAfter;

    component* includedBefore;
    component* includedAfter;

    component* doubleBefore;
    component* doubleAfter;

    int numEntitiesSharedBefore;
    int numEntitiesSharedAfter;

public:
    component(int id, double eps, int gT, double pos, double inc) : id(id) , epsilon(eps) , groupThreshold(gT), matPosition(pos),inclusionT(inc){
       // std::cout<<"Creating component with id "<<id<<std::endl;
        thisAGroup=false;
        relatedGroupsBefore=std::list<component*>();
        relatedGroupsAfter=std::list<component*>();

        includedBefore= nullptr;
        includedAfter= nullptr;

        doubleAfter= nullptr;
        doubleBefore= nullptr;

        numEntitiesSharedAfter=0;
        numEntitiesSharedBefore=0;
    }

    //copy constructor
    component(const  component &cprima) //Copy constructor
    {
        id=cprima.id;
        groupThreshold=cprima.groupThreshold;
        epsilon=cprima.epsilon;
        inclusionT=cprima.inclusionT;
        thisAGroup=cprima.thisAGroup;
        matPosition=cprima.matPosition;

        groupedEvents=std::vector<event*>(cprima.groupedEvents.begin(),cprima.groupedEvents.end());
        std::list<component*> relatedGroupsBefore=std::list<component*>(cprima.relatedGroupsBefore.begin(),cprima.relatedGroupsBefore.end());
        std::list<component*> relatedGroupsAfter=std::list<component*>(cprima.relatedGroupsAfter.end(),cprima.relatedGroupsAfter.end());

        includedBefore=cprima.includedBefore;
        includedAfter= cprima.includedAfter;

        doubleAfter= cprima.doubleAfter;
        doubleBefore= cprima.doubleBefore;

        numEntitiesSharedBefore=cprima.numEntitiesSharedBefore;
        numEntitiesSharedAfter=cprima.numEntitiesSharedAfter;

    }


    component& operator=(const component& cprima)
    {
        id=cprima.id;
        groupThreshold=cprima.groupThreshold;
        epsilon=cprima.epsilon;
        inclusionT=cprima.inclusionT;
        thisAGroup=cprima.thisAGroup;
        matPosition=cprima.matPosition;

        groupedEvents=std::vector<event*>(cprima.groupedEvents.begin(),cprima.groupedEvents.end());
        std::list<component*> relatedGroupsBefore=std::list<component*>(cprima.relatedGroupsBefore.begin(),cprima.relatedGroupsBefore.end());
        std::list<component*> relatedGroupsAfter=std::list<component*>(cprima.relatedGroupsAfter.end(),cprima.relatedGroupsAfter.end());

        includedBefore=cprima.includedBefore;
        includedAfter= cprima.includedAfter;

        doubleAfter= cprima.doubleAfter;
        doubleBefore= cprima.doubleBefore;

        numEntitiesSharedBefore=cprima.numEntitiesSharedBefore;
        numEntitiesSharedAfter=cprima.numEntitiesSharedAfter;

        return *this;

    }

    int getGroupThreshold() const {
        return groupThreshold;
    }

    void setGroupThreshold(int groupThreshold) {
        component::groupThreshold = groupThreshold;
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        component::id = id;
    }

    bool isThisAGroup() const { return thisAGroup;}
    void setThisAGroup(bool isThisAgroup) {component::thisAGroup = isThisAgroup;}

    RunningTime getLastTime();
    int getLastCurrentClassification();
    int getFirstCurrentClassification();

    int getLastClassificationIthMat(int matNumber);
    int getFirstClassificationIthMat(int matNumber);


    typedef std::vector<event*>::iterator iterator;
    typedef std::vector<event*>::const_iterator const_iterator;
    iterator begin() { return groupedEvents.begin(); }
    iterator end() { return groupedEvents.end(); }

    bool addEvent(event *e);

    int numberOfGroupedEvents(){return groupedEvents.size();}

    void addPreviousRelatedGroup(component *precedent); // the component next needs to be a group and hold some inclusion coefficient conditions
    void addSubsequentRelatedGroup(component *next); // the component next needs to be a group and hold some inclusion coefficient conditions

    int maximumSurvivedSteps();// return how many steps forward this group has doubly related groups
    int maximumIncludedStepsForward();// return how many steps forward this group has inclusion relations.
    int maximumIncludedStepsBackward();// return how many steps backward this group has inclusion relations.

   // std::list<std::vector<int> >* forwardGroup(int numStepsForward); // take the athletes in this group and see how many of them survive together for numStepsForward steps, account for possible alternative paths
    std::list<std::list<event *> >* core(int numStepsForward); // take the athletes in this group and see how many of them survive together for numStepsForward steps, account for possible alternative paths

   std::list<component *>* getRelatedGroupsBefore() {return &relatedGroupsBefore;}
   std::list<component *>* getRelatedGroupsAfter() {return &relatedGroupsAfter;}
    component *getIncludedBefore() const {
        return includedBefore;
    }

    void setIncludedBefore(component *includedBefore) {
      //  std::cout<<"///////////////////////////////////////////////component::setIncludedBefore linking this groups with "<<this->numberOfGroupedEvents()<<" entities to a guy in the previous step with this many "<<includedBefore->numberOfGroupedEvents()<<" "<<relatedGroupsBefore.size()<<std::endl;
        component::includedBefore = includedBefore;
    }

    component *getIncludedAfter() const {
        return includedAfter;
    }

    void setIncludedAfter(component *includedAfter) {
        //std::cout<<"\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\component::setIncludedafter linking this groups with "<<this->numberOfGroupedEvents()<<" entities to a guy in the nextstep with this many "<<includedAfter->numberOfGroupedEvents()<<std::endl;
        component::includedAfter = includedAfter;
    }

    component *getDoubleBefore() const {
        return doubleBefore;
    }

    void setDoubleBefore(component *doubleBefore) {
       // std::cout<<"Setting double before "<<matPosition<<" "<<doubleBefore<<std::endl;
        component::doubleBefore = doubleBefore;
    }

    component *getDoubleAfter() const {
        return doubleAfter;
    }

    void setDoubleAfter(component *doubleAfter) {
        //std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Setting double after "<<matPosition<<" "<<doubleAfter<<std::endl;
        component::doubleAfter = doubleAfter;
    }

    int getNumEntitiesSharedBefore() const {
        return numEntitiesSharedBefore;
    }

    void setNumEntitiesSharedBefore(int numEntitiesSharedBefore) {
        component::numEntitiesSharedBefore = numEntitiesSharedBefore;
    }

    int getNumEntitiesSharedAfter() const {
        return numEntitiesSharedAfter;
    }

    void setNumEntitiesSharedAfter(int numEntitiesSharedAfter) {
        component::numEntitiesSharedAfter = numEntitiesSharedAfter;
    }


    bool willSurvive(){return doubleAfter!= nullptr;}
    bool hasSurvived(){return doubleBefore!= nullptr;}

    bool appears(){return (!containsSomeoneBefore()&&!isIncludedBackward());}
    bool dissappears(){return (!containsSomeoneAfter()&&!isIncludedForward());}

    bool expands(){return ((relatedGroupsBefore.size()==1)&&!hasSurvived());}
    bool shrinks(){return ((relatedGroupsAfter.size()==1)&&!willSurvive());}

    bool merges(){
       // std::cout<<"component::merges group id "<<id<<" at mat "<<matPosition<<" booleans "<<(relatedGroupsBefore.size()>1)<<" "<<!hasSurvived()<<" "<<(numEntitiesSharedBefore/groupedEvents.size()>inclusionT)<< " numbers: "<<numEntitiesSharedBefore<<"/"<<groupedEvents.size()<<"="<<numEntitiesSharedBefore/groupedEvents.size()<<" >? "<<inclusionT<<std::endl;
        return (relatedGroupsBefore.size()>1)&&!hasSurvived()&&(numEntitiesSharedBefore/groupedEvents.size()>inclusionT);};
    bool splits(){return (relatedGroupsAfter.size()>1)&&!willSurvive()&&(numEntitiesSharedAfter/groupedEvents.size()>inclusionT);};

    bool coheres(){return (relatedGroupsBefore.size()>1)&&!hasSurvived()&&!merges(); };
    bool disbands(){return (relatedGroupsAfter.size()>1)&&!willSurvive()&&!splits();};


    bool isIncludedForward(){return includedAfter!= nullptr;}
    bool isIncludedBackward(){return includedBefore!= nullptr;}

    bool containsSomeoneBefore(){return relatedGroupsBefore.size()!=0;}
    bool containsSomeoneAfter(){return relatedGroupsAfter.size()!=0;}

    std::string listSuccessorSurvivors();
    std::string listSuccessorRelated();
    std::string listPredecessorRelated();

    void write(std::ostream& os)
    {
        int size=groupedEvents.size();
        os << "Writing Component, Id: " << id ;
        os << " Position: " << matPosition ;
        os << " Group?: " << thisAGroup ;
        os << " Sizes of previous and following surviving group list: "<< relatedGroupsBefore.size()<<" "<<relatedGroupsAfter.size()<<" \n";
        for (unsigned i=0; i < size; i++) {
            os << "Athlete here: " << (groupedEvents[i]->getWho()->getName()) <<std::endl;
        }

    }
    friend std::ostream& operator<<(std::ostream& os, component c)
    {
        c.write(os);
        return os;
    }

};


#endif //ONEDTRAJECTORIES_GROUP_H
