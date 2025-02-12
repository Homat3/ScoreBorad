#include "Arduino.h"

#include <MsTimer2.h>
#include "Score_borad.h"
#include "decoder.h"



//static:
bool CurrentState::serious = false;
//蜂鸣器
const int ScoreBorad::BEEP = 6;
//超声波传感器
const int ScoreBorad::TRIGGER = 2;
const int ScoreBorad::ECHO = 3;
//风扇
const int ScoreBorad::FAN = 5;
//温湿度传感器
const int ScoreBorad::DHT = 7;
//RGB灯
const int ScoreBorad::RGB_R = 9;
const int ScoreBorad::RGB_G = 10;
const int ScoreBorad::RGB_B = 11;
//双色LED
const int ScoreBorad::DOUBLE_LED_R = 12;
const int ScoreBorad::DOUBLE_LED_G = 8;
Beep ScoreBorad::beep(BEEP);                                  //蜂鸣器
Sonar ScoreBorad::sonar(TRIGGER, ECHO);                       //超声波
Fan ScoreBorad::fan(FAN);                                     //风扇
Dht ScoreBorad::dht(DHT);                                     //温湿度
Rgb ScoreBorad::rgb(RGB_R, RGB_G, RGB_B);                     //RGB灯
DoubleLed ScoreBorad::doubleLed(DOUBLE_LED_R, DOUBLE_LED_G);  //双色LED
LCDScreen ScoreBorad::lcd(0x27, 16, 2);                       //LCD显示屏
void ScoreBorad::SeriousEvent() {
  static int times = 0;
  times = (times + 1) % 24;         //设置周期

  if (CurrentState::serious) {
    if (times % 2) ScoreBorad::doubleLed.lightGreen();
    else ScoreBorad::doubleLed.lightRed();              //红绿闪烁
    if (times == 1) ScoreBorad::beep.startTone(600);
    else if (times == 21) ScoreBorad::beep.stopTone();  //设置每5秒停一秒
  }
}


Team::Team(){
  for (int i = 0; i < 8; i++) memberScore[i] = memberPunishment[i] = 0;
}
void Team::addMemberScore(char whichMember, int step){
  memberScore[whichMember - '0'] += step;
}
void Team::removeMemberScore(char whichMember, int step){
  memberScore[whichMember - '0'] -= step;
  if (memberScore[whichMember - '0'] < 0) memberScore[whichMember - '0'] = 0;
}
bool Team::punishMember(char whichMember){
  memberPunishment[whichMember - '0']++;
  memberDisabled[whichMember - '0'] = memberPunishment[whichMember - '0'] >= 5;
  return memberDisabled[whichMember - '0'];//罚五次则为true
}
void Team::unpunishMember(char whichMember){
  memberPunishment[whichMember - '0'] = 0;
  memberDisabled[whichMember - '0'] = false;
  if (memberPunishment[whichMember - '0'] < 0) memberScore[whichMember - '0'] = 0;
}



