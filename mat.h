//
// Created by yago on 16/04/21.
//

#ifndef ONEDTRAJECTORIES_MAT_H
#define ONEDTRAJECTORIES_MAT_H


#include "component.h"
#include<vector>

class mat {

    double position;
    int id;
    std::vector<component*> components; // these are the maximal components, not even the groups, they depend on the group threshold
    int athletesPassed; // the number of athletes that passed by this mat

    double epsilon;
    int groupThreshold;
    std::vector<int> groups;
    int largestGroupId; // keep the id of the group with the most entities
    int largestGroupEntities;// keep also the number of entities


public:

    mat(double position, int id, double eps, int gt) : position(position), id(id), epsilon(eps) , groupThreshold(gt)
    {
        largestGroupId=-1;
        largestGroupEntities=-1;
        athletesPassed=1;
    }

    ~mat()
    {
        // here we should delete the components one by one.
    }


    double getPosition() const {return position;}

    void setPosition(double position) {mat::position = position;}

    int getId() const {return id;}

    void setId(int id) { mat::id = id;}

    int getLargestGroupEntities() const {
        return largestGroupEntities;
    }

    void setLargestGroupEntities(int largestGroupEntities) {
        mat::largestGroupEntities = largestGroupEntities;
    }

    int getLargestGroupId() const {
        return largestGroupId;
    }

    void setLargestGroupId(int largestGroupId) {
        mat::largestGroupId = largestGroupId;
    }

    void addEvent(event *e);

    std::vector<component* >* getcomponents() {return &components; }
    std::vector<int>* getGroups()  {return &groups;}

    bool inCurrentComponent(event e);

    int numElementsInComponent(int i){return components[i]->numberOfGroupedEvents();}

    RunningTime lastRunningTime();

};


#endif //ONEDTRAJECTORIES_MAT_H
