#include <iostream>
#include "threeDTimePoint.h"
#include "oneDTimePoint.h"
#include "athlete.h"
#include "groupComputations.h"
#include <fstream>
#include <string>
#include "component.h"
#include "inputData.h"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Going to compute things with this file  "<< argv[1]<<" and this number of parameters "<<argc<< endl;

    srand(0);
    vector<athlete*> runners;
    groupComputations comp;

    // type of data
    bool realData=true;

    // define group threshold, so far static
    int groupThreshold = (argc > 2 ? atoi(argv[2]) : 7);
    int epsilon = (argc > 3 ? atoi(argv[3]) : 2);
    double inclusionT = (argc > 4 ? atof(argv[4]) : 0.7);
    //  double jackardT = (argc > 5 ? atof(argv[4]) : 0.6); //jackard coefficient no longer used

    // upsample variables
    double upsampleStep = 1000;
    //bool upsample=true;
    bool upsample=false;

    comp.setGroupThreshold(groupThreshold);
    comp.setEpsilon(epsilon); // should probably be 0 or 1
    //comp.setJackardThreshold(jackardT);
    comp.setInclusionThreshold(inclusionT);

    cout<<"Group Threshold chosen to be "<<groupThreshold<<endl;
    cout<<"Epsilon chosen to be "<<epsilon<<endl;
    cout<<"Inclusion Threshold "<<inclusionT<<endl;
    cout<<"num parameters "<<argc<<endl;

    // now do input
    inputData inp;

    // Real data input
    if(realData) inp.readBostonMarathon(&runners, argv[1],&comp);
    else {
        // Synthtetic input
        int numTimePoints =  atoi(argv[5]);
        int numGroups =  atoi(argv[6]);

        int regularGuys = 25;

        for (int j=0;j<numGroups;j++) {

            // Functions to add new runners
            //vector<int> splitNorm = {2}; // remember points go from 0 to numTimePoints-1

            vector<int> bakuhatsuNorm=vector<int>();
            vector<int> splitNorm=vector<int>();
            vector<int> shrinkNorm=vector<int>();
            vector<int> disbNorm=vector<int>();

            int x;
            for(int k=0;k<numTimePoints;k++)
            {
                x=rand()%atoi(argv[7]);
                switch(x)
                {
                    case 0:
                        bakuhatsuNorm.push_back(k);
                    case 1:
                        splitNorm.push_back(k);
                        break;
                    case 2:
                        shrinkNorm.push_back(k);
                        break;
                    case 3:
                        disbNorm.push_back(k);
                        break;
                }

            }

            int pace=200+10*x;
            inp.addRegularRunningGuys(&runners, &comp, regularGuys, numTimePoints, epsilon, pace, true, bakuhatsuNorm,
                                      true, splitNorm, true, shrinkNorm, true, disbNorm);

        }

    }

    //sort events
    comp.sortEvents();

    if(upsample==true) {
        cout << "number of runners " << runners.size() << endl;
        cout << "number of events " << comp.numberOfEvents() << endl;

        // Function to upsample athletes
        std::list<event> newEventList = std::list<event>();
        int count = 0;
        for (groupComputations::iterator it = comp.begin(); it != comp.end(); ++it) {
            std::list<event> auxList = comp.upsampleEvent(it, upsampleStep);
            newEventList.insert(newEventList.end(), auxList.begin(), auxList.end());
            if ((count++) % 10000 == 0)cout << "upsampling event " << count << endl;
        }
        for (std::list<event>::iterator it = newEventList.begin(); it != newEventList.end(); it++)
            comp.addEvent(*it);//add all generated event to the list of events


        cout << "number of runners after upsampling " << runners.size() << endl;
        cout << "number of events after upsampling  " << comp.numberOfEvents() << endl;

        //sort events
        comp.sortEvents();
    }
    //   cout<<"EVENTS: "<<comp<<endl;

    // now process events
    // for every event
    int comptador=0;

    for (groupComputations::iterator it = comp.begin(); it != comp.end(); ++it) {
        // access element as *it
     //cout<<"Processing event where athlete called "<<(*it).getWho()->getName()<<" is at "<< (*it).getPointAndTime()<<endl;
      try {
          comp.processEvent(it);
         // cout<<"Processed event where athlete called "<<(*it).getWho()->getName()<<" is at "<< (*it).getPointAndTime()<<endl;

      }
      catch (char const* c)
      {
          cout<<"Exception caught at highest level "<<c<<endl;
          exit(-1);
      }

        comptador++;
       // if(comptador % 10000 == 0) std::cout<<" processing event "<<comptador<<" where athlete called "<<it->getWho()->getName()<<" is at "<< (*it).getPointAndTime()<<endl;
    }

    //Add a "broom athlete to every mat so all mats are closed
