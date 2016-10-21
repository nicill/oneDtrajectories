cmake_minimum_required(VERSION 3.3)
project(oneDTrajectories)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp threeDTimePoint.cpp threeDTimePoint.h Race.cpp Race.h course/course.cpp course/course.h course/segment3D.cpp course/segment3D.h course/segment1D.cpp course/segment1D.h course/point3D.cpp course/point3D.h track/track.cpp track/track.h oneDTimePoint.cpp oneDTimePoint.h RunningTime.cpp RunningTime.h athlete.cpp athlete.h groupComputations.cpp groupComputations.h event.cpp event.h mat.cpp mat.h component.cpp component.h groupRelations.cpp groupRelations.h inputData.cpp inputData.h)
add_executable(oneDTrajectories ${SOURCE_FILES})