#include "ButtonTransmitter.h"

ButtonTransmitter transmitter;

void setup()  // 장치 초기화 진입점이다
{
  transmitter.begin();
}

void loop()  // 반복 실행 진입점이다
{
  transmitter.update();
}
