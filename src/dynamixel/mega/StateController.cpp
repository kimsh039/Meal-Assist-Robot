#include "StateController.h"

#include <Wire.h>
#include "ControllerConfig.h"

void StateController::begin()  // Mega 입력 제어기를 초기화한다
{
  Wire.begin();
  Serial.begin(ControllerConfig::kSerialBaud);

  pinMode(ControllerConfig::kEnablePin, OUTPUT);
  digitalWrite(ControllerConfig::kEnablePin, HIGH);
  pinMode(ControllerConfig::kRedButtonPin, INPUT_PULLUP);
  pinMode(ControllerConfig::kBlueButtonPin, INPUT_PULLUP);
  pinMode(ControllerConfig::kPowerSwitchPin, INPUT_PULLUP);

  previousBlue_ = digitalRead(ControllerConfig::kBlueButtonPin);
  previousRed_ = digitalRead(ControllerConfig::kRedButtonPin);
  enterPowerOff();
}

void StateController::update()  // 입력 상태를 한 번 처리한다
{
  if (digitalRead(ControllerConfig::kPowerSwitchPin) == HIGH)
  {
    enterPowerOff();
    return;
  }

  if (pressed(ControllerConfig::kBlueButtonPin, previousBlue_))
  {
    handleBlueButton();
  }

  if (pressed(ControllerConfig::kRedButtonPin, previousRed_))
  {
    handleRedButton();
  }
}

void StateController::send(StateProtocol::Command command)  // Uno에 상태 명령을 전송한다
{
  const uint8_t value = static_cast<uint8_t>(command);
  Wire.beginTransmission(StateProtocol::kUnoAddress);
  Wire.write(value);
  const uint8_t result = Wire.endTransmission();
  Serial.print(F("상태 전송: "));
  Serial.print(value);
  Serial.print(F(", 결과: "));
  Serial.println(result);
  delay(ControllerConfig::kCommandDelayMs);
}

void StateController::enterPowerOff()  // 전원 대기 절차를 수행한다
{
  Serial.println(F("전원 대기"));
  send(StateProtocol::Command::PowerOff);

  while (digitalRead(ControllerConfig::kPowerSwitchPin) == HIGH)
  {
    delay(10);
  }

  moveToReady();
  previousBlue_ = digitalRead(ControllerConfig::kBlueButtonPin);
  previousRed_ = digitalRead(ControllerConfig::kRedButtonPin);
}

void StateController::moveToReady()  // 사용자 앞 대기 자세를 요청한다
{
  send(StateProtocol::Command::Ready);
  armRaised_ = true;
  plateIndex_ = 0;
}

void StateController::handleBlueButton()  // 파란 버튼 동작을 수행한다
{
  if (armRaised_)
  {
    send(StateProtocol::Command::PlateOne);
    armRaised_ = false;
    plateIndex_ = 0;
    return;
  }

  if (plateIndex_ == 2)
  {
    send(StateProtocol::Command::ForkAction);
  }
  else
  {
    send(StateProtocol::Command::SpoonAction);
  }

  moveToReady();
}

void StateController::handleRedButton()  // 빨간 버튼 동작을 수행한다
{
  if (armRaised_)
  {
    return;
  }

  plateIndex_ = static_cast<uint8_t>((plateIndex_ + 1) % 3);
  const StateProtocol::Command commands[3] = {StateProtocol::Command::PlateOne, StateProtocol::Command::PlateTwo, StateProtocol::Command::PlateThree};
  send(commands[plateIndex_]);
}

bool StateController::pressed(uint8_t pin, bool& previousState)  // 풀업 버튼의 눌림 순간을 감지한다
{
  const bool currentState = digitalRead(pin);
  const bool wasPressed = (previousState == HIGH) && (currentState == LOW);
  previousState = currentState;
  return wasPressed;
}