void CurrentState::setTeamName(char which, String name) {
  if (which == 'A') {
    teamA.name = name;
  } else if (which == 'B') {
    teamB.name = name;
  }
}
void CurrentState::setTeamFrequency(char which, int frequency){
  if (which == 'A') {
    teamA.scoredFrequency = frequency;
  } else if (which == 'B') {
    teamB.scoredFrequency = frequency;
  }
}
void CurrentState::addTeamScore(char which, int step) {
  if (which == 'A') {
    teamA.score += step;
  } else if (which == 'B') {
    teamB.score += step;
  }
}
void CurrentState::removeTeamScore(char which, int step) {
  if (which == 'A') {
    teamA.score -= step;
    if (teamA.score < 0) teamA.score = 0;
  } else if (which == 'B') {
    teamB.score -= step;
    if (teamB.score < 0) teamB.score = 0;
  }
}
void CurrentState::addTeamMemberScore(char whichTeam, char whichMember, int step){
  if (whichTeam == 'A') {
    teamA.addMemberScore(whichMember, step);
  } else if (whichTeam == 'B') {
    teamB.addMemberScore(whichMember, step);
  }
}
void CurrentState::punishMember(char whichTeam, char whichMember){
  if (whichTeam == 'A') {
    if (teamA.punishMember(whichMember))//如果罚完大于5次
    {
      ScoreBorad::lcd.setCursor(0, 0);
      ScoreBorad::lcd.print(whichMember);
      ScoreBorad::lcd.print(" in team " + teamA.name);
      ScoreBorad::lcd.setCursor(0, 1);
      ScoreBorad::lcd.print("has been kicked");
      ScoreBorad::beep.startTone(600);
      ScoreBorad::rgb.lightRed();
      delay(3000);
      ScoreBorad::beep.stopTone();
      ScoreBorad::rgb.shutDown();
    }
  } else if (whichTeam == 'B') {
    if (teamB.punishMember(whichMember))
    {
      ScoreBorad::lcd.setCursor(0, 0);
      ScoreBorad::lcd.print(whichMember);
      ScoreBorad::lcd.print(" in team " + teamB.name);
      ScoreBorad::lcd.setCursor(0, 1);
      ScoreBorad::lcd.print("has been kicked");
      ScoreBorad::beep.startTone(600);
      ScoreBorad::rgb.lightRed();
      delay(3000);
      ScoreBorad::beep.stopTone();
      ScoreBorad::rgb.shutDown();
    }
  }
}
void CurrentState::unpunishMember(char whichTeam, char whichMember){
  if (whichTeam == 'A') {
    teamA.unpunishMember(whichMember);
  } else if (whichTeam == 'B') {
    teamB.unpunishMember(whichMember);
  }
}
void CurrentState::removeTeamMemberScore(char whichTeam, char whichMember, int step){
  if (whichTeam == 'A') {
    teamA.removeMemberScore(whichMember, step);
  } else if (whichTeam == 'B') {
    teamB.removeMemberScore(whichMember, step);
  }
}
void CurrentState::setTeamScoreRang(char which, int min, int max){
  if (which == 'A') {
    teamA.scoreRangeMin = min;
    teamA.scoreRangeMax = max;
  } else if (which == 'B') {
    teamB.scoreRangeMin = min;
    teamB.scoreRangeMax = max;
  }
}
Team CurrentState::getTeam(char which) {
  if (which == 'A') {
    return teamA;
  } else if (which == 'B') {
    return teamB;
  } else return teamA;
}
int CurrentState::getSerTemperature() {
  return serTemperature;
}
int CurrentState::getSerHumidity() {
  return serHumidity;
}
int CurrentState::getNorTemperature() {
  return serTemperature;
}
int CurrentState::getNorHumidity() {
  return serHumidity;
}
int CurrentState::getTemperatureLine(){
  return temperatureLine;
}
void CurrentState::showMVP(){
  int MVP_A = 0;
  for (int i; i < 8; i++) {
    if (!teamA.memberDisabled[i]) { MVP_A = teamA.memberScore[i] > teamA.memberScore[MVP_A] ? i : MVP_A;}
  }
  int MVP_B = 0;
  for (int i; i < 8; i++) {
    if (!teamB.memberDisabled[i]) { MVP_B = teamB.memberScore[i] > teamB.memberScore[MVP_B] ? i : MVP_B;}
  }
  ScoreBorad::lcd.setCursor(0, 0);
  ScoreBorad::lcd.print("                ");
  ScoreBorad::lcd.setCursor(0, 0);
  ScoreBorad::lcd.print(teamA.name + " MVP: ");
  ScoreBorad::lcd.print(MVP_A);
  ScoreBorad::lcd.print(": ");
  ScoreBorad::lcd.print(teamA.memberScore[MVP_A]);
  ScoreBorad::lcd.setCursor(0, 1);
  ScoreBorad::lcd.print("                ");
  ScoreBorad::lcd.setCursor(0, 1);
  ScoreBorad::lcd.print(teamB.name + " MVP: ");
  ScoreBorad::lcd.print(MVP_B);
  ScoreBorad::lcd.print(": ");
  ScoreBorad::lcd.print(teamB.memberScore[MVP_B]);
  delay(5000);
}
void CurrentState::reset() {
  resetTeamScore('A');
  resetTeamScore('B');
  teamA.name = "A";
  teamB.name = "B";
  teamA.scoreRangeMax = 40;
  teamA.scoreRangeMin = 20;
  teamB.scoreRangeMax = 100;
  teamB.scoreRangeMin = 80;
  teamA.scoredFrequency = 400;
  teamB.scoredFrequency = 300;
  for (int i = 0; i < 8; i++) teamA.memberPunishment[i] = teamB.memberPunishment[i] = 0;
}
void CurrentState::resetTeamScore(char which) {
  if (which == 'A') {
    teamA.score = 0;
    for (int i = 0; i < 8; i++) teamA.memberScore[i] = 0;
  } else if (which == 'B') {
    teamB.score = 0;
    for (int i = 0; i < 8; i++) teamB.memberScore[i] = 0;
  } else {
    teamA.score = 0;
    teamB.score = 0;
    for (int i = 0; i < 8; i++) teamA.memberScore[i] = teamB.memberScore[i] = 0;
  }
}
void CurrentState::init() {
  reset();
  serTemperature = 30;
  serHumidity =99;
  norTemperature = 25;
  norHumidity = 95;
  temperatureLine = 20;
}



