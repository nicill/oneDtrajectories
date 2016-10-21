#include <iostream>
#include "threeDTimePoint.h"
#include "oneDTimePoint.h"
#include "athlete.h"
#include "groupComputations.h"
#include <fstream>
#include <string>
#include "component.h"


using namespace std;

bool isInteger(const std::string & s)
{

   // cout<<"checking if "<<s<<" is numeric"<<endl;
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

    char * p ;
    strtol(s.c_str(), &p, 10) ;

    return (*p == 0) ;
}

RunningTime stringToRunningTime(string s)
{
   //cout<<"interpreting "<<s<<endl;
    std::size_t found = s.find(".");
    try {
        int S = 0.6 * stoi(s.substr(found+1, 2));
        int M = stoi(s.substr(0, found));
        if(isInteger(s.substr(0, found)) &&  isInteger( s.substr(found+1, 2 ) ) )
        {
            return RunningTime(M, S);
        }
        else
        {
            exception e=exception();
            throw e;
        }
    }
    catch (exception e)
    {
            //cout << " Exception caught when processing string " << s << endl;
            // just propagate exception
            throw e;
    }

}


void readBostonMarathon(vector<athlete*> *runners, char * fileName,groupComputations *gc)
{
    string line,s5k,s10k,s20k,sHalf,s25k,name,division,s30k,s35k,s40k,gender,age,sEnd;
    ifstream myfile (fileName);
    std::size_t pos5k,pos10k,pos20k,posHalf,pos25k,pos30k,pos35k,pos40k,posEND;
    std::size_t posName,posNameHalf,posDivision,posGender,posAge,posBib,posGenderDiv,posCTZ,posOverall,posPace,posState,posCountry,posCity;

    int athleteCounter=1;

    if (myfile.is_open())
    {
        getline (myfile,line);
        cout <<"First Line! "<< line << '\n';

        while ( getline (myfile,line) )
        {
            athlete *a=new athlete() ;
            a->setId(athleteCounter);

           // cout << line << '\n';

            pos10k = line.find(',');

            // get all final positions
          //  posNameHalf = line.find(',', pos10k + 1);
            posName = line.find(',', pos10k + 1);
            posDivision = line.find(',', posName + 1);
            pos25k = line.find(',', posDivision + 1);
            posGender = line.find(',', pos25k + 1);
            posAge = line.find(',', posGender + 1);
            posEND = line.find(',', posAge + 1);
            posBib = line.find(',', posEND + 1);
            posGenderDiv = line.find(',', posBib + 1);
            posCTZ = line.find(',', posGenderDiv + 1);
            pos35k = line.find(',', posCTZ + 1);
            posOverall = line.find(',', pos35k + 1);
            posPace = line.find(',', posOverall + 1);
            posState = line.find(',', posPace + 1);
            pos30k = line.find(',', posState + 1);
            pos5k = line.find(',', pos30k + 1);
            posHalf = line.find(',', pos5k + 1);
            pos20k = line.find(',', posHalf + 1);
            posCountry = line.find(',', pos20k + 1);
            posCity = line.find(',', posCountry + 1);
            pos40k = line.find(',', posCity + 1);

            // properly get interesting strings
            s5k = line.substr(pos30k + 1, pos5k - pos30k - 1);
            s10k = line.substr(0, pos10k);
            s20k = line.substr(posHalf + 1, pos20k - posHalf - 1);
            sHalf = line.substr(pos5k + 1, posHalf -pos5k - 1);
            s25k = line.substr(posDivision + 1, pos25k - posDivision - 1);
            s30k = line.substr(posState + 1, pos30k - posState - 1);
            s35k = line.substr(posCTZ + 1, pos35k - posCTZ - 1);
            s40k = line.substr(posCity + 1, pos40k - posCity - 1);
            sEnd = line.substr(posAge + 1, posEND - posAge - 1);

            name = line.substr(pos10k + 1, posName - pos10k - 1);
            division = line.substr(posName + 1, posDivision - posName - 1);
            gender = line.substr(pos25k + 1, posGender - pos25k - 1);
            //age = line.substr(posGender+1,posAge-posGender-1);

            //set all athlete parameters (except the id)
            a->setDivision(division);
            a->setGender(gender);
            a->setName(name);

            bool b5k,b10k,b20k,bHalf,b25k,b30k,b35k,b40k,bEND;
            b5k=b10k=b20k=bHalf=b25k=b30k=b35k=b40k=bEND=true;
            oneDTimePoint tp5,tp10,tp15,tp20,tpHalf,tp25,tp30,tp35,tp40,tpEND;

            // to begin with, the athletes do not know their results
            try
            {
                tp5=oneDTimePoint(5000, stringToRunningTime(s5k));
                //a.addTimePoint(tp5);
            }
            catch (exception e)
            {
                // cout<<" caught exception at number "<<athleteCounter<<" 5K athlete name "<<a->getName()<<endl;
                b5k=false;
            }

            try
            {
                tp10=oneDTimePoint(10000, stringToRunningTime(s10k));
                //a.addTimePoint(tp10);
            }
            catch (exception e)
            {
                // cout<<" caught exception at number "<<athleteCounter<<" 10K athlete name "<<a->getName()<<endl;
                b10k=false;
            }

            try
            {
                tp20=oneDTimePoint(20000, stringToRunningTime(s20k));
                //a.addTimePoint(tp20);
            }
            catch (exception e)
            {
                // cout<<" caught exception at number "<<athleteCounter<<" 20K athlete name "<<a->getName()<<endl;
                b20k=false;
            }

            try
            {
                tpHalf=oneDTimePoint(21097, stringToRunningTime(sHalf));
                // a.addTimePoint(tpHalf);
            }
            catch (exception e)
            {
                // cout<<" caught exception at number "<<athleteCounter<<" HALF athlete name "<<a->getName()<<endl;
                bHalf=false;
            }

            try
            {
                tp25=oneDTimePoint(25000, stringToRunningTime(s25k));
                //a.addTimePoint(tp25);
            }
            catch (exception e)
            {
                // cout<<" caught exception at number "<<athleteCounter<<" 25k athlete name "<<a->getName()<<endl;
                b25k=false;
            }

            try
            {
                tp30=oneDTimePoint(30000, stringToRunningTime(s30k));
                //a.addTimePoint(tp30);
            }
            catch (exception e)
            {
                // cout<<" caught exception at number "<<athleteCounter<<" 30K athlete name "<<a->getName()<<endl;
                b30k=false;
            }

            try
            {
                tp35=oneDTimePoint(35000, stringToRunningTime(s35k));
                //a.addTimePoint(tp35);
            }
            catch (exception e)
            {
                // cout<<" caught exception at number "<<athleteCounter<<" 35K athlete name "<<a->getName()<<endl;
                b35k=false;
            }

            try
            {
                tp40=oneDTimePoint(40000, stringToRunningTime(s40k));
                //a.addTimePoint(tp40);
            }
            catch (exception e)
            {
                // cout<<" caught exception at number "<<athleteCounter<<" 40K athlete name "<<a->getName()<<endl;
                b40k=false;
            }

            try
            {
                tpEND=oneDTimePoint(42195, stringToRunningTime(sEnd));
                //a.addTimePoint(tpEND);
            }
            catch (exception e)
            {
                // cout<<" caught exception at number "<<athleteCounter<<" END athlete name "<<a->getName()<<endl;
                bEND=false;
            }

            //a.checkConsistency();

            if(b5k&&b10k&&b20k&&bHalf&&b25k&&b30k&&b35k&&b40k&&bEND)
            {
                    event e5(a, tp5);
                    gc->addEvent(e5);
                    event e10(a, tp10);
                    gc->addEvent(e10);
                    event e20(a, tp20);
                    gc->addEvent(e20);
                    event eHalf(a, tpHalf);
                    gc->addEvent(eHalf);
                    event e25(a, tp25);
                    gc->addEvent(e25);
                    event e30(a, tp30);
                    gc->addEvent(e30);
                    event e35(a, tp35);
                    gc->addEvent(e35);
                    event e40(a, tp40);
                    gc->addEvent(e40);
                    event eEND(a, tpEND);
                    gc->addEvent(eEND);

                    runners->push_back(a);
            }

            //cout << "ATHLETE " << a << endl;
            athleteCounter++;

            // exit(0);

        }
        cout<<"Athletes looped "<<athleteCounter<<endl;
        myfile.close();
    }
    else cout << "Unable to open file "<<fileName;
}

