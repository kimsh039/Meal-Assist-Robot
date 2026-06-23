#include <Wire.h>
#include "DynamixelConfig.h"
#include "MealAssistant.h"

constexpr uint8_t kCommandQueueSize = 8;
volatile uint8_t commandQueue[kCommandQueueSize];
volatile uint8_t commandHead = 0;
volatile uint8_t commandTail = 0;
MealAssistant assistant;

void receiveCommand(int byteCount)  // I2C 수신 인터럽트를 처리한다
{
  if (byteCount > 0 && Wire.available() > 0)
  {
    const uint8_t value = static_cast<uint8_t>(Wire.read());

    const uint8_t nextHead = static_cast<uint8_t>((commandHead + 1) % kCommandQueueSize);

    if (value >= 1 && value <= 7 && nextHead != commandTail)
    {
      commandQueue[commandHead] = value;
      commandHead = nextHead;
    }
  }

  while (Wire.available() > 0)
  {
    Wire.read();
  }
}

void setup()  // 장치 초기화 진입점이다
{
  Wire.begin(DynamixelConfig::kI2cAddress);
  Wire.onReceive(receiveCommand);
  assistant.begin();
}

bool dequeueCommand(uint8_t& command)  // 버튼으로 전달된 다음 명령을 꺼낸다
{
  if (commandTail == commandHead)
  {
    return false;
  }

  noInterrupts();
  command = commandQueue[commandTail];
  commandTail = static_cast<uint8_t>((commandTail + 1) % kCommandQueueSize);
  interrupts();
  return true;
}

void loop()  // 반복 실행 진입점이다
{
  uint8_t command;

  if (dequeueCommand(command))
  {
    assistant.execute(command);
  }
}
