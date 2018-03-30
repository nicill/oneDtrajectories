//
// Created by yago on 16/04/20.
//

#include "groupComputations.h"
#include <exception>
#include <list>
#include <algorithm>

int groupComputations::findMat(double m)
{
    bool found=false;
    unsigned i=0;
    int size=mats.size();

   // std::cout<<"finding mat "<<m<<std::endl;

    if(mats.size()==0)
    {
        // add new mat and make a note on the group relations
        mats.push_back(mat(m,0,epsilon,groupThreshold,inclusionThreshold));
        relationsBackward.push_back(std::list< groupRelations* >());
        relationsForward.push_back(std::list< groupRelations* >());
        return 0;
    }

    while (i < size && !found) {
        if (mats[i].getPosition() == m) {
            found = true;
        }
        else { i++; }
    }

   // std::cout<<"found mat? "<<found<<std::endl;

    if(found)return i;
    else
    {
       // std::cout<<"MAT "<<m<<" NOT FOUND, size of mats "<<mats.size()<<" and i "<<i<<std::endl;

        if(m>mats[i-1].getPosition())// here epsilon and threshold could be modified depending on the mat
        {
            // add new mat and make a note on the group relations
            mats.push_back(mat(m,i,epsilon,groupThreshold,inclusionThreshold));
            relationsBackward.push_back(std::list< groupRelations* >());
            relationsForward.push_back(std::list< groupRelations* >());
            return i;
        }
        else
        {
            std::cout<<"FUNKY BUSINESS WITH WRONG MAT placement "<<std::endl;
            std::exception e;
            throw e;
        }
    }
}

void groupComputations::processEvent(groupComputations::iterator it)
{
    int matNumber;

    // First check that the athlete is not a known cheater
//    if(!(*it).getWho()->isCheater())
//    {
        // first, find the mat the event belong to, if not present but feasible, add, if not, throw exception
        try {
            matNumber=findMat((*it).getPointAndTime().getPos());
            // std::cout<<"groupComputations::processEvent, mat found "<<matNumber<<std::endl;
        }
        catch (std::exception &e)
        {
            std::cout<<"Exception caught at groupComputations::ProcessEvent (comming from find mat), funky business, measurement from misplaced mat "<<(*it).getWho()->getName()<<" pos "<<(*it).getPointAndTime().getPos()<<std::endl;
            throw e;
        }

        // then, add runner to mat (check if it needs to start a new component or what, in any case, keep info updated.
        // probably should start here turning this into a pointer
        mats[matNumber].addEvent(&(*it));


//    }
//    else{ std::cout<<(*it).getWho()->getName()<<" is a cheater so I am not adding him to the new mat"<<std::endl; }


    /*std::cout<<"waiting!!!! "<<std::endl;
    char c;
    std::cin>>c;*/
}

std::list<event> groupComputations::upsampleEvent(groupComputations::iterator it, double upsampleStep)
{
    std::list<event> retValue=std::list<event>();
    // get the athlete the event belongs to
    double prevPos = (*it).getWho()->getPreviousPosition((*it).getPointAndTime().getPos());
    RunningTime rt= (*it).getWho()->getPreviousTime((*it).getPointAndTime().getPos());

    // Now create events between
    double currentPos=prevPos;
   //std::cout<<"Athlete "<<(*it).getWho()->getName()<<" with current and previous position "<<(*it).getPointAndTime().getPos()<<" "<<prevPos<<std::endl;
    // compute time difference
  //  std::cout<<"Athlete "<<(*it).getWho()->getName()<<" with current and previous times "<<(*it).getPointAndTime().getTime()<<" "<<rt<<std::endl;
    int timeDiff=(*it).getPointAndTime().getTime()-rt;
  //  std::cout<<"Athlete "<<(*it).getWho()->getName()<<" time difference "<<timeDiff<<" and step "<<upsampleStep*(timeDiff)/((*it).getPointAndTime().getPos()-prevPos)<<std::endl;

    int timeStep=upsampleStep*(timeDiff)/((*it).getPointAndTime().getPos()-prevPos);
    int currentTime=rt.toSeconds();

    while(currentPos<(*it).getPointAndTime().getPos())
    {
        //create a new time point and add event
        currentPos=currentPos+upsampleStep;
        currentTime=currentTime+timeStep;
       // std::cout<<"Adding new step at position "<<currentPos<<" for athlete named "<<(*it).getWho()->getName()<<" with time "<<RunningTime(currentTime)<<std::endl;

        // get
        //oneDTimePoint(5000, stringToRunningTime(s5k));
        retValue.push_back(event((*it).getWho(),oneDTimePoint(currentPos,RunningTime(currentTime))));
    }
    return retValue;
}


