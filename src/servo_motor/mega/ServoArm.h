#ifndef SERVO_ARM_H
#define SERVO_ARM_H

#include <Arduino.h>
#include "ServoConfig.h"

class ServoArm
{
public:
  void begin();                                      // PWM 타이머와 시작 위치 초기화
  void moveTo(const ServoConfig::JointPose& pose);   // 여섯 관절 동시 보간 이동

private:
  void configureTimers();                                                          // Timer3과 Timer4 설정
  void calculatePulses(const ServoConfig::JointPose& pose, float pulses[6]) const;  // 관절각을 PWM 값으로 변환
  void writePulses(const float pulses[6]);                                          // PWM 레지스터 갱신
  void waitUntil(uint32_t deadlineUs) const;                                        // 보간 주기 유지

  float currentPulses_[6];
};

#endif
