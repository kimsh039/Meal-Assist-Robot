#ifndef DYNAMIXEL_MEAL_ASSISTANT_H
#define DYNAMIXEL_MEAL_ASSISTANT_H

#include <Arduino.h>
#include "DynamixelArm.h"

class MealAssistant
{
public:
  void begin();                    // 다이나믹셀 팔 초기화
  void execute(uint8_t command);   // 상태 명령에 맞는 동작 수행

private:
  void runForkAction();     // 포크 동작 궤적
  void runSpoonAction();    // 숟가락 동작 궤적

  DynamixelArm arm_;
};

#endif
