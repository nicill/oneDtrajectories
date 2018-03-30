//
// Created by yago on 16/04/21.
//

#include "mat.h"

void mat::addEvent(event *e) {
// change this to pointer to event, start by adding the position in the mat to the athlete position

    // check if the current athlete falls inside the current component or we have to create a new one.
    if(!inCurrentComponent(*e))
    {
 //       std::cout<<"NOT IN CURRENT COMPONENT "<<std::endl;
        components.push_back(new component(components.size(),epsilon,groupThreshold,position,inclusionT));
        if(components.size()>1)
        {
            component *c= components[components.size()-2];
            if(c->isThisAGroup())
            {
               //if(c.numberOfGroupedEvents()>100) std::cout<<"                                                                                                  abandoning a group with "<<c->numberOfGroupedEvents()<<" elements and id "<<c->getId()<<" at mat Position "<<position<<" with athletes classified from "<<c->getFirstClassificationIthMat(id)<<" to "<<c->getLastClassificationIthMat(id)<<std::endl;
               // Update largest group information
                if( largestGroupId==-1 || largestGroupEntities<c->numberOfGroupedEvents() )
                {
                    largestGroupId=c->getId();
                    largestGroupEntities=c->numberOfGroupedEvents();
                }
            }
            else
            {
               // go over all the athletes here and mark that they were not in a group
               for (component::iterator it = c->begin(); it != c->end(); ++it)
               {
                     (*it)->getWho()->setGroupAtIthMat(this->id,-1);

               }


                }
        }
    }
    else{
       // std::cout<<"CURRENT COMPONENT "<<std::endl;
    }

    // always add the athlete to the last component (which we may or may not have just created)
  //  std::cout<<" in mat "<<this->getId()<<" adding event to component number "<<components.size()-1<<std::endl;
    if(components[components.size()-1]->addEvent(e))groups.push_back(components.size()-1);


    athlete *a=e->getWho();

    // add the time point to the athlete
    a->addTimePoint(e->getPointAndTime());
    // add to the list of groups of the athlete the current one
    a->setNextGroup(components[components.size()-1]->getId());
    //update the groups that share entities with this one
    //in any case, we have another athlete passed, update his list
    a->setNextClassification(athletesPassed);
   // if((athletesPassed % 10000 == 0)||(athletesPassed==100))
   // {
    //    std::cout<<athletesPassed<<" guys passed at mat "<<position<<" the last guy was "<<*(e->getWho())<<" at this moment this mat has "<<components.size()<<" components and "<<groups.size()<<" groups "<<std::endl;
    //}
    athletesPassed++;



 //   std::cout<<"LEAVING MAT "<<std::endl;

}

bool mat::inCurrentComponent(event e) {
    if(components.size()==0)return false;
    else{
        component *c=components[components.size()-1];
        RunningTime lastTime=c->getLastTime();
        int elapsed=abs(lastTime.difference(e.getPointAndTime().getTime()) );
        return elapsed<=epsilon;
    }
}

RunningTime mat::lastRunningTime() {
    //get last athletes running time
    return components[components.size()-1]->getLastTime();
}
