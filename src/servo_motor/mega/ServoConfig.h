#ifndef SERVO_CONFIG_H
#define SERVO_CONFIG_H

#include <Arduino.h>

namespace ServoConfig
{
constexpr uint32_t kDebugBaud   = 115200UL;  // PC 디버그 통신 속도
constexpr uint32_t kCommandBaud = 115200UL;  // 외부 명령 통신 속도

constexpr uint8_t kSelectButtonPin = 10;  // 식판 위치 선택 버튼
constexpr uint8_t kActionButtonPin = 11;  // 상하 및 식사 동작 버튼
constexpr uint8_t kPowerSwitchPin  = 12;  // 전원 상태 입력

constexpr uint16_t kPwmTop          = 4999;                            // 50Hz PWM 주기값
constexpr uint32_t kMotionStepUs    = 10000UL;                         // 보간 단계 간격
constexpr uint16_t kInitialPulses[] = {335, 333, 455, 360, 388, 395};  // 관절별 시작 PWM

struct JointPose
{
  float joint1;
  float joint2;
  float joint3;
  float joint4;
  float joint5;
  float joint6;
};

constexpr JointPose kOffPose    = {90.00f, 70.00f, 150.00f, 59.00f, 90.00f, 180.00f};  // 전원 대기 자세
constexpr JointPose kReadyPose  = {158.20f, 68.69f, 96.89f, 104.43f, 21.80f, 80.00f};  // 사용자 앞 대기 자세
constexpr JointPose kMiddlePose = {45.00f, 67.20f, 149.59f, 53.21f, 135.00f, 80.00f};   // 식판 중앙 자세
constexpr JointPose kNorthPose  = {38.66f, 71.65f, 133.95f, 64.41f, 141.34f, 80.00f};   // 식판 북쪽 자세
constexpr JointPose kEastPose   = {65.27f, 75.87f, 128.85f, 65.28f, 114.73f, 80.00f};   // 식판 동쪽 자세
constexpr JointPose kSouthPose  = {73.30f, 52.28f, 154.82f, 62.89f, 106.70f, 80.00f};   // 식판 남쪽 자세
constexpr JointPose kWestPose   = {31.78f, 50.78f, 156.57f, 62.65f, 148.22f, 80.00f};   // 식판 서쪽 자세
constexpr JointPose kScoopPose  = {45.00f, 67.20f, 149.59f, 53.21f, 135.00f, 27.00f};   // 음식 뜨기 자세
}

#endif
