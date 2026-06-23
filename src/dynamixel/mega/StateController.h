#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include <Arduino.h>
#include "StateProtocol.h"

class StateController
{
public:
  void begin();   // I2C와 버튼 입력 초기화
  void update();  // 전원과 버튼 상태 처리

private:
  void send(StateProtocol::Command command);         // Uno에 상태 명령 전송
  void enterPowerOff();                              // 전원 대기 절차 수행
  void moveToReady();                                // 사용자 앞 대기 요청
  void handleBlueButton();                           // 식사 동작 버튼 처리
  void handleRedButton();                            // 식판 선택 버튼 처리
  bool pressed(uint8_t pin, bool& previousState);    // 풀업 버튼 눌림 에지 검출

  bool armRaised_ = true;
  uint8_t plateIndex_ = 0;
  bool previousBlue_ = HIGH;
  bool previousRed_ = HIGH;
};

#endif
