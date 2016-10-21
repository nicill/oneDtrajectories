//
// Created by yago on 16/04/19.
//

#include "RunningTime.h"

int RunningTime::difference(RunningTime t) {
 //std::cout<<"time difference "<<( (minutes-t.minutes)*60 + (seconds-t.seconds ) )<<std::endl;
 return ( (minutes-t.minutes)*60 + (seconds-t.seconds ) );
}
