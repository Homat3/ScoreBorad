
#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include <Arduino.h>                  //标准库
#include <WString.h>

#endif

//指令
struct Instruction {
  char device;
  String message;
};

class MessageManager {
private:
  char incomingByte;
  bool startBit;
  String inputString;

  Instruction currentInstruction;  //当前指令

private:
  void analyzeMessage();  //解析消息为指令

public:
  void serialEvent();            //消息事件
  Instruction getInstruction();  //获取指令
};