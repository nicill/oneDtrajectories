//
// Created by yago on 16/10/21.
//

#ifndef ONEDTRAJECTORIES_INPUTDATA_H
#define ONEDTRAJECTORIES_INPUTDATA_H

#include "RunningTime.h"
#include "athlete.h"
#include "groupComputations.h"

using namespace std;

bool isInteger(const std::string & s);

RunningTime stringToRunningTime(string s);


// functional class to read or create inputs for the trajectory following application
class inputData {
public:
    void readBostonMarathon(vector<athlete*> *runners, char * fileName,groupComputations *gc);

    void addSluggishJoes(vector<athlete*> *runners, groupComputations *gc,int numAthletes, int numTimePoints, int epsilon,  int rythm, bool exploding=false, vector<int> vExplode=vector<int>() );

    void addRegularRunningGuys(vector<athlete*> *runners, groupComputations *gc,int numAthletes, int numTimePoints, int epsilon, int rythm, bool exploding=false, vector<int> vExplode=vector<int>(), bool splitting=false ,
                               vector<int> vSplit=vector<int>(), bool shrinking=false , vector<int> vShrink=vector<int>(),bool disbanding=false , vector<int> vDisband=vector<int>() ) ;

};


#endif //ONEDTRAJECTORIES_INPUTDATA_H
