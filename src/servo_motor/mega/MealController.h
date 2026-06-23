#ifndef MEAL_CONTROLLER_H
#define MEAL_CONTROLLER_H

#include <Arduino.h>
#include "ServoArm.h"

class MealController
{
public:
  void begin();   // 통신, 버튼, 서보 초기화
  void update();  // 전원과 사용자 입력 처리

private:
  void handleAction();                                    // 상하 이동 또는 음식 뜨기
  void handleSelection();                                 // 식판 위치 순환 선택
  void enterPowerOff();                                   // 안전 자세에서 전원 대기
  void moveToReady();                                     // 사용자 앞 자세로 이동
  void moveToMiddle();                                    // 식판 중앙으로 이동
  bool pressed(uint8_t pin, bool& previousState);         // 풀업 버튼 눌림 에지 검출

  ServoArm arm_;
  bool armRaised_ = true;
  uint8_t selection_ = 0;
  bool previousSelect_ = HIGH;
  bool previousAction_ = HIGH;
};

#endif
