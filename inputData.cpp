//
// Created by yago on 16/10/21.
//

#include "inputData.h"

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


void inputData::readBostonMarathon(vector<athlete*> *runners, char * fileName,groupComputations *gc)
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

            //cout << line << '\n';

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
                 cout<<" caught exception at number "<<athleteCounter<<" 5K athlete name "<<a->getName()<<endl;
                b5k=false;
            }

            try
            {
                tp10=oneDTimePoint(10000, stringToRunningTime(s10k));
                //a.addTimePoint(tp10);
            }
            catch (exception e)
            {
                cout<<" caught exception at number "<<athleteCounter<<" 10K athlete name "<<a->getName()<<endl;
                b10k=false;
            }

            try
            {
                tp20=oneDTimePoint(20000, stringToRunningTime(s20k));
                //a.addTimePoint(tp20);
            }
            catch (exception e)
            {
                 cout<<" caught exception at number "<<athleteCounter<<" 20K athlete name "<<a->getName()<<endl;
                b20k=false;
            }

            try
            {
                tpHalf=oneDTimePoint(21097, stringToRunningTime(sHalf));
                // a.addTimePoint(tpHalf);
            }
            catch (exception e)
            {
                cout<<" caught exception at number "<<athleteCounter<<" HALF athlete name "<<a->getName()<<endl;
                bHalf=false;
            }

            try
            {
                tp25=oneDTimePoint(25000, stringToRunningTime(s25k));
                //a.addTimePoint(tp25);
            }
            catch (exception e)
            {
                cout<<" caught exception at number "<<athleteCounter<<" 25k athlete name "<<a->getName()<<endl;
                b25k=false;
            }

            try
            {
                tp30=oneDTimePoint(30000, stringToRunningTime(s30k));
                //a.addTimePoint(tp30);
            }
            catch (exception e)
            {
                cout<<" caught exception at number "<<athleteCounter<<" 30K athlete name "<<a->getName()<<endl;
                b30k=false;
            }

            try
            {
                tp35=oneDTimePoint(35000, stringToRunningTime(s35k));
                //a.addTimePoint(tp35);
            }
            catch (exception e)
            {
                cout<<" caught exception at number "<<athleteCounter<<" 35K athlete name "<<a->getName()<<endl;
                b35k=false;
            }

            try
            {
                tp40=oneDTimePoint(40000, stringToRunningTime(s40k));
                //a.addTimePoint(tp40);
            }
            catch (exception e)
            {
                cout<<" caught exception at number "<<athleteCounter<<" 40K athlete name "<<a->getName()<<endl;
                b40k=false;
            }

            try
            {
                tpEND=oneDTimePoint(42195, stringToRunningTime(sEnd));
                //a.addTimePoint(tpEND);
            }
            catch (exception e)
            {
                cout<<" caught exception at number "<<athleteCounter<<" END athlete name "<<a->getName()<<endl;
                bEND=false;
            }

            a->checkConsistency();

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
            else
            {
                //cout<<" Athlete "<<athleteCounter<<" with name "<<a->getName()<<" is a mother fucker"<<endl;
                cout << line << '\n';
                //exit(-1);

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

void inputData::addSluggishJoes(vector<athlete*> *runners, groupComputations *gc,int numAthletes, int numTimePoints, int epsilon,  int rythm, bool exploding, vector<int> vExplode)
{
    int athleteCounter=1;
    int athletesBefore=runners->size();

    while(athleteCounter<=numAthletes)
    {
        athlete *a=new athlete() ;
        a->setId(athleteCounter+athletesBefore);

        // cout << line << '\n';

        //set all athlete parameters (except the id)
        string s="SlugJoe";
        a->setName(s.append(std::to_string(athleteCounter+athletesBefore)));

        for(int i=0;i<numTimePoints;i++)
        {
            //cout<<"creating time point  "<<i<<endl;
            oneDTimePoint *tp;
            int minutes,seconds;

            //decide minutes,  these guys run fast at first and slow down
            if(i<numTimePoints/3) { minutes=rythm * (i + 1);}
            else if(i<2*numTimePoints/3) {minutes=(rythm+1) * (i + 1);}
            else {minutes=(rythm+2) * (i + 1);}

            // decide seconds, if exploding they are separated
            if(exploding&&find(vExplode.begin(),vExplode.end(),i)!=vExplode.end() ) {seconds=(((athleteCounter*(epsilon+1)))%60);}
            else{seconds=0;}

            //create and add event
            tp = new oneDTimePoint((i + 1) * 1000,RunningTime(minutes,seconds));
            event e(a, *tp);
            gc->addEvent(e);

            //cout<<"created event  "<<e<<endl;
        }
        a->checkConsistency();
        runners->push_back(a);

        //cout << "ATHLETE " << a << endl;
        athleteCounter++;
    }
}

void inputData::addRegularRunningGuys(vector<athlete*> *runners, groupComputations *gc,int numAthletes,int numTimePoints, int epsilon, int rythm, bool exploding,vector<int> vExplode,
                                      bool splitting , vector<int> vSplit, bool shrinking , vector<int> vShrink, bool disbanding , vector<int> vDisband )
{
    int athleteCounter=1;
    int athletesBefore=runners->size();

    // behavior parameters
    int splitfactor=3;
    int shrinkfactor=5;

    while(athleteCounter<=numAthletes)
    {
        athlete *a=new athlete() ;
        a->setId(athleteCounter+athletesBefore);

        // cout << line << '\n';
        //set all athlete parameters (except the id)
        string s="reggie";
        a->setName(s.append(std::to_string(athleteCounter+athletesBefore)));

        for(int i=0;i<numTimePoints;i++)
        {
            //cout<<"creating time point  "<<i<<endl;
            oneDTimePoint *tp;
            int minutes,seconds;

            // athletes running at "rythm" min / km,
            //minutes = floor(rythm * (i + 1));
            //seconds = 0;
            minutes = (rythm * (i + 1))/60;
            seconds = (rythm * (i + 1))%60;


            //if some other behavior is detected, adjust parameters
            if(exploding&&find(vExplode.begin(),vExplode.end(),i)!=vExplode.end() ) // these explode
            {
                seconds = ((athleteCounter * (epsilon + 1))) % 60;
                if( (athleteCounter * (epsilon + 1)) >60 ) minutes=minutes+((athleteCounter * (epsilon + 1))/60);
            }
            else if(splitting&&find(vSplit.begin(),vSplit.end(),i)!=vSplit.end() ) //these split
            {
                switch(athleteCounter%splitfactor) {
                    case 0: {
                        minutes=minutes+1;
                        break;
                    }
                    case 1: {
                        minutes=minutes-1;
                        break;
                    }
                    default: // for the other case, do nothing, the value we have is ok.
                    break;
                }
            }
            else if(shrinking&&find(vShrink.begin(),vShrink.end(),i)!=vShrink.end() ) //these shrink to 1/5 of their size
            {
                switch (athleteCounter % shrinkfactor) {
                    case 0: {
                        // do nothing in this case, these guys are ok
                        break;
                    }
                    default: // mostly everyone disbands in this case.
                        seconds = (athleteCounter * (epsilon + 1)) % 60;
                        if ((athleteCounter * (epsilon + 1)) > 60) minutes = minutes + ((athleteCounter * (epsilon + 1)) / 60);
                        break;
                }
            }
            else if(disbanding&&find(vDisband.begin(),vDisband.end(),i)!=vDisband.end() ) //these Disband
            {
                switch(athleteCounter%splitfactor) {
                    case 0: {
                        minutes=minutes+1;
                        break;
                    }
                    case 1: {
                        minutes=minutes-1;
                        break;
                    }
                    default: // for the other case, explode.
                        seconds = ((athleteCounter * (epsilon + 1))) % 60;
                        if( (athleteCounter * (epsilon + 1)) >60 ) minutes=minutes+((athleteCounter * (epsilon + 1))/60);
                        break;
                }
            }


            tp = new oneDTimePoint((i + 1) * 1000, RunningTime(minutes, seconds ));
            event e(a, *tp);
            gc->addEvent(e);

            //cout<<"created event  "<<e<<endl;
         }
        a->checkConsistency();
        runners->push_back(a);

        //cout << "ATHLETE " << a << endl;
        athleteCounter++;
    }
}