void groupComputations::computeGroupRelations() // this function should take every group and
{
    int matIterator=0;
    std::vector<component*>* thisMatComponents=NULL;
    std::vector<int>* thisMatGroupList=NULL;
    std::vector<component*>* previousMatComponents = nullptr;
    std::vector<component*>* nextMatComponents= nullptr;

    while(matIterator<mats.size())
    {
        bool lookForward=(matIterator<mats.size()-1);
        if(lookForward) nextMatComponents=mats[matIterator+1].getcomponents();
        bool lookBack=(matIterator>0);
        if(lookBack) previousMatComponents=mats[matIterator-1].getcomponents();

        // for the mats from the second, get their components and also the group list group list used to know which ones are groups, component list used to access them.
        thisMatComponents=mats[matIterator].getcomponents();
        thisMatGroupList=mats[matIterator].getGroups();

       // std::cout<<"*********************at Mat "<<matIterator<<" computing groups for mat "<<matIterator<<" with position "<<mats[matIterator].getPosition()<<" this one has this many groups: "<<thisMatGroupList->size()<<std::endl;
        std::cout<<"*********************at Mat "<<matIterator<<" The biggest group here has id "<<mats[matIterator].getLargestGroupId()<<" and has this many entities "<<mats[matIterator].getLargestGroupEntities()<<std::endl;

        for(int i=0;i<thisMatGroupList->size();i++)            // for every group
        {
            // first, create the entry for the relations in this mat
            component *c = (*thisMatComponents)[(*thisMatGroupList)[i]];
            if (lookBack) relationsBackward[matIterator].push_back(new groupRelations(&mats[matIterator-1], c));
            if (lookForward) relationsForward[matIterator].push_back(new groupRelations(&mats[matIterator + 1], c));


            // Create backward relations
            for (component::iterator it = c->begin(); it != c->end(); ++it) {

                // process the event* here, check the previous group of the athlete
               // std::cout<<"athlete!!! "<<(*it)->getWho()->getId()<<std::endl;

                if (lookBack) {
                    int athletePreviousGroup = (*it)->getWho()->groupAtIthMat(matIterator - 1);
                    if (athletePreviousGroup != -1) {
                 //       std::cout<<"athlete "<<*(*it)->getWho()<<" has previous group "<<athletePreviousGroup<<std::endl;
                        relationsBackward[matIterator].back()->addSharedEntity(athletePreviousGroup);
                    }
                    // The relations forward of the groups in the previous mat have already been taken care of
                }

                // now take care of the forward relations of the groups in this mat
                if (lookForward) {
                    int athleteNextGroup = (*it)->getWho()->groupAtIthMat(matIterator + 1);

                    if (athleteNextGroup != -1) {
                       // std::cout<<"athlete "<<(*it)->getWho()->getId()<<" has next group "<<athleteNextGroup<<std::endl;
                        relationsForward[matIterator].back()->addSharedEntity(athleteNextGroup);
                    }

                }

            }

            // compute Inclusion Coefficients for Backward relations and update all the realational pointers of this group
            if (lookBack)
            {

                //std::cout<<"Processed group "<<c->getId()<<" with "<<c->numberOfGroupedEvents()<<" athletes at Mat "<<matIterator<<" found "<<((relationsBackward[matIterator]).back())->getTouchedGroups()->size()<<" groups touched backward"<<std::endl;
                for (std::list<int>::iterator it3 = ((relationsBackward[matIterator]).back())->getTouchedGroups()->begin();
                     it3 != ((relationsBackward[matIterator]).back())->getTouchedGroups()->end(); ++it3) {
                    std::vector<int> *aux = ((relationsBackward[matIterator]).back())->getSharedEntitiesVector();
                   // std::cout<<" group "<<*it3<<" was touched "<<(*aux)[*it3]<<" times "<<std::endl;

                    // first update the number of shared entities
                    double intersection = (*aux)[*it3];
                    int sharedBefore=c->getNumEntitiesSharedBefore();
                    sharedBefore=sharedBefore+intersection;
                    c->setNumEntitiesSharedBefore(sharedBefore);

                    // compute Inclusion coefficients
                    double elementsA = c->numberOfGroupedEvents();
                    double elementsB = mats[matIterator - 1].numElementsInComponent(*it3);
                    std::vector<double> *inclusionsBackward = ((relationsBackward[matIterator]).back())->getInclusionCoefficientsVector();
                    double incCoefBA = intersection/(elementsB);// coeficient of inclusion I(B,A), previous group on the current one
                    double incCoefAB = intersection/(elementsA);// coeficient of inclusion I(A,B), current group on the previous
                    if(incCoefAB>1||incCoefBA>1)
                    {
                        std::cout<<"1FUCKCUFKUFKSUFK BIG COEFFS "<<incCoefBA<< " "<<intersection<<"/"<<elementsB<<std::endl;
                        exit(-1);
                    }


                  //  if((*aux)[*it3]>500)  std::cout << " backward group " << c->getId() << " in mat  " << matIterator << " shares with group " << *it3 << ", " << (*aux)[*it3] << " entitites. Coefs (ab,ba) "<<incCoefAB<<" "<<incCoefBA <<" intersection,elA,elB "<<intersection<<" "<<elementsA<<" "<<elementsB<< std::endl;

                    // store in the group relation
                    (*inclusionsBackward)[*it3] = incCoefBA;
                    // If over the threshold, store in the list of the group
                    if (incCoefBA > inclusionThreshold)c->addPreviousRelatedGroup((*previousMatComponents)[*it3]);
                    // check also if this is the only group the current group is included in


                    if (incCoefAB > inclusionThreshold) {
                        if (c->isIncludedBackward()) { throw ("groupComputations::computeGroupRelations FUCKUP, included in more than one previous group"); }
                        c->setIncludedBefore((*previousMatComponents)[*it3]);
                    }
                    // and check if the relation is double
                    if ((incCoefBA > inclusionThreshold) && (incCoefAB > inclusionThreshold))
                        c->setDoubleBefore((*previousMatComponents)[*it3]);
                }
            }

             // compute Inclusion Coefficients for Forward relations and update all the relational pointers of this group
            if(lookForward) {
                for (std::list<int>::iterator it3 = ((relationsForward[matIterator]).back())->getTouchedGroups()->begin();
                     it3 != ((relationsForward[matIterator]).back())->getTouchedGroups()->end(); ++it3) {
                    std::vector<int> *aux = ((relationsForward[matIterator]).back())->getSharedEntitiesVector();

                    // first update the number of shared entities
                    double intersection = (*aux)[*it3];
                    int sharedAfter=c->getNumEntitiesSharedAfter();
                    sharedAfter=sharedAfter+intersection;
                    c->setNumEntitiesSharedAfter(sharedAfter);


                    // compute Inclusion coefficients
                    double elementsA = c->numberOfGroupedEvents();
                    double elementsB = mats[matIterator + 1].numElementsInComponent(*it3);
                    std::vector<double> *inclusionsForward = ((relationsForward[matIterator]).back())->getInclusionCoefficientsVector();
                    double incCoefBA = intersection/(elementsB);// coeficient of inclusion I(B,A), next group on the current one
                    double incCoefAB = intersection/(elementsA);// coeficient of inclusion I(A,B), current group on the next
                    if(incCoefAB>1||incCoefBA>1)
                    {
                        std::cout<<"FUCKCUFKUFKSUFK BIG COEFFS "<<incCoefAB<<" "<<incCoefBA<<std::endl;
                        exit(-1);
                    }

                 // if((*aux)[*it3]>500) std::cout << " forward group " << c->getId() << " in mat  " << matIterator << " shares with group " << *it3 << ", " << (*aux)[*it3] << " entitites. Coefs (ab,ba) "<<incCoefAB<<" "<<incCoefBA<<" intersection,elA,elB "<<intersection<<" "<<elementsA<<" "<<elementsB << std::endl;

                    // store in the group relation
                    (*inclusionsForward)[*it3] = incCoefBA;
                    // If over the threshold, store in the list of the group
                    if (incCoefBA > inclusionThreshold)c->addSubsequentRelatedGroup((*nextMatComponents)[*it3]);
                    // check also if this is the only group the current group is included in
                    if (incCoefAB > inclusionThreshold) {
                        if (c->isIncludedForward())throw ("groupComputations::computeGroupRelations FUCKUP, included in more than one following group");
                        c->setIncludedAfter((*nextMatComponents)[*it3]);
                    }
                    // and check if the relation is double
                    if ((incCoefBA > inclusionThreshold) && (incCoefAB > inclusionThreshold))c->setDoubleAfter((*nextMatComponents)[*it3]);

                }
            }

        }
        //std::cout<<"******************* FINISHING Mat: "<<matIterator<<" with position "<<mats[matIterator].getPosition()<<" the bigger group here (id "<<mats[matIterator].getLargestGroupId()<<") had : "<<mats[matIterator].getLargestGroupEntities()<<" elements"<<std::endl;
        matIterator++;
    }
}