void addSluggishJoes(vector<athlete*> *runners, groupComputations *gc,int numAthletes,int startAthleteNumber,int numTimePoints)
{
    int athleteCounter=1;

    while(athleteCounter<=numAthletes)
    {

        // cout<<"Looping Athlete "<<athleteCounter<<endl;

        athlete *a=new athlete() ;
        a->setId(startAthleteNumber+athleteCounter);

        // cout << line << '\n';

        //set all athlete parameters (except the id)
        a->setName("SluggishJoe"+std::to_string(athleteCounter));

        for(int i=0;i<numTimePoints;i++)
        {
            //cout<<"creating time point  "<<i<<endl;
            oneDTimePoint *tp;

            // these guys run slow at first and pick up speed
            if(i<numTimePoints/3) {
                cout << "case  0 " << endl;
                tp = new oneDTimePoint((i + 1) * 1000,
                                       RunningTime(5 * (i + 1), i + 1)); // athletes running at 5 min / km
            }
            else if(i<2*numTimePoints/3) {
                    cout << "case  1 " << endl;
                    tp = new oneDTimePoint((i + 1) * 1000,
                                           RunningTime(4 * (i + 1), i + 1)); // athletes running at 4 min / km
                }
            else
            {
                    cout<<"case  2 "<<endl;
                    tp = new oneDTimePoint((i+1) * 1000, RunningTime(floor(6*(i+1)),i+1)); // athletes running at 3 min / km

            }


            event e(a, *tp);
            gc->addEvent(e);

        }

        a->checkConsistency();

        runners->push_back(a);

        //cout << "ATHLETE " << a << endl;
        athleteCounter++;

        // exit(0);


        //  cout<<"Athletes looped "<<athleteCounter<<endl;
    }


}



