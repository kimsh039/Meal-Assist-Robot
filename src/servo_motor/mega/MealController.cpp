#include "MealController.h"

#include "ServoConfig.h"

void MealController::begin()  // 전체 장치를 초기화한다
{
  Serial.begin(ServoConfig::kDebugBaud);
  Serial3.begin(ServoConfig::kCommandBaud);

  pinMode(ServoConfig::kSelectButtonPin, INPUT_PULLUP);
  pinMode(ServoConfig::kActionButtonPin, INPUT_PULLUP);
  pinMode(ServoConfig::kPowerSwitchPin, INPUT_PULLUP);

  arm_.begin();
  delay(1000);
  enterPowerOff();
}

void MealController::update()  // 모든 입력을 한 번 처리한다
{
  if (digitalRead(ServoConfig::kPowerSwitchPin) == LOW)
  {
    enterPowerOff();
    return;
  }

  if (Serial3.available() > 0)
  {
    const uint8_t command = static_cast<uint8_t>(Serial3.read());

    if (command == 1)
    {
      handleAction();
    }
    else if (command == 2)
    {
      handleSelection();
    }
  }

  if (pressed(ServoConfig::kActionButtonPin, previousAction_))
  {
    handleAction();
  }

  if (pressed(ServoConfig::kSelectButtonPin, previousSelect_))
  {
    handleSelection();
  }
}

void MealController::handleAction()  // 주 동작 버튼을 처리한다
{
  if (armRaised_)
  {
    moveToMiddle();
    return;
  }

  Serial.println(F("음식 뜨기"));
  arm_.moveTo(ServoConfig::kScoopPose);
  arm_.moveTo(ServoConfig::kMiddlePose);
  moveToReady();
}

void MealController::handleSelection()  // 식판 위치 선택을 처리한다
{
  if (armRaised_)
  {
    return;
  }

  selection_ = static_cast<uint8_t>((selection_ + 1) % 5);

  switch (selection_)
  {
    case 1:
      Serial.println(F("식판 북쪽"));
      arm_.moveTo(ServoConfig::kNorthPose);
      break;

    case 2:
      Serial.println(F("식판 동쪽"));
      arm_.moveTo(ServoConfig::kEastPose);
      break;

    case 3:
      Serial.println(F("식판 남쪽"));
      arm_.moveTo(ServoConfig::kSouthPose);
      break;

    case 4:
      Serial.println(F("식판 서쪽"));
      arm_.moveTo(ServoConfig::kWestPose);
      break;

    default:
      moveToMiddle();
      break;
  }
}

void MealController::enterPowerOff()  // 전원 대기 절차를 수행한다
{
  Serial.println(F("전원 대기"));
  arm_.moveTo(ServoConfig::kOffPose);

  while (digitalRead(ServoConfig::kPowerSwitchPin) == LOW)
  {
    delay(10);
  }

  previousSelect_ = digitalRead(ServoConfig::kSelectButtonPin);
  previousAction_ = digitalRead(ServoConfig::kActionButtonPin);
  moveToReady();
}

void MealController::moveToReady()  // 사용자 앞 대기 자세로 이동한다
{
  Serial.println(F("사용자 앞 대기"));
  arm_.moveTo(ServoConfig::kReadyPose);
  armRaised_ = true;
  selection_ = 0;
}

void MealController::moveToMiddle()  // 식판 중앙 자세로 이동한다
{
  Serial.println(F("식판 중앙"));
  arm_.moveTo(ServoConfig::kMiddlePose);
  armRaised_ = false;
  selection_ = 0;
}

bool MealController::pressed(uint8_t pin, bool& previousState)  // 풀업 버튼의 눌림 순간을 감지한다
{
  const bool currentState = digitalRead(pin);
  const bool wasPressed = (previousState == HIGH) && (currentState == LOW);
  previousState = currentState;
  return wasPressed;
}
