#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "RobotTypes.h"

class Kinematics
{
public:
  static bool solve(const Point3& point, RawPose& pose);  // 직교 좌표를 관절 위치로 변환

private:
  static int32_t clampRaw(float value);                   // RAW 값을 안전 범위로 제한
};

#endif
