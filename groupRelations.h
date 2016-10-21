//
// Created by yago on 16/05/26.
//

#ifndef ONEDTRAJECTORIES_GROUPRELATION_H
#define ONEDTRAJECTORIES_GROUPRELATION_H

#include <list>
#include "mat.h"
#include <algorithm>

// This class encapsulates the number of entities shared by two groups as well as the main relations of a group.
class groupRelations {

    mat* mat1; //previous mat, does not actually have to be the one just before

    component* group; //the group we are storing the behaviour about

    std::list<int> touchedGroups; // list of the identifiers of the groups touched by athletes in the current group
    std::vector<int> numberOfSharedEntities;
    std::vector<double> inclusionCoefficients;
  //  std::vector<double> jacardCoefficients; //do not need these any more

public:
    groupRelations()
    {
        std::cout<<"GroupRelation empty creator, you should really not be doing this "<<std::endl;
        exit(-1);
    }

    groupRelations(mat *mat1, component *group1) : mat1(mat1), group(group1)
    {
        std::cout<<"GroupRelation creator,  "<<mat1->getcomponents()->size()<<std::endl;
        touchedGroups=std::list<int>();
        std::cout<<"0CCCCCCCCCCCCCCCCCCC"<<std::endl;
        numberOfSharedEntities=std::vector<int>(mat1->getcomponents()->size());
        std::cout<<"1CCCCCCCCCCCCCCCCCCC "<<mat1->getcomponents()->size()<<std::endl;
        inclusionCoefficients=std::vector<double>(mat1->getcomponents()->size());
        std::cout<<"2CCCCCCCCCCCCCCCCCCC"<<std::endl;
        numberOfSharedEntities=std::vector<int>(mat1->getcomponents()->size());

        //     jacardCoefficients=std::vector<double>(mat1->getcomponents()->size());
        std::cout<<"Finishing GroupRelation creator,  "<<mat1->getcomponents()->size()<<std::endl;

    }

    mat *getMat() const {
        return mat1;
    }

    component *getGroup() const {
        return group;
    }

    std::list<int> *getTouchedGroups() {
        return &touchedGroups;
    }

    std::vector<int> * getSharedEntitiesVector() {
        return &numberOfSharedEntities;
    }

    std::vector<double> * getInclusionCoefficientsVector() {
        return &inclusionCoefficients;
    }

//    std::vector<double> * getJacardCoefficientsVector() {
//        return &jacardCoefficients;
//    }

    void addSharedEntity(int groupId)
    {
       // std::cout<<"adding shared entity"<<std::endl;
        bool found=false;
       std::list<int>::iterator it2;
       // this could be changed to have this list sorted
       it2 = std::find(touchedGroups.begin(), touchedGroups.end(), groupId);

       if (it2 == touchedGroups.end())
       {
         //  std::cout<<"enteringshared entity "<<groupId<<std::endl;

                //add to list of touched groups if it was not there already
                touchedGroups.push_back(groupId);
       }

        // always accumulate
       numberOfSharedEntities[groupId] = numberOfSharedEntities[groupId] + 1;
       std::cout<<"added shared entity siez: "<<touchedGroups.size()<< " just wrote "<<numberOfSharedEntities[groupId] + 1<<" at group number "<<groupId<<" size of vector "<<numberOfSharedEntities.size()<<std::endl;

    }

};


#endif //ONEDTRAJECTORIES_GROUPRELATION_H