void groupComputations::computeIndividualGroupBehaviour() {

/*    int matIterator=0; //start at the first mat
    std::vector<component*>* thisMatComponents=NULL;
    std::vector<int>* thisMatGroupList=NULL;
   // std::vector<component*>* previousMatComponents=mats[0].getcomponents();
   // std::vector<component*>* nextMatComponents= nullptr;

    while(matIterator<mats.size())
    {
        bool lookForward=(matIterator<mats.size()-1);
        if(lookForward) nextMatComponents=mats[matIterator+1].getcomponents();

        // for the mats from the second, get their components and also the group list group list used to know which ones are groups, component list used to access them.
        thisMatComponents=mats[matIterator].getcomponents();
        thisMatGroupList=mats[matIterator].getGroups();

        std::cout<<"*********************at Mat "<<matIterator<<" computing groups for mat "<<matIterator<<" with position "<<mats[matIterator].getPosition()<<" this one has this many groups: "<<thisMatGroupList->size()<<std::endl;
        std::cout<<"*********************at Mat "<<matIterator<<" The biggest group here has id "<<mats[matIterator].getLargestGroupId()<<" and has this many entities "<<mats[matIterator].getLargestGroupEntities()<<std::endl;

        for(int i=0;i<thisMatGroupList->size();i++)            // for every group
        {*/

}

