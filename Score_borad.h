
#ifndef SCORE_BORAD_H
#define SCORE_BORAD_H
#include <Arduino.h>          //标准库
#include "module.h"           //模块
#include "message_manager.h"  //通信接口
#include <WString.h>

#endif

class Team{
public:
  String name;
  int score;

  int scoredFrequency;
  int scoreRangeMax;
  int scoreRangeMin;

  int memberScore[8];
  int memberPunishment[8];
  int memberDisabled[8];

  Team();
  void addMemberScore(char whichMember, int step);
  void removeMemberScore(char whichMember, int step);
  bool punishMember(char whichMember);
  void unpunishMember(char whichMember);
};

class CurrentState {
private:
  Team teamA;
  Team teamB;

  int serTemperature;
  int serHumidity;
  int norTemperature;
  int norHumidity;
  int temperatureLine;

public:
  static bool serious;

  void setTeamName(char which, String name);
  void setTeamFrequency(char which, int frequency);
  void addTeamScore(char which, int step);
  void removeTeamScore(char which, int step);
  void addTeamMemberScore(char whichTeam, char whichMember, int step);
  void removeTeamMemberScore(char whichTeam, char whichMember, int step);
  void punishMember(char whichTeam, char whichMember);
  void unpunishMember(char whichTeam, char whichMember);
  void setTeamScoreRang(char which, int min, int max);
  
  Team getTeam(char which);
  int getSerTemperature();
  int getSerHumidity();
  int getNorTemperature();
  int getNorHumidity();
  int getTemperatureLine();

  void showMVP();

  void reset();
  void resetTeamScore(char which = 'C');
  void init();
};

class ScoreBorad {
private:
  //define pin

  //蜂鸣器
  static const int BEEP;

  //超声波传感器
  static const int TRIGGER;
  static const int ECHO;

  //风扇
  static const int FAN;

  //温湿度传感器
  static const int DHT;

  //RGB灯
  static const int RGB_R;
  static const int RGB_G;
  static const int RGB_B;

  //双色LED
  static const int DOUBLE_LED_R;
  static const int DOUBLE_LED_G;

public:
  static Beep beep;            //蜂鸣器
  static Sonar sonar;          //超声波
  static Fan fan;              //风扇
  static Dht dht;              //温湿度
  static Rgb rgb;              //RGB灯
  static DoubleLed doubleLed;  //双色LED
  static LCDScreen lcd;        //LCD显示屏

private:
  MessageManager connection;  //通信接口
  Instruction instruction;    //指令

  CurrentState data;    //数据

  bool ledMode;

  unsigned long contestLong;
  bool isInContest;
  unsigned long contestMicro;

private:
  void contestBegin();
  void contestFinished();
  void refreshScreen();
  void refreshCondition();
  void detectScore();

  static void SeriousEvent();

public:
  ScoreBorad();
  void setup();
  void loop();
  void serialEvent();  //消息事件

  void run(String message);
};