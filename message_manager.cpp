
#include "message_manager.h"
#include "decoder.h"

void MessageManager::analyzeMessage() {
  //掐头去尾
  inputString.remove(0, 1);
  inputString.remove(inputString.length() - 1);
  Serial.print("Analyzing " + inputString + "\n");

  //解析-X-......
  char Device = ReadStringMsg(inputString)[0];
  String Msg = inputString;

  //操作
  currentInstruction.device = Device;
  currentInstruction.message = Msg;
}

void MessageManager::serialEvent() {

  while (Serial.available())  //如果串口接收到数据则进入循环
  {
    incomingByte = Serial.read();  //一个字节一个字节地读，下一句是读到的放入字符串数组中组成一个完成的数据包
    if (incomingByte == '^')       //如果到来的字节是'^'，开始读取
    {
      startBit = true;
    }
    if (startBit == true) {
      inputString += (char)incomingByte;  // 全双工串口可以不用在下面加延时，半双工则要加的//
    }
    if (incomingByte == '!')  //如果到来的字节是'!'，读取结束
    {
      startBit = false;
    }
    delay(10);
  }
  Serial.print("get string:" + inputString + "\n");
  analyzeMessage();
  inputString = "";
}

Instruction MessageManager::getInstruction() {
  Instruction tem = currentInstruction;
  currentInstruction.device = '\0';  //清空指令保障安全
  return tem;
}