std::list<component*> *groupComputations::longestSurvivingGroups() {
    std::list<component *> *returnList = new std::list<component *>();
    int matNumber=0;
    int largesttNumberOfSteps = 0;

    while(matNumber<mats.size()) {
        //std::cout<<"starting longest group computations mat "<<matNumber<<std::endl;
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest surviving time and store at the same time
        std::vector<int>::iterator it;
        int count=1;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) {
           // std::cout<<"starting survival computation for group id "<<(*it)<<" at mat "<<matNumber<<" groups to be looked at "<<groupIndexes->size()<<std::endl;
            // check that this group starts at the given mat
            if(!(*startComponents)[(*it)]->hasSurvived()) {
                //std::cout<<"group "<<(*it)<<" at mat "<<matNumber<<" has not survived "<<count++<<"/"<<groupIndexes->size()<<std::endl;
                int aux = (*startComponents)[(*it)]->maximumSurvivedSteps();
                if (aux != 0 && aux == largesttNumberOfSteps)returnList->push_back((*startComponents)[(*it)]);
                else if (aux > largesttNumberOfSteps) {
                    // found a largest possible step lenght, pop all stored untill now
                    returnList->clear();
                    // store new information
                    returnList->push_back((*startComponents)[(*it)]);
                    largesttNumberOfSteps = aux;
                  //  std::cout << "1UPDATING LONGEST GROUP LIST, now maximum surviving step number is " << aux <<std::endl;
                }
            }
        }
        // check that getting a longer surviving group is still possible
        if(largesttNumberOfSteps>mats.size()-matNumber) matNumber=mats.size();
        else matNumber++;
    }
    return returnList;
}

std::list<component*> *groupComputations::longestSurvivingGroups(int matNumber) {

    std::list<component*>* returnList = new std::list<component*>();

    std::vector<component*>* startComponents = mats[matNumber].getcomponents();
    std::vector<int>* groupIndexes = mats[matNumber].getGroups();

  //  std::cout<<"survival, mat position "<<mats[matNumber].getPosition()<<std::endl;

    // compute longest surviving time and store at the same time
    std::vector<int>::iterator it;
    int largesttNumberOfSteps=0;
    for(it=groupIndexes->begin();it!=groupIndexes->end();it++)
    {
    //    std::cout<<"starting survival computation for group id "<<(*it)<<" at mat "<<matNumber<<" groups to be looked at "<<groupIndexes->size()<<std::endl;
        int aux = (*startComponents)[(*it)]->maximumSurvivedSteps();
        if(aux!=0 && aux==largesttNumberOfSteps)returnList->push_back((*startComponents)[(*it)]);
        else if ( aux>largesttNumberOfSteps )
        {
            // found a largest possible step lenght, pop all stored untill now
            returnList->clear();
            // store new information
            returnList->push_back((*startComponents)[(*it)]);
            largesttNumberOfSteps=aux;
            //std::cout<<"2UPDATING LONGEST GROUP LIST, now maximum surviving step number is "<<aux<<std::endl;
        }
    }
    return returnList;
}

