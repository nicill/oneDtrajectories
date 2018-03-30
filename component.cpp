//
// Created by yago on 16/04/21.
//

#include "component.h"
#include <iostream>

RunningTime component::getLastTime() {return groupedEvents[groupedEvents.size()-1]->getPointAndTime().getTime();}
int component::getLastCurrentClassification() {return groupedEvents[groupedEvents.size()-1]->getWho()->getLastClassification();}
int component::getFirstCurrentClassification() {return groupedEvents[0]->getWho()->getLastClassification();}


bool component::addEvent(event *e) //return true if it just became a group
{
    groupedEvents.push_back(e);

    //std::cout<<"Adding event with athlete "<<e->getWho()->getId()<<" to component with id "<<id<<" CURRENT NUMBER OF EVENTS IN COMPONENT "<<groupedEvents.size()<<" group threshold is "<<groupThreshold<<std::endl;

    // checked if we have become a group
    if(groupedEvents.size()==groupThreshold)
    {
        setThisAGroup(true);
        //std::cout<<"component with id "<<id<<" JUST BECAME A GROUP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! MAT POSITION IS "<<matPosition<<std::endl;
        return true;
    }

    return false;

}

void component::addPreviousRelatedGroup(component *precedent)
{
    //std::cout<<"                                                                                                                                 AddingPreviously related Griput to group with "<<id<<" at mat "<<matPosition<<" group with id "<<precedent->getId()<<" at mat "<<precedent->matPosition<<std::endl;
    // annotate previous group in this list
    relatedGroupsBefore.push_back(precedent);

    // also connect the previous to this
//    precedent->relatedGroupsAfter.push_back(this);
//    std::cout<<"WRITTING PREVIOUS COMPONENT "<<*precedent<<std::endl;

}

void component::addSubsequentRelatedGroup(component *next)
{
   // std::cout<<"                                                                                                                                 AddingSUBSEQ related Griput to group with "<<id<<" at mat "<<matPosition<<" group with id "<<next->getId()<<" at mat "<<next->matPosition<<std::endl;

    // annotate previous group in this list
    relatedGroupsAfter.push_back(next);
    // also connect the previous to this
  //  next->relatedGroupsBefore.push_back(this);

}

int component::maximumSurvivedSteps()
{
 // if(hasSurvived()||willSurvive())  std::cout<<" component::maximumSurvivedSteps() "<<this->matPosition<<" "<<relatedGroupsBefore.size()<<" "<<relatedGroupsAfter.size()<<" "<<hasSurvived()<<" "<<willSurvive()<<std::endl;
    if(!willSurvive())return 1;
    else { return (doubleAfter->maximumSurvivedSteps()+1);}
}

std::list<std::list<event *> >* component::core(int numStepsForward)
{
    std::list<std::list<event *> > *returnValue = new std::list<std::list<event *> >();

    // first get the list of


    return returnValue;
}

int component::maximumIncludedStepsForward() {
    if(!isIncludedForward())return 1;
    else { return (includedAfter->maximumIncludedStepsForward()+1);}
}

int component::maximumIncludedStepsBackward() {
    if(!isIncludedBackward())return 1;
    else { return (includedBefore->maximumIncludedStepsBackward()+1);}
}

std::string component::listSuccessorSurvivors() {
    if(!willSurvive())return std::string(" ").append(std::to_string(getId()));
    else {
        std::string s=" Position: ";
        s.append(std::to_string((int)matPosition));
        s.append(", ID: ");
        s.append(std::to_string(getId()));
        return s.append(doubleAfter->listSuccessorSurvivors());}
}

std::string component::listSuccessorRelated() {
    if(!isIncludedForward())return std::string(" final ID: ").append(std::to_string(getId()));
    else
    {
        std::string s=" Position: ";
        s.append(std::to_string((int)matPosition));
        s.append(", ID: ");
        s.append(std::to_string(getId()));
        return s.append(includedAfter->listSuccessorRelated());
    }
}

std::string component::listPredecessorRelated() {
    if(!isIncludedBackward())return std::to_string(getId()).append(std::string(" <-final ID "));
    else
    {
        std::string s=" Position: ";
        s.append(std::to_string((int)matPosition));
        s.append(", ID: ");
        s.append(std::to_string(getId()));
        return includedBefore->listPredecessorRelated().append(s);
    }
}

int component::getLastClassificationIthMat(int matNumber) {return groupedEvents[groupedEvents.size()-1]->getWho()->getIthClassification(matNumber); }

int component::getFirstClassificationIthMat(int matNumber) {return groupedEvents[0]->getWho()->getIthClassification(matNumber);}
