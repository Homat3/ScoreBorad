

#ifndef MODULE_H
#define MODULE_H

#include <Arduino.h>  //标准库

#include <WString.h>
#include <NewTone.h>            //替代标准tone
#include <LiquidCrystal_I2C.h>  //LCD屏幕模块
#include <NewPing.h>            //超声波模块
#include <dht11.h>              //温湿度模块

#endif

//蜂鸣器
class Beep {
//私有属性
private:
  const int PIN;

//公共接口
public:
  void startTone(unsigned int frequency);
  void stopTone();

  Beep(int pin);
  void init();
  void run(String command);
};

//声纳
class Sonar : public NewPing {
//私有属性
private:
  const int TRIGGER_PIN;
  const int ECHO_PIN;

//公共接口
public:
  Sonar(int triggerPin, int echoPin);

  unsigned int getDistanceCM();
};

//风扇
class Fan {
//私有属性
private:
  const int PIN;

//公共接口
public:
  void roll(int speed);
  void stopRoll();

  Fan(int pin);
  void init();
  void run(String command);
};

//温湿度传感器
class Dht : public dht11 {
//私有属性
private:
  const int PIN;

//公共接口
public:
  void updateData();

  Dht(int pin);
  void init();
  void run(String command);
};

//RGB灯
class Rgb {
//私有属性
private:
  const int R_PIN;
  const int G_PIN;
  const int B_PIN;

//公共接口
public:
  void lightRed();
  void lightGreen();
  void lightBlue();
  void shutDown();

  Rgb(int rPin, int gPin, int bPin);
  void init();
  void run(String command);
};

//双色LED
class DoubleLed {
//私有属性
private:
  const int R_PIN;
  const int G_PIN;

//公共接口
public:
  void lightRed();
  void lightGreen();
  void shutDown();

  DoubleLed(int rPin, int gPin);
  void init();
  void run(String command);
};

//LCD屏幕
class LCDScreen : public LiquidCrystal_I2C {
//私有属性
private:

//公共接口
public:
  LCDScreen(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
  void init();
  void run(String command);
};