void addRegularRunningGuys(vector<athlete*> *runners, groupComputations *gc,int numAthletes,int numTimePoints, int epsilon, bool exploding=false)
{
    int athleteCounter=1;

    while(athleteCounter<=numAthletes)
    {

       // cout<<"Looping Athlete "<<athleteCounter<<endl;

        athlete *a=new athlete() ;
        a->setId(athleteCounter);

        // cout << line << '\n';

        //set all athlete parameters (except the id)
        a->setName(std::to_string(athleteCounter));

        for(int i=0;i<numTimePoints;i++)
        {
            //cout<<"creating time point  "<<i<<endl;
            oneDTimePoint *tp;

            // in 2*steps run regularly
            if(exploding) {
                switch (i % 2) {
                    case 0:
                        switch (athleteCounter % 3) {
                            case 0:
                                //cout<<"case  0 "<<endl;
                                tp = new oneDTimePoint((i + 1) * 1000,
                                                       RunningTime(5 * (i + 1),
                                                                   i + 1)); // athletes running at 5 min / km
                                break; //optional
                            case 1:
                                //cout<<"case  1 "<<endl;
                                tp = new oneDTimePoint((i + 1) * 1000,
                                                       RunningTime(4 * (i + 1),
                                                                   i + 1)); // athletes running at 4 min / km
                                break; //optional
                            case 2:
                                //cout<<"case  2 "<<endl;
                                //tp = new oneDTimePoint((i+1) * 1000, RunningTime(floor(4.5*(i+1)),i+1)); // athletes running at 3 min / km
                                tp = new oneDTimePoint((i + 1) * 1000, RunningTime(floor(6 * (i + 1)),
                                                                                   i +
                                                                                   1)); // athletes running at 3 min / km
                                break; //optional
                            default : //Optional
                                cout << "case  default, nothing to do " << endl;
                        }
                        break;
                    case 1:
                        switch (athleteCounter % 3) {
                            case 0:
                                //cout<<"case  0 "<<endl;
                                tp = new oneDTimePoint((i + 1) * 1000,
                                                       RunningTime(5 * (i + 1), ((athleteCounter*epsilon)%60))); // athletes running at 5 min / km
                                break; //optional
                            case 1:
                                //cout<<"case  1 "<<endl;
                                tp = new oneDTimePoint((i + 1) * 1000,
                                                       RunningTime(4 * (i + 1),((athleteCounter*epsilon)%60) )); // athletes running at 4 min / km
                                break; //optional
                            case 2:
                                //cout<<"case  2 "<<endl;
                                //tp = new oneDTimePoint((i+1) * 1000, RunningTime(floor(4.5*(i+1)),i+1)); // athletes running at 3 min / km
                                tp = new oneDTimePoint((i + 1) * 1000, RunningTime(floor(6 * (i + 1)), ((athleteCounter*epsilon)%60) )); // athletes running at 3 min / km
                                break; //optional
                        }
                        break;


                }
            }
            else
            {
                switch (athleteCounter % 3) {
                    case 0:
                        //cout<<"case  0 "<<endl;
                        tp = new oneDTimePoint((i + 1) * 1000,
                                               RunningTime(5 * (i + 1), i + 1)); // athletes running at 5 min / km
                        break; //optional
                    case 1:
                        //cout<<"case  1 "<<endl;
                        tp = new oneDTimePoint((i + 1) * 1000,
                                               RunningTime(4 * (i + 1), i + 1)); // athletes running at 4 min / km
                        break; //optional
                    case 2:
                        //cout<<"case  2 "<<endl;
                        //tp = new oneDTimePoint((i+1) * 1000, RunningTime(floor(4.5*(i+1)),i+1)); // athletes running at 3 min / km
                        tp = new oneDTimePoint((i + 1) * 1000, RunningTime(floor(6 * (i + 1)),i + 1)); // athletes running at 3 min / km
                        break; //optional
                    default : //Optional
                        cout << "case  default, nothing to do " << endl;
                }
            }
            event e(a, *tp);
            gc->addEvent(e);

            cout<<"created event  "<<e<<endl;

            // made up time points spaced 1KM apart
            //first test, everyone at the same speed, 5 minutes one second per kilometer
        }

        a->checkConsistency();

        runners->push_back(a);

        //cout << "ATHLETE " << a << endl;
        athleteCounter++;

        // exit(0);


      //  cout<<"Athletes looped "<<athleteCounter<<endl;
    }



}