//cout<<"CLOSING MATS"<<endl;
    comp.closeMats();

  //  std::cout<<std::endl<<std::endl<<std::endl<<" COMPUTING GROUP RELATIONS "<<std::endl;
    // compute group relations
    comp.computeGroupRelations();

   // std::cout << std::endl << std::endl << std::endl << " COMPUTING Longest surviving group overall" << std::endl;
    std::list<component *> *longestSurvivingGroups = comp.longestSurvivingGroups();
    if(longestSurvivingGroups->size()!=0) std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest surviving group overall found *" <<longestSurvivingGroups->size()<<"# that survive for this many steps +"<<(*longestSurvivingGroups->begin())->maximumSurvivedSteps()<<"?"<< std::endl;
   // if(longestSurvivingGroups->size()!=0) std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest surviving group , The first is id "<<(*longestSurvivingGroups->begin())->getId()<<" followed by "<<(*longestSurvivingGroups->begin())->listSuccessorSurvivors()<< " with this many athletes " <<(*longestSurvivingGroups->begin())->numberOfGroupedEvents() << std::endl;
    //else std::cout<<"THERE ARE NO SURVIVORS "<<std::endl;

   // std::cout << std::endl << std::endl << std::endl << " COMPUTING Longest included group forward overall" << std::endl;
    std::list<component *> *longestIncludedGroupsForward = comp.longestRelatedGroupForward();
    if(longestIncludedGroupsForward->size()!=0)std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest included group forward overall found *" <<longestIncludedGroupsForward->size()<<"# that survive for this many steps +"<<(*longestIncludedGroupsForward->begin())->maximumIncludedStepsForward()<<"?"<< std::endl;
    //if(longestIncludedGroupsForward->size()!=0)std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest included group forward , The first is id "<<(*longestIncludedGroupsForward->begin())->getId()<<" "<<(*longestIncludedGroupsForward->begin())->listSuccessorRelated()<< " with this many athletes " <<(*longestIncludedGroupsForward->begin())->numberOfGroupedEvents() << std::endl;
    //else std::cout<<"THERE ARE NO Included groups forwards "<<std::endl;

   // std::cout << std::endl << std::endl << std::endl << " COMPUTING Longest included group backward overall" << std::endl;
    std::list<component *> *longestIncludedGroupsBackward = comp.longestRelatedGroupBackward();
    if(longestIncludedGroupsBackward->size()!=0) std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest included group backward overall found *" <<longestIncludedGroupsBackward->size()<<"# that survive for this many steps +"<<(*longestIncludedGroupsBackward->begin())->maximumIncludedStepsBackward()<<"?" << std::endl;
    //if(longestIncludedGroupsBackward->size()!=0) std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest included group backward , The first is id "<<(*longestIncludedGroupsBackward->begin())->getId()<<" "<<(*longestIncludedGroupsBackward->begin())->listPredecessorRelated()<< " with this many athletes " <<(*longestIncludedGroupsBackward->begin())->numberOfGroupedEvents() << std::endl;
   // else std::cout<<"THERE ARE NO Included groups backwards "<<std::endl;

    std::cout << std::endl << std::endl << std::endl << " STARTING THE COMPUTATION OF NUMBERS" << std::endl;
    std::cout << "SURVIVES  *"<<comp.countSurvives()<<"#"<<std::endl;
    std::cout << "APPEARS   *"<<comp.countAppears()<<"#"<< std::endl;
    std::cout << "DISAPPEARS*"<<comp.countDissappears()<<"#"<< std::endl;
    std::cout << "EXPANDS   *"<<comp.countExpands()<<"#"<< std::endl;
    std::cout << "SHRINKS   *"<<comp.countShrinks()<<"#"<< std::endl;
    std::cout << "MERGES    *"<<comp.countMerges()<<"#"<< std::endl;
    std::cout << "SPLITS    *"<<comp.countSplits()<<"#"<< std::endl;
    std::cout << "DISBANDS  *"<<comp.countDisbands()<<"#"<< std::endl;
    std::cout << "COHERES   *"<<comp.countCoheres()<<"#"<< std::endl;


    exit(0);



  /*  for(int j=0;j<comp.numberOfMats();j++) {
        std::cout << std::endl << std::endl << std::endl << " COMPUTING Longest surviving group " << j<< std::endl;
        std::list<component *> *longestSurvivingGroups = comp.longestSurvivingGroups(j);
        std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest surviving group , found " <<longestSurvivingGroups->size() <<" that survive for this many steps "<<(*longestSurvivingGroups->begin())->maximumSurvivedSteps() <<std::endl;
       // std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest surviving group , The first is " <<*(*longestSurvivingGroups->begin()) << std::endl;
    }*/



    std::cout<<std::endl<<std::endl<<std::endl<<" CHECKING PACES    "<<std::endl;


    // Check paces
    for (vector<athlete*>::iterator it = runners.begin(); it != runners.end(); ++it)
    {
        (*it)->computeAllPaceIntervals();
       // cout<<"Athlete called "<<(*it)->getName()<<" has had his pace computed "<<*(*it)<<endl;
    }

    //REPORT CHEATERS
    int numCheaters=0;
    for (vector<athlete*>::iterator it = runners.begin(); it != runners.end(); ++it)
    {
        if((*it)->isCheater() )
        {
            cout<<"Athlete called "<<(*it)->getName()<<" is a cheater just check "<<*(*it)<<endl;
            numCheaters++;
        }

    }
    cout<<"Found "<<numCheaters<<" cheaters"<<endl;

    return 0;
}