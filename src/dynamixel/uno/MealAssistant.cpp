#include "MealAssistant.h"

#include "DynamixelConfig.h"

void MealAssistant::begin()  // 다이나믹셀 식사 보조기를 초기화한다
{
  arm_.begin();
  arm_.moveTool(DynamixelConfig::kToolCenterRaw, DynamixelConfig::kToolCenterRaw);
  arm_.moveLinear(DynamixelConfig::kOffPoint);
}

void MealAssistant::execute(uint8_t command)  // 한 바이트 상태 명령을 수행한다
{
  switch (command)
  {
    case 1:
      arm_.moveLinear(DynamixelConfig::kOffPoint);
      break;

    case 2:
      arm_.moveLinear(DynamixelConfig::kReadyPoint);
      break;

    case 3:
      arm_.moveLinear(DynamixelConfig::kPlateOnePoint);
      break;

    case 4:
      arm_.moveLinear(DynamixelConfig::kPlateTwoPoint);
      break;

    case 5:
      arm_.moveLinear(DynamixelConfig::kPlateThreePoint);
      break;

    case 6:
      runForkAction();
      break;

    case 7:
      runSpoonAction();
      break;

    default:
      break;
  }
}

void MealAssistant::runForkAction()  // 포크로 음식을 집는 동작을 수행한다
{
  const Point3 base = arm_.currentPoint();
  const Point3 lifted = {base.x, base.y, base.z + 100.0f};
  const Point3 pulled = {base.x + 50.0f, base.y, base.z - 50.0f};

  arm_.moveLinear(lifted, false);
  arm_.moveTool(DynamixelConfig::kToolCenterRaw, DynamixelConfig::kToolForkFirstRaw);
  arm_.followLinear(lifted, base);
  arm_.followLinear(base, pulled);
  arm_.moveTool(506, DynamixelConfig::kToolForkSecondRaw);
  arm_.followLinear(pulled, base);
}

void MealAssistant::runSpoonAction()  // 숟가락으로 음식을 뜨는 동작을 수행한다
{
  const Point3 base = arm_.currentPoint();
  const Point3 curveEnd = {base.x + 50.0f, base.y, base.z};

  arm_.moveTool(DynamixelConfig::kToolCenterRaw, DynamixelConfig::kToolSpoonRaw);
  arm_.followSpoonCurve(base);
  arm_.followLinear(curveEnd, base);
}