int main(int argc, char *argv[]) {
    cout << "Going to compute things with this file  "<< argv[1]<<" and this number of parameters "<<argc<< endl;

    vector<athlete*> runners;
    groupComputations comp;

    // define group threshold, so far static
    int groupThreshold = (argc > 2 ? atoi(argv[2]) : 7);
    int epsilon = (argc > 3 ? atoi(argv[3]) : 2);
    double inclusionT = (argc > 4 ? atof(argv[5]) : 0.7);
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
    cout<<"num parameters "<<argc<<endl;

    //readBostonMarathon(&runners, argv[1],&comp);

    int numTimePoints=10;
    int regularGuys=25;
    int slowJoes=10;
    int fastJacks=5;

   // Functions to add new runners
   addRegularRunningGuys(&runners,&comp,regularGuys,numTimePoints,epsilon,false);
   addSluggishJoes(&runners,&comp,slowJoes,regularGuys,numTimePoints);

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
          cout<<"Processed event where athlete called "<<(*it).getWho()->getName()<<" is at "<< (*it).getPointAndTime()<<endl;

      }
      catch (char const* c)
      {
          cout<<"Exception caught at highest level "<<c<<endl;
          exit(-1);
      }

        comptador++;
        if(comptador % 10000 == 0) std::cout<<" processing event "<<comptador<<" where athlete called "<<it->getWho()->getName()<<" is at "<< (*it).getPointAndTime()<<endl;
    }

    //Add a "broom athlete to every mat so all mats are closed
    comp.closeMats();

    std::cout<<std::endl<<std::endl<<std::endl<<" COMPUTING GROUP RELATIONS "<<std::endl;
    // compute group relations
    comp.computeGroupRelations();

    std::cout << std::endl << std::endl << std::endl << " COMPUTING Longest surviving group overall" << std::endl;
    std::list<component *> *longestSurvivingGroups = comp.longestSurvivingGroups();
    std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest surviving group overall found " <<longestSurvivingGroups->size()<<" that survive for this many steps "<<(*longestSurvivingGroups->begin())->maximumSurvivedSteps() << std::endl;
    std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest surviving group , The first is id "<<(*longestSurvivingGroups->begin())->getId()<<" followed by "<<(*longestSurvivingGroups->begin())->listSuccessorSurvivors()<< " with this many athletes " <<(*longestSurvivingGroups->begin())->numberOfGroupedEvents() << std::endl;

    std::cout << std::endl << std::endl << std::endl << " COMPUTING Longest included group forward overall" << std::endl;
    std::list<component *> *longestIncludedGroupsForward = comp.longestRelatedGroupForward();
    std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest included group forward overall found " <<longestIncludedGroupsForward->size()<<" that survive for this many steps "<<(*longestIncludedGroupsForward->begin())->maximumIncludedStepsForward() << std::endl;
    std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest included group forward , The first is id "<<(*longestIncludedGroupsForward->begin())->getId()<<" "<<(*longestSurvivingGroups->begin())->listSuccessorRelated()<< " with this many athletes " <<(*longestIncludedGroupsForward->begin())->numberOfGroupedEvents() << std::endl;

    std::cout << std::endl << std::endl << std::endl << " COMPUTING Longest included group backward overall" << std::endl;
    std::list<component *> *longestIncludedGroupsBackward = comp.longestRelatedGroupForward();
    std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest included group backward overall found " <<longestIncludedGroupsBackward->size()<<" that survive for this many steps "<<(*longestIncludedGroupsBackward->begin())->maximumIncludedStepsForward() << std::endl;
    std::cout << std::endl << std::endl << std::endl << " COMPUTED Longest included group backward , The first is id "<<(*longestIncludedGroupsBackward->begin())->getId()<<" "<<(*longestSurvivingGroups->begin())->listSuccessorRelated()<< " with this many athletes " <<(*longestIncludedGroupsBackward->begin())->numberOfGroupedEvents() << std::endl;


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