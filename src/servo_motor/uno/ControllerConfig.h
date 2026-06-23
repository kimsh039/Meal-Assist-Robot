#ifndef CONTROLLER_CONFIG_H
#define CONTROLLER_CONFIG_H

#include <Arduino.h>

namespace ControllerConfig
{
constexpr uint8_t kActionButtonPin = 2;         // 오른쪽 동작 버튼
constexpr uint8_t kSelectButtonPin = 3;         // 왼쪽 선택 버튼
constexpr uint8_t kBluetoothRxPin  = 4;         // 블루투스 수신 핀
constexpr uint8_t kBluetoothTxPin  = 5;         // 블루투스 송신 핀
constexpr uint32_t kSerialBaud     = 115200UL;  // 직렬 통신 속도
constexpr uint32_t kDebounceMs     = 50UL;      // 버튼 채터링 방지 시간
constexpr uint8_t kActionCommand   = 1;         // 상하 및 식사 동작 명령
constexpr uint8_t kSelectCommand   = 2;         // 식판 위치 선택 명령
}

#endif
