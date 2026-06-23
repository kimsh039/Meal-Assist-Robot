#include "ServoArm.h"

#include <math.h>

void ServoArm::begin()  // PWM 장치를 초기화한다
{
  configureTimers();

  for (uint8_t index = 0; index < 6; ++index)
  {
    currentPulses_[index] = ServoConfig::kInitialPulses[index];
  }

  writePulses(currentPulses_);
}

void ServoArm::moveTo(const ServoConfig::JointPose& pose)  // 여섯 관절을 동시 보간한다
{
  float targetPulses[6];
  float startPulses[6];
  float maximumDelta = 0.0f;

  calculatePulses(pose, targetPulses);

  for (uint8_t index = 0; index < 6; ++index)
  {
    startPulses[index] = currentPulses_[index];
    maximumDelta = max(maximumDelta, fabsf(targetPulses[index] - startPulses[index]));
  }

  const uint16_t stepCount = static_cast<uint16_t>(ceilf(maximumDelta));

  if (stepCount == 0)
  {
    return;
  }

  for (uint16_t step = 1; step <= stepCount; ++step)
  {
    const uint32_t deadlineUs = micros() + ServoConfig::kMotionStepUs;
    const float ratio = static_cast<float>(step) / static_cast<float>(stepCount);
    float interpolated[6];

    for (uint8_t index = 0; index < 6; ++index)
    {
      interpolated[index] = startPulses[index] + ((targetPulses[index] - startPulses[index]) * ratio);
    }

    writePulses(interpolated);
    waitUntil(deadlineUs);
  }

  for (uint8_t index = 0; index < 6; ++index)
  {
    currentPulses_[index] = targetPulses[index];
  }
}

void ServoArm::configureTimers()  // Mega 2560의 PWM 타이머를 설정한다
{
  DDRH |= 0b00111000;
  DDRE |= 0b00111000;

  TCCR3A = _BV(WGM31) | _BV(COM3A1) | _BV(COM3B1) | _BV(COM3C1);  // Timer3 Fast PWM 출력 모드를 설정한다
  TCCR3B = _BV(WGM33) | _BV(WGM32) | _BV(CS31) | _BV(CS30);  // Timer3 분주비를 64로 설정한다
  TCCR4A = _BV(WGM41) | _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1);  // Timer4 Fast PWM 출력 모드를 설정한다
  TCCR4B = _BV(WGM43) | _BV(WGM42) | _BV(CS41) | _BV(CS40);  // Timer4 분주비를 64로 설정한다

  ICR3  = ServoConfig::kPwmTop;  // Timer3 주기를 50Hz로 설정한다
  ICR4  = ServoConfig::kPwmTop;  // Timer4 주기를 50Hz로 설정한다
  TCNT3 = 0;
  TCNT4 = 0;
}

void ServoArm::calculatePulses(const ServoConfig::JointPose& pose, float pulses[6]) const  // 각도를 보정 PWM으로 변환한다
{
  pulses[0] = 17.0f  + ((270.0f - pose.joint1) * 500.0f / 270.0f);  // 1번 관절 보정값을 적용한다
  pulses[1] = 166.0f + (pose.joint2 * 500.0f / 270.0f);  // 2번 관절 보정값을 적용한다
  pulses[2] = 117.0f + ((270.0f - pose.joint3) * 500.0f / 270.0f);  // 3번 관절 보정값을 적용한다
  pulses[3] = 120.0f + ((180.0f - pose.joint4) * 500.0f / 180.0f);  // 4번 관절 보정값을 적용한다
  pulses[4] = 138.0f + ((180.0f - pose.joint5) * 500.0f / 180.0f);  // 5번 관절 보정값을 적용한다
  pulses[5] = 145.0f + (pose.joint6 * 500.0f / 180.0f);  // 6번 관절 보정값을 적용한다
}

void ServoArm::writePulses(const float pulses[6])  // 여섯 PWM 출력을 갱신한다
{
  OCR4A = static_cast<uint16_t>(pulses[0]);  // 1번 서보 PWM을 기록한다
  OCR4B = static_cast<uint16_t>(pulses[1]);  // 2번 서보 PWM을 기록한다
  OCR4C = static_cast<uint16_t>(pulses[2]);  // 3번 서보 PWM을 기록한다
  OCR3A = static_cast<uint16_t>(pulses[3]);  // 4번 서보 PWM을 기록한다
  OCR3B = static_cast<uint16_t>(pulses[4]);  // 5번 서보 PWM을 기록한다
  OCR3C = static_cast<uint16_t>(pulses[5]);  // 6번 서보 PWM을 기록한다
}

void ServoArm::waitUntil(uint32_t deadlineUs) const  // 마이크로초 기준 시각까지 기다린다
{
  while (static_cast<int32_t>(micros() - deadlineUs) < 0)
  {
  }
}