void groupComputations::closeMats() {

    athlete *a=new athlete() ;
    std::string s="broomCarMat";
    a->setName(s);

    for(int matNumber=0;matNumber<mats.size();matNumber++) {
     //   std::cout<<"groupComputations::closeMats(), closing mat "<<matNumber<<std::endl;
        // get last running time
        RunningTime rt=mats[matNumber].lastRunningTime();
        int seconds=rt.toSeconds()+((int)epsilon*3);

     //  std::cout<<"groupComputations::closeMats(), adding event "<<event(a,oneDTimePoint(mats[matNumber].getPosition(),RunningTime(seconds)))<<std::endl;
        mats[matNumber].addEvent(new event(a,oneDTimePoint(mats[matNumber].getPosition(),RunningTime(seconds))));

    }
}

std::list<component *> *groupComputations::longestRelatedGroupForward() {

    std::list<component *> *returnList = new std::list<component *>();
    int matNumber=0;
    int largesttNumberOfSteps = 0;
    while(matNumber<mats.size()) {
        //std::cout<<"starting longest related group forward computations mat "<<matNumber<<std::endl;
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest related time and store at the same time
        std::vector<int>::iterator it;
        int count=1;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) {
           // std::cout<<"starting survival computation for group id "<<(*it)<<" at mat "<<matNumber<<" groups to be looked at "<<groupIndexes->size()<<std::endl;
            // check that this group starts at the given mat
            if(!(*startComponents)[(*it)]->containsSomeoneBefore()) {
              //  std::cout<<"group "<<(*it)<<" at mat "<<matNumber<<" has not survived "<<count++<<"/"<<groupIndexes->size()<<std::endl;
                int aux = (*startComponents)[(*it)]->maximumIncludedStepsForward();
             //   std::cout<<"group "<<(*it)<<" at mat "<<matNumber<<" goes forwars "<<aux<<" steps"<<std::endl;

                if (aux != 0 && aux == largesttNumberOfSteps)returnList->push_back((*startComponents)[(*it)]);
                else if (aux > largesttNumberOfSteps) {
                    // found a largest possible step lenght, pop all stored untill now
                    returnList->clear();
                    // store new information
                    returnList->push_back((*startComponents)[(*it)]);
                    largesttNumberOfSteps = aux;
                   // std::cout << "3UPDATING LONGEST GROUP LIST, now maximum surviving step number is " << aux <<std::endl;
                }
            }
            //else{         std::cout<<" group id "<<(*it)<<" at mat "<<matNumber<<" contains someone before "<<std::endl;}


        }
        // check that getting a longer surviving group is still possible
        if(largesttNumberOfSteps>mats.size()-matNumber) matNumber=mats.size();
        else matNumber++;
    }
    return returnList;
}

std::list<component *> *groupComputations::longestRelatedGroupBackward() {

    std::list<component *> *returnList = new std::list<component *>();
    int matNumber=mats.size()-1;
    int largesttNumberOfSteps = 0;
    while(matNumber>=0) {
        //std::cout<<"starting longest related group Backwards computations mat "<<matNumber<<std::endl;
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest related time and store at the same time
        std::vector<int>::iterator it;
        int count=1;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) {
            // std::cout<<"starting survival computation for group id "<<(*it)<<" at mat "<<matNumber<<" groups to be looked at "<<groupIndexes->size()<<std::endl;
            // check that this group starts at the given mat
            if(!(*startComponents)[(*it)]->containsSomeoneAfter()) {
                //std::cout<<"group "<<(*it)<<" at mat "<<matNumber<<" has not survived "<<count++<<"/"<<groupIndexes->size()<<std::endl;
                int aux = (*startComponents)[(*it)]->maximumIncludedStepsBackward();
                if (aux != 0 && aux == largesttNumberOfSteps)returnList->push_back((*startComponents)[(*it)]);
                else if (aux > largesttNumberOfSteps) {
                    // found a largest possible step lenght, pop all stored untill now
                    returnList->clear();
                    // store new information
                    returnList->push_back((*startComponents)[(*it)]);
                    largesttNumberOfSteps = aux;
                  //  std::cout << "4UPDATING LONGEST GROUP LIST, now maximum surviving step number is " << aux << std::endl;
                }
            }
        }
        // check that getting a longer surviving group is still possible
       // if(largesttNumberOfSteps>matNumber+1) matNumber=mats.size();
        //else matNumber--;
        matNumber--;
    }
    return returnList;
}

