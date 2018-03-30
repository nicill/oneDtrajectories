//
// Created by yago on 16/04/19.
//

#include "oneDTimePoint.h"


double oneDTimePoint::computePace(oneDTimePoint previous)
{
 double distance=position-previous.position;
 //std::cout<<"distance  "<<distance<<std::endl;

 int time=t.difference(previous.getTime());

 if(t<0||distance<0)
 {
  std::cout<<"oneDTimePoint::computePace something is negative, throwing exception "<<std::endl;
  throw ("oneDTimePoint::computePace something is negative, throwing exception ");
 }

 //std::cout<<"returning  "<<(1000*time/distance)<<std::endl;


 return (1000*time/distance);
}
