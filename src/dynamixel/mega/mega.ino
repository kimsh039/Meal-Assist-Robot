#include "StateController.h"

StateController controller;

void setup()  // 장치 초기화 진입점이다
{
  controller.begin();
}

void loop()  // 반복 실행 진입점이다
{
  controller.update();
}