void ScoreBorad::contestBegin(){
  Serial.print("Contest begins");
  data.resetTeamScore();
  isInContest = true;
  contestMicro = micros();
}
void ScoreBorad::contestFinished(){
  Serial.print("Contest finishes");
  isInContest = false;
  contestMicro = 0;
  data.showMVP();
}
void ScoreBorad::refreshScreen() {
  static int times = 0;
  times = (times + 1) % 30;
  if (times) return;              //降低屏幕刷新频率
  if (CurrentState::serious){     //紧急情况显示
    lcd.setCursor(0, 0);
    lcd.print("Tem:" + String(dht.temperature) + "C");
    lcd.setCursor(8, 0);
    lcd.print("Hum:" + String(dht.humidity) + "%");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.print("Serious!!");
  } else {                        //正常显示
    if (isInContest) {            //比赛中
      lcd.setCursor(0, 0);
      lcd.print("                ");
      Team team;
      team = data.getTeam('A');
      lcd.setCursor(0, 0);
      lcd.print(team.name + ":");
      lcd.setCursor(5, 0);
      lcd.print(String(team.score));
      team = data.getTeam('B');
      lcd.setCursor(8, 0);
      lcd.print(team.name + ":");
      lcd.setCursor(13, 0);
      lcd.print(String(team.score));
      lcd.setCursor(0, 1);
      lcd.print("Normal condition");
    }
    else {                        //无比赛
      lcd.setCursor(0, 0);
      lcd.print("No contest      ");
      lcd.setCursor(0, 1);
      lcd.print("Normal condition");
    }
  }
}
void ScoreBorad::refreshCondition() {
  static int times = 0;
  times = (times + 1) % 30;
  if (times) return;              //降低屏幕刷新频率
  dht.updateData();
  
  if (ledMode){
    if (CurrentState::serious) {
      if (dht.temperature < data.getNorTemperature() && dht.humidity < data.getNorHumidity()){//解除危机
        CurrentState::serious = false;
        MsTimer2::stop();
        doubleLed.shutDown();
        beep.stopTone();
      }
    }
    else {
      if (dht.temperature > data.getSerTemperature() && dht.humidity > data.getSerHumidity()){//开启危机
        CurrentState::serious = true;
        MsTimer2::start();
      }
    }
  }
  else {
    if (dht.temperature > data.getTemperatureLine()) {
      doubleLed.lightGreen();
      fan.roll(250);
    }
    else {
      doubleLed.lightRed();
      fan.roll(100);
    }
  }
}
void ScoreBorad::detectScore(){
  static int times = 0;
  times = (times + 1) % 15;
  if (times) return;

  Serial.println(dht.temperature);
  Serial.println(dht.humidity);

  if (isInContest) {//正在比赛
    int distance = sonar.getDistanceCM();//获取距离
    Serial.print("Detect ");
    Serial.println(distance);
    Team A = data.getTeam('A');
    Team B = data.getTeam('B');
    if (A.scoreRangeMin <= distance && distance <= A.scoreRangeMax) {
      //在A框范围
      data.addTeamScore('A', 1);
      beep.startTone(data.getTeam('A').scoredFrequency);
      rgb.lightRed();

      delay(2000);

      beep.stopTone();
      rgb.shutDown();
    } else if (B.scoreRangeMin <= distance && distance <= B.scoreRangeMax) {
      //在B框范围
      data.addTeamScore('B', 1);
      beep.startTone(data.getTeam('B').scoredFrequency);
      rgb.lightRed();

      delay(2000);

      beep.stopTone();
      rgb.shutDown();
    }
  }
}
ScoreBorad::ScoreBorad()
  : connection() {
  instruction.device = '\0';
}
void ScoreBorad::setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);  //波特率9600 （Wifi通讯设定波特率）

  beep.init();
  fan.init();
  dht.init();
  rgb.init();
  doubleLed.init();
  lcd.init();
  data.init();
  ledMode = true;

  contestLong = 600000000;   //10分钟
  isInContest = false;
  contestMicro = 0;

  MsTimer2::set(250, SeriousEvent);
}
void ScoreBorad::loop() {
  //执行被动指令
  if (isInContest) {
    if (micros() - contestMicro >= contestLong) {
      contestFinished();
    }
  }
  refreshCondition();  //刷新环境
  detectScore();       //检测进球
  refreshScreen();

  //执行主动指令
  switch (instruction.device) {
    case 'B':  //蜂鸣器
      beep.run(instruction.message);
      Serial.print("Beep -> " + instruction.message + "\n");
      break;
    case 'F':  //风扇
      fan.run(instruction.message);
      Serial.print("Fan -> " + instruction.message + "\n");
      break;
    case 'R':  //RGB灯
      rgb.run(instruction.message);
      Serial.print("Rgb -> " + instruction.message + "\n");
      break;
    case 'D':  //双色LED
      doubleLed.run(instruction.message);
      Serial.print("DoubleLed -> " + instruction.message + "\n");
      break;
    case 'L':  //LCD显示屏
      lcd.run(instruction.message);
      Serial.print("LCDScreen -> " + instruction.message + "\n");
      break;
    case 'V':  //数据中心
      run(instruction.message);
      Serial.print("Data -> " + instruction.message + "\n");
      break;
    default:  //空指令或者非法指令
      break;
  }
  instruction.device = '\0';  //清空指令保障安全
}
void ScoreBorad::serialEvent() {
  connection.serialEvent();                   //触发事件
  instruction = connection.getInstruction();  //获取指令
}
void ScoreBorad::run(String message) {
  //解析-XXX-......
  switch (ReadStringMsg(message)[0]) {
    case 'A':                             //队伍加分
      {
        char Team = ReadStringMsg(message)[0];
        int Value = ReadIntMsg(message);
        data.addTeamScore(Team, Value);
        //提示音+提示灯
        beep.startTone(data.getTeam(Team).scoredFrequency);
        rgb.lightRed();

        delay(2000);

        beep.stopTone();
        rgb.shutDown();
      }
      break;
    case 'R':                             //队伍减分
      {
        char Team = ReadStringMsg(message)[0];
        int Value = ReadIntMsg(message);
        data.removeTeamScore(Team, Value);
        //提示灯
        rgb.lightGreen();

        delay(2000);

        rgb.shutDown();
      }
      break;
    case 'a':                             //队员加分（不会自动给队所在队伍加分）
      {
        char Team = ReadStringMsg(message)[0];
        char Member = ReadStringMsg(message)[0];
        int Value = ReadIntMsg(message);
        data.addTeamMemberScore(Team, Member, Value);
      }
      break;
    case 'r':                             //队员减分（不会自动给队所在队伍减分）
      {
        char Team = ReadStringMsg(message)[0];
        char Member = ReadStringMsg(message)[0];
        int Value = ReadIntMsg(message);
        data.removeTeamMemberScore(Team, Member, Value);
      }
      break;
    case 'P':                             //罚一次
      {
        char Team = ReadStringMsg(message)[0];
        char Member = ReadStringMsg(message)[0];
        data.punishMember(Team, Member);
      }
      break;
    case 'p':                             //撤罚
      {
        char Team = ReadStringMsg(message)[0];
        char Member = ReadStringMsg(message)[0];
        data.unpunishMember(Team, Member);
      }
      break;
    case 'F':                             //设置队伍得分音调
      {
        char Team = ReadStringMsg(message)[0];
        int Value = ReadIntMsg(message);
        data.setTeamFrequency(Team, Value);
      }
      break;
    case 'G':                             //设置队伍球筐范围
      {
        char Team = ReadStringMsg(message)[0];
        int From = ReadIntMsg(message);
        int To = ReadIntMsg(message);
        data.setTeamScoreRang(Team, From, To);
      }
      break;
    case 'N':                             //设置队伍名称
      {
        char Team = ReadStringMsg(message)[0];
        String Name = ReadStringMsg(message);
        data.setTeamName(Team, Name);
      }
      break;
    case '0':                             //重置
      {
        data.reset();
      }
      break;
    case '1':                             //重置分数
      {
        String Option = ReadStringMsg(message);
        if (Option == "Both"){
          data.resetTeamScore('A');
          data.resetTeamScore('B');
        } else {
          data.resetTeamScore(Option[0]);
        }    
        rgb.lightBlue();
        delay(2000);
        rgb.shutDown();
      }
      break;
    case '2':                             //开始比赛
      {
        contestBegin();
      }
      break;
    case '3':                             //结束比赛
      {
        contestFinished();
      }
      break;
    case '4':                             //设置比赛时长
      {
        int Value = ReadIntMsg(message);
        contestLong = Value * 60000000;
      }
      break;
    case '5':                             //切换报警模式
      {
        CurrentState::serious = false;
        ledMode = !ledMode;
        doubleLed.shutDown();
        fan.stopRoll();
      }
      break;
    default:
      break;
  }
/*
  if (Do == "ResetAll") {
    data.reset();
  } else if (Do == "ContestBegin") {
    contestBegin();
  } else if (Do == "SetContestLong") {
    contestLong = Value * 60000;
  } else if (Do == "ContestFinished") {
    contestFinished();
  } else if (Do == "SwitchLedMode") {
    CurrentState::serious = false;
    ledMode = !ledMode;
    doubleLed.shutDown();
    fan.stopRoll();
  } else if (Do == "Reset") {
    if (Team == "Both") {
      data.resetTeamScore('A');
      data.resetTeamScore('B');
    }
    else {
      data.resetTeamScore(Team[0]);
    }
    rgb.lightBlue();
    delay(2000);
    rgb.shutDown();
  } else if (Do == "Add") {
    data.addTeamScore(Team[0], Value);
    //得分提示
    {
      beep.startTone(data.getTeam(Team[0]).scoredFrequency);
      rgb.lightRed();

      delay(2000);

      beep.stopTone();
      rgb.shutDown();
    }
  } else if (Do == "Remove") {
    data.removeTeamScore(Team[0], Value);
    //减分提示
    {
      rgb.lightGreen();

      delay(2000);

      rgb.shutDown();
    }
  } else if (Do == "MAdd") {
    data.addTeamMemberScore(Team[0], Member[0], Value);
    //得分提示
    {
      beep.startTone(data.getTeam(Team[0]).scoredFrequency);
      rgb.lightRed();

      delay(2000);

      beep.stopTone();
      rgb.shutDown();
    }
  } else if (Do == "MRemove") {
    data.removeTeamMemberScore(Team[0], Member[0], Value);
    //减分提示
    {
      rgb.lightGreen();

      delay(2000);

      rgb.shutDown();
    }
  } else if (Do == "Punish") {
    data.punishMember(Team[0], Member[0]);
  } else if (Do == "Unpunish") {
    data.unpunishMember(Team[0], Member[0]);
  } else if (Do == "Frequency") {
    data.setTeamFrequency(Team[0], Value);
  } else if (Do == "Name") {
    data.setTeamName(Team[0], Name);
  } else if (Do == "Range") {
    data.setTeamScoreRang(Team[0], From, To);
  }
  */
}