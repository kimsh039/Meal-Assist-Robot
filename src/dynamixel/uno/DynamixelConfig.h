#ifndef DYNAMIXEL_CONFIG_H
#define DYNAMIXEL_CONFIG_H

#include <Arduino.h>
#include "RobotTypes.h"

namespace DynamixelConfig
{
constexpr uint8_t kI2cAddress            = 0x12;              // Uno I2C 슬레이브 주소
constexpr uint8_t kDirectionPin          = 2;                 // 반이중 통신 방향 핀
constexpr uint32_t kBusBaud              = 57600UL;           // 다이나믹셀 버스 속도
constexpr float kProtocolVersion         = 2.0f;              // 다이나믹셀 프로토콜 버전
constexpr uint16_t kTorqueAddress        = 64;                // Torque Enable 주소
constexpr uint16_t kAccelerationAddress  = 108;               // Profile Acceleration 주소
constexpr uint16_t kVelocityAddress      = 112;               // Profile Velocity 주소
constexpr uint32_t kProfileAcceleration  = 50UL;              // 기본 프로파일 가속도
constexpr uint32_t kProfileVelocity      = 120UL;             // 기본 프로파일 속도
constexpr uint8_t kIds[6]                = {1, 2, 3, 4, 5, 6};  // 관절별 모터 ID

constexpr float kBaseHeight = 70.0f;   // 베이스 높이
constexpr float kUpperArm   = 200.0f;  // 상박 링크 길이
constexpr float kForearm    = 180.0f;  // 하박 링크 길이

constexpr uint16_t kTrajectorySteps = 100;      // 궤적 보간 구간 수
constexpr uint32_t kArmStepUs        = 20000UL;  // 팔 궤적 단계 시간
constexpr uint32_t kToolStepUs       = 2000UL;   // 도구 회전 단계 시간

constexpr int32_t kInitialRaw[6] = {1820, 2200, 2150, 1800, 2048, 2048};  // 전원 직후 모터 위치
constexpr Point3 kInitialPoint    = {0.0f, 0.0f, 450.0f};                  // 전원 직후 가정 좌표
constexpr Point3 kOffPoint        = {0.0f, 10.0f, 200.0f};                 // 전원 대기 좌표
constexpr Point3 kReadyPoint      = {30.0f, 100.0f, 300.0f};               // 사용자 앞 대기 좌표
constexpr Point3 kPlateOnePoint   = {-20.0f, 50.0f, 140.0f};               // 첫 번째 식판 좌표
constexpr Point3 kPlateTwoPoint   = {-20.0f, 200.0f, 140.0f};              // 두 번째 식판 좌표
constexpr Point3 kPlateThreePoint = {-110.0f, 240.0f, 140.0f};             // 세 번째 식판 좌표

constexpr int32_t kToolCenterRaw      = 2048;    // 도구 중앙 위치
constexpr int32_t kToolForkFirstRaw   = 530;     // 포크 첫 번째 회전 위치
constexpr int32_t kToolForkSecondRaw  = 0;       // 포크 두 번째 회전 위치
constexpr int32_t kToolSpoonRaw       = 2554;  // 숟가락 회전 위치
}

#endif
