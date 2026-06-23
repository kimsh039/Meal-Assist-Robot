#ifndef DYNAMIXEL_CONTROLLER_CONFIG_H
#define DYNAMIXEL_CONTROLLER_CONFIG_H

#include <Arduino.h>

namespace ControllerConfig
{
constexpr uint8_t kEnablePin       = 53;        // 외부 회로 활성 출력
constexpr uint8_t kRedButtonPin    = 51;        // 식판 위치 선택 버튼
constexpr uint8_t kBlueButtonPin   = 49;        // 상하 및 식사 동작 버튼
constexpr uint8_t kPowerSwitchPin  = 47;        // 전원 상태 입력
constexpr uint32_t kSerialBaud     = 115200UL;  // PC 디버그 통신 속도
constexpr uint32_t kCommandDelayMs = 1000UL;    // 상태 명령 처리 대기 시간
}

#endif