int groupComputations::countSurvives() {
    int matNumber=0;
    int count=0;

    while(matNumber<mats.size()) {
       // std::cout<<"starting the count for the number of surviving groups in mat "<<matNumber<<std::endl;
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest surviving time and store at the same time
        std::vector<int>::iterator it;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) { if((*startComponents)[(*it)]->willSurvive()) {count++;} }
        matNumber++;
    }
    return count;
}

int groupComputations::countMerges() {
    int matNumber=0;
    int count=0;

    while(matNumber<mats.size()) {
        // std::cout<<"starting the count for the number of surviving groups in mat "<<matNumber<<std::endl;
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest surviving time and store at the same time
        std::vector<int>::iterator it;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) { if((*startComponents)[(*it)]->merges()) {
               // std::cout<<"gc::count merges updatting count"<<count<<std::endl;
                count++;} }
        matNumber++;
    }
    return count;
}

int groupComputations::countSplits() {
    int matNumber=0;
    int count=0;

    while(matNumber<mats.size()) {
        // std::cout<<"starting the count for the number of surviving groups in mat "<<matNumber<<std::endl;
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest surviving time and store at the same time
        std::vector<int>::iterator it;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) { if((*startComponents)[(*it)]->splits()) {count++;} }
        matNumber++;
    }
    return count;
}

int groupComputations::countExpands() {
    int matNumber=0;
    int count=0;

    while(matNumber<mats.size()) {
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest surviving time and store at the same time
        std::vector<int>::iterator it;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) { if((*startComponents)[(*it)]->expands()) {count++;} }
        matNumber++;
    }
    return count;
}

int groupComputations::countShrinks() {
    int matNumber=0;
    int count=0;

    while(matNumber<mats.size()) {
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest surviving time and store at the same time
        std::vector<int>::iterator it;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) { if((*startComponents)[(*it)]->shrinks()) {count++;} }
        matNumber++;
    }
    return count;}

int groupComputations::countCoheres() {
    int matNumber=0;
    int count=0;

    while(matNumber<mats.size()) {
        // std::cout<<"starting the count for the number of surviving groups in mat "<<matNumber<<std::endl;
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest surviving time and store at the same time
        std::vector<int>::iterator it;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) { if((*startComponents)[(*it)]->coheres()) {count++; }
        }
        matNumber++;
    }
    return count;}

int groupComputations::countDisbands() {
    int matNumber=0;
    int count=0;

    while(matNumber<mats.size()) {
        // std::cout<<"starting the count for the number of surviving groups in mat "<<matNumber<<std::endl;
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest surviving time and store at the same time
        std::vector<int>::iterator it;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) { if((*startComponents)[(*it)]->disbands()) {count++;} }
        matNumber++;
    }
    return count;}

int groupComputations::countAppears() {
    int matNumber=0;
    int count=0;

    while(matNumber<mats.size()) {
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest surviving time and store at the same time
        std::vector<int>::iterator it;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) { if((*startComponents)[(*it)]->appears()){count++;} }
        matNumber++;
    }
    return count;
}

int groupComputations::countDissappears() {
    int matNumber=0;
    int count=0;

    while(matNumber<mats.size()) {
        std::vector<component *> *startComponents = mats[matNumber].getcomponents();
        std::vector<int> *groupIndexes = mats[matNumber].getGroups();

        // compute longest surviving time and store at the same time
        std::vector<int>::iterator it;

        for (it = groupIndexes->begin(); it != groupIndexes->end(); it++) { if((*startComponents)[(*it)]->dissappears()) {count++;} }
        matNumber++;
    }
    return count;
}
