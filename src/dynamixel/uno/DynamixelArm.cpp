#include "DynamixelArm.h"

#include "DynamixelConfig.h"
#include "Kinematics.h"

DynamixelArm::DynamixelArm()  // 다이나믹셀 통신 객체를 구성한다
  : dxl_(Serial, DynamixelConfig::kDirectionPin),
    currentPoint_(DynamixelConfig::kInitialPoint)
{
}

void DynamixelArm::begin()  // 다이나믹셀 버스를 초기화한다
{
  dxl_.begin(DynamixelConfig::kBusBaud);
  dxl_.setPortProtocolVersion(DynamixelConfig::kProtocolVersion);

  uint32_t acceleration = DynamixelConfig::kProfileAcceleration;
  uint32_t velocity = DynamixelConfig::kProfileVelocity;
  uint8_t torqueEnabled = 1;

  for (uint8_t index = 0; index < 6; ++index)
  {
    const uint8_t id = DynamixelConfig::kIds[index];
    dxl_.write(id, DynamixelConfig::kAccelerationAddress, reinterpret_cast<uint8_t*>(&acceleration), 4);
    dxl_.write(id, DynamixelConfig::kVelocityAddress, reinterpret_cast<uint8_t*>(&velocity), 4);
    dxl_.write(id, DynamixelConfig::kTorqueAddress, &torqueEnabled, 1);
    dxl_.ledOn(id);
    dxl_.setGoalPosition(id, DynamixelConfig::kInitialRaw[index], UNIT_RAW);
  }

  delay(5000);
}

void DynamixelArm::moveLinear(const Point3& target, bool remember)  // 현재 좌표에서 목표까지 이동한다
{
  followLinear(currentPoint_, target);

  if (remember)
  {
    currentPoint_ = target;
  }
}

void DynamixelArm::followLinear(const Point3& start, const Point3& target)  // 두 좌표 사이를 직선 보간한다
{
  for (uint16_t step = 0; step <= DynamixelConfig::kTrajectorySteps; ++step)
  {
    const uint32_t deadlineUs = micros() + DynamixelConfig::kArmStepUs;
    const float ratio = static_cast<float>(step) / static_cast<float>(DynamixelConfig::kTrajectorySteps);
    writeArmPoint(interpolate(start, target, ratio));
    waitUntil(deadlineUs);
  }
}

void DynamixelArm::followSpoonCurve(const Point3& base)  // 숟가락 동작 곡선을 이동한다
{
  const Point3 points[4] = {  // 베지어 제어점 네 개를 정의한다
    base,
    {base.x, base.y, base.z - 50.0f},
    {base.x + 50.0f, base.y, base.z - 50.0f},
    {base.x + 50.0f, base.y, base.z}
  };

  for (uint16_t step = 0; step <= DynamixelConfig::kTrajectorySteps; ++step)
  {
    const uint32_t deadlineUs = micros() + DynamixelConfig::kArmStepUs;
    const float ratio = static_cast<float>(step) / static_cast<float>(DynamixelConfig::kTrajectorySteps);
    writeArmPoint(bezier(points, ratio));  // 베지어 좌표를 관절 위치로 출력한다

    if (ratio >= 0.5f)
    {
      const float returnRatio = (ratio - 0.5f) * 2.0f;
      const int32_t toolRaw = DynamixelConfig::kToolSpoonRaw + static_cast<int32_t>((DynamixelConfig::kToolCenterRaw - DynamixelConfig::kToolSpoonRaw) * returnRatio);
      writeTool(toolRaw);
    }

    waitUntil(deadlineUs);
  }
}

void DynamixelArm::moveTool(int32_t startRaw, int32_t targetRaw)  // 도구 모터를 선형 보간한다
{
  for (uint16_t step = 0; step <= DynamixelConfig::kTrajectorySteps; ++step)
  {
    const uint32_t deadlineUs = micros() + DynamixelConfig::kToolStepUs;
    const float ratio = static_cast<float>(step) / static_cast<float>(DynamixelConfig::kTrajectorySteps);
    const int32_t raw = startRaw + static_cast<int32_t>((targetRaw - startRaw) * ratio);
    writeTool(raw);
    waitUntil(deadlineUs);
  }
}

const Point3& DynamixelArm::currentPoint() const  // 기억 중인 현재 좌표를 제공한다
{
  return currentPoint_;
}

Point3 DynamixelArm::interpolate(const Point3& start, const Point3& target, float ratio) const  // 직선 보간 좌표를 계산한다
{
  return {start.x + ((target.x - start.x) * ratio), start.y + ((target.y - start.y) * ratio), start.z + ((target.z - start.z) * ratio)};
}

Point3 DynamixelArm::bezier(const Point3 points[4], float ratio) const  // 3차 베지어 좌표를 계산한다
{
  const float oneMinus = 1.0f - ratio;
  const float weight0 = oneMinus * oneMinus * oneMinus;
  const float weight1 = 3.0f * oneMinus * oneMinus * ratio;
  const float weight2 = 3.0f * oneMinus * ratio * ratio;
  const float weight3 = ratio * ratio * ratio;
  return {(weight0 * points[0].x) + (weight1 * points[1].x) + (weight2 * points[2].x) + (weight3 * points[3].x), (weight0 * points[0].y) + (weight1 * points[1].y) + (weight2 * points[2].y) + (weight3 * points[3].y), (weight0 * points[0].z) + (weight1 * points[1].z) + (weight2 * points[2].z) + (weight3 * points[3].z)};
}

void DynamixelArm::writeArmPoint(const Point3& point)  // 목표 좌표를 팔 관절에 출력한다
{
  RawPose pose;

  if (!Kinematics::solve(point, pose))
  {
    return;
  }

  for (uint8_t index = 0; index < 5; ++index)
  {
    dxl_.setGoalPosition(DynamixelConfig::kIds[index], pose.joint[index], UNIT_RAW);
  }
}

void DynamixelArm::writeTool(int32_t raw)  // 도구 모터 위치를 출력한다
{
  const int32_t safeRaw = constrain(raw, 0L, 4095L);
  dxl_.setGoalPosition(DynamixelConfig::kIds[5], safeRaw, UNIT_RAW);
}

void DynamixelArm::waitUntil(uint32_t deadlineUs) const  // 마이크로초 기준 시각까지 기다린다
{
  while (static_cast<int32_t>(micros() - deadlineUs) < 0)
  {
  }
}
