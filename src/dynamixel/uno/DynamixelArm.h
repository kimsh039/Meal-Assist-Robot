#ifndef DYNAMIXEL_ARM_H
#define DYNAMIXEL_ARM_H

#include <Arduino.h>
#include <Dynamixel2Arduino.h>
#include "RobotTypes.h"

class DynamixelArm
{
public:
  DynamixelArm();
  void begin();                                                        // 버스와 모터 초기화
  void moveLinear(const Point3& target, bool remember = true);         // 현재 좌표에서 직선 이동
  void followLinear(const Point3& start, const Point3& target);        // 지정 좌표 사이 임시 이동
  void followSpoonCurve(const Point3& base);                            // 숟가락 베지어 궤적 수행
  void moveTool(int32_t startRaw, int32_t targetRaw);                   // 도구 모터 선형 보간
  const Point3& currentPoint() const;                                   // 기억 중인 기준 좌표 반환

private:
  Point3 interpolate(const Point3& start, const Point3& target, float ratio) const;  // 직선 보간 좌표 계산
  Point3 bezier(const Point3 points[4], float ratio) const;                         // 3차 베지어 좌표 계산
  void writeArmPoint(const Point3& point);                                           // 좌표를 1~5번 모터에 출력
  void writeTool(int32_t raw);                                                       // 6번 도구 모터 출력
  void waitUntil(uint32_t deadlineUs) const;                                         // 궤적 주기 유지

  Dynamixel2Arduino dxl_;
  Point3 currentPoint_;
};

#endif
