#ifndef BUTTON_TRANSMITTER_H
#define BUTTON_TRANSMITTER_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class ButtonTransmitter
{
public:
  ButtonTransmitter();
  void begin();   // 버튼과 블루투스 초기화
  void update();  // 버튼 입력 확인 및 명령 전송

private:
  bool pressed(uint8_t pin, bool& previousState);  // 눌림 에지와 채터링 확인
  void send(uint8_t command);                      // 한 바이트 명령 전송

  SoftwareSerial bluetooth_;
  bool previousAction_ = HIGH;
  bool previousSelect_ = HIGH;
  uint32_t lastPressMs_ = 0;
};

#endif
