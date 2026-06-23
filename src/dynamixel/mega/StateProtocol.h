#ifndef STATE_PROTOCOL_H
#define STATE_PROTOCOL_H

#include <Arduino.h>

namespace StateProtocol
{
constexpr uint8_t kUnoAddress = 0x12;  // Uno I2C 슬레이브 주소

enum class Command : uint8_t
{
  PowerOff    = 1,  // 전원 대기 자세
  Ready       = 2,  // 사용자 앞 대기 자세
  PlateOne    = 3,  // 첫 번째 식판 위치
  PlateTwo    = 4,  // 두 번째 식판 위치
  PlateThree  = 5,  // 세 번째 식판 위치
  ForkAction  = 6,  // 포크 동작
  SpoonAction = 7   // 숟가락 동작
};
}

#endif
