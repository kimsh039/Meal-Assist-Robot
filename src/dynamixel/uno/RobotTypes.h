#ifndef ROBOT_TYPES_H
#define ROBOT_TYPES_H

#include <Arduino.h>

struct Point3        // 끝단의 직교 좌표
{
  float x;           // 좌우 좌표
  float y;           // 전후 좌표
  float z;           // 높이 좌표
};

struct RawPose       // 다이나믹셀 관절 위치
{
  int32_t joint[5];  // 1~5번 모터 RAW 값
};

#endif
