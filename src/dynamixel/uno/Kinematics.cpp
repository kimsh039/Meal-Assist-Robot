#include "Kinematics.h"

#include <Arduino.h>
#include <math.h>
#include "DynamixelConfig.h"

bool Kinematics::solve(const Point3& point, RawPose& pose)  // 목표 좌표의 관절 위치를 계산한다
{
  const float radial = sqrtf((point.x * point.x) + (point.y * point.y));
  const float height = point.z - DynamixelConfig::kBaseHeight;
  const float numerator = (radial * radial) + (height * height) - (DynamixelConfig::kUpperArm * DynamixelConfig::kUpperArm) - (DynamixelConfig::kForearm * DynamixelConfig::kForearm);  // 코사인 법칙 분자를 계산한다
  const float denominator = 2.0f * DynamixelConfig::kUpperArm * DynamixelConfig::kForearm;  // 코사인 법칙 분모를 계산한다
  const float rawCosine = numerator / denominator;  // 세 번째 관절의 코사인 값을 계산한다

  if (rawCosine < -1.001f || rawCosine > 1.001f)  // 목표가 작업 공간 밖인지 확인한다
  {
    return false;
  }

  const float cosine3 = constrain(rawCosine, -1.0f, 1.0f);
  const float angle1Rad = atan2f(point.y, point.x);
  const float angle3Rad = atan2f(-sqrtf(1.0f - (cosine3 * cosine3)), cosine3);
  const float angle2Rad = atan2f(height, radial) - atan2f(DynamixelConfig::kForearm * sinf(angle3Rad), DynamixelConfig::kUpperArm + (DynamixelConfig::kForearm * cosf(angle3Rad)));  // 두 번째 관절각을 계산한다
  const float angle1Deg = angle1Rad * (180.0f / PI);  // 첫 번째 관절각을 도 단위로 바꾼다
  const float angle2Deg = angle2Rad * (180.0f / PI);  // 두 번째 관절각을 도 단위로 바꾼다
  const float angle3Deg = angle3Rad * (180.0f / PI);  // 세 번째 관절각을 도 단위로 바꾼다
  const float angle4Deg = angle2Deg + angle3Deg;  // 끝단 수평 유지를 위한 네 번째 관절각을 계산한다
  const float angle5Deg = 180.0f - angle1Deg;  // 끝단 방향 유지를 위한 다섯 번째 관절각을 계산한다

  pose.joint[0] = clampRaw(1820.0f + ((angle1Deg - 90.0f) * 4096.0f / 360.0f));  // 1번 모터 보정값을 적용한다
  pose.joint[1] = clampRaw(2200.0f + ((angle2Deg - 90.0f) * 4096.0f / 360.0f));  // 2번 모터 보정값을 적용한다
  pose.joint[2] = clampRaw(2140.0f - (angle3Deg * 4096.0f / 360.0f));  // 3번 모터 보정값을 적용한다
  pose.joint[3] = clampRaw(1840.0f - (angle4Deg * 4096.0f / 360.0f));  // 4번 모터 보정값을 적용한다
  pose.joint[4] = clampRaw(2048.0f - ((angle5Deg - 90.0f) * 4096.0f / 360.0f));  // 5번 모터 보정값을 적용한다
  return true;
}

int32_t Kinematics::clampRaw(float value)  // RAW 위치를 0~4095로 제한한다
{
  return static_cast<int32_t>(constrain(value, 0.0f, 4095.0f));
}
