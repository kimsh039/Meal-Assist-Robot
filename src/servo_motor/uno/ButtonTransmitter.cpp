#include "ButtonTransmitter.h"

#include "ControllerConfig.h"

ButtonTransmitter::ButtonTransmitter()  // 블루투스 포트를 구성한다
  : bluetooth_(ControllerConfig::kBluetoothRxPin, ControllerConfig::kBluetoothTxPin)
{
}

void ButtonTransmitter::begin()  // 입력 컨트롤러를 초기화한다
{
  Serial.begin(ControllerConfig::kSerialBaud);
  bluetooth_.begin(ControllerConfig::kSerialBaud);
  pinMode(ControllerConfig::kActionButtonPin, INPUT_PULLUP);
  pinMode(ControllerConfig::kSelectButtonPin, INPUT_PULLUP);
  previousAction_ = digitalRead(ControllerConfig::kActionButtonPin);
  previousSelect_ = digitalRead(ControllerConfig::kSelectButtonPin);
}

void ButtonTransmitter::update()  // 버튼 입력을 한 번 처리한다
{
  if (pressed(ControllerConfig::kActionButtonPin, previousAction_))
  {
    send(ControllerConfig::kActionCommand);
  }
  else if (pressed(ControllerConfig::kSelectButtonPin, previousSelect_))
  {
    send(ControllerConfig::kSelectCommand);
  }
}

bool ButtonTransmitter::pressed(uint8_t pin, bool& previousState)  // 버튼의 눌림 순간과 채터링을 판정한다
{
  const bool currentState = digitalRead(pin);
  const uint32_t nowMs = millis();
  const bool fallingEdge = (previousState == HIGH) && (currentState == LOW);
  const bool debouncePassed = (nowMs - lastPressMs_) >= ControllerConfig::kDebounceMs;
  previousState = currentState;

  if (fallingEdge && debouncePassed)
  {
    lastPressMs_ = nowMs;
    return true;
  }

  return false;
}

void ButtonTransmitter::send(uint8_t command)  // 명령 한 바이트를 전송한다
{
  bluetooth_.write(command);
  Serial.println(command);
}
