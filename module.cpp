
#include "module.h"
#include "decoder.h"


Beep::Beep(int pin)
  : PIN(pin) {
}
void Beep::init() {
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
}
void Beep::run(String command) {
  //解析-xxx-
  int Frequency = ReadIntMsg(command);
  
  //操作
  if(Frequency == 0) stopTone();
  else startTone(Frequency);
}
void Beep::startTone(unsigned int frequency) {
  NewTone(PIN, frequency);
}
void Beep::stopTone() {
  noNewTone(PIN);
  digitalWrite(PIN, HIGH);
}



Sonar::Sonar(int triggerPin, int echoPin)
  : NewPing(triggerPin, echoPin), TRIGGER_PIN(triggerPin), ECHO_PIN(echoPin) {
}
unsigned int Sonar::getDistanceCM() {
  return ping_cm();
}



Fan::Fan(int pin)
  : PIN(pin) {
}
void Fan::init() {
  pinMode(PIN, OUTPUT);
}
void Fan::run(String command) {
  //解析-xxx-
  int Speed = ReadIntMsg(command);
  
  //操作
  if(Speed == 0) stopRoll();
  else roll(Speed);
}
void Fan::roll(int speed) {
  analogWrite(PIN, speed);
}
void Fan::stopRoll() {
  analogWrite(PIN, 0);
}



Dht::Dht(int pin)
  : dht11(), PIN(pin) {
}
void Dht::init() {
  pinMode(PIN, OUTPUT);
}
void Dht::updateData() {
  read(PIN);
}



Rgb::Rgb(int rPin, int gPin, int bPin)
  : R_PIN(rPin), G_PIN(gPin), B_PIN(bPin) {
}
void Rgb::init() {
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  digitalWrite(R_PIN, LOW);
  digitalWrite(G_PIN, LOW);
  digitalWrite(B_PIN, LOW);
}
void Rgb::run(String command) {
  //解析-XXX-
  String Color = ReadStringMsg(command);
  
  //操作
  if (Color == "Red") {
    lightRed();
  } else if (Color == "Green"){
    lightGreen();
  } else if (Color == "Blue"){
    lightBlue();
  } else if (Color == "Null"){
    shutDown();
  }
}
void Rgb::lightRed() {
  shutDown();
  analogWrite(R_PIN, 255);
}
void Rgb::lightGreen() {
  shutDown();
  analogWrite(G_PIN, 255);
}
void Rgb::lightBlue() {
  shutDown();
  analogWrite(B_PIN, 255);
}
void Rgb::shutDown() {
  analogWrite(R_PIN, 0);
  analogWrite(G_PIN, 0);
  analogWrite(B_PIN, 0);
}



DoubleLed::DoubleLed(int rPin, int gPin)
  : R_PIN(rPin), G_PIN(gPin) {
}
void DoubleLed::init() {
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  digitalWrite(R_PIN, LOW);
  digitalWrite(G_PIN, LOW);
}
void DoubleLed::run(String command) {
  //解析-XXX-
  String Color = ReadStringMsg(command);
  
  //操作
  if (Color == "Red") {
    lightRed();
  } else if (Color == "Green"){
    lightGreen();
  } else if (Color == "Null"){
    shutDown();
  }
}
void DoubleLed::lightRed() {
  shutDown();
  digitalWrite(R_PIN, HIGH);
}
void DoubleLed::lightGreen() {
  shutDown();
  digitalWrite(G_PIN, HIGH);
}
void DoubleLed::shutDown() {
  digitalWrite(R_PIN, LOW);
  digitalWrite(G_PIN, LOW);
}



LCDScreen::LCDScreen(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
  : LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows) {
}
void LCDScreen::init() {
  LiquidCrystal_I2C::init();
  backlight();
  setCursor(0, 0);
  print("Hello");
  setCursor(0, 1);
  print("Score borad");
  delay(1000);

  setCursor(0, 0);
  print("                ");
  setCursor(0, 1);
  print("                ");
}
void LCDScreen::run(String command) {
  //解析-XXX-
  String Text = ReadStringMsg(command);
  
  //操作
  setCursor(0, 0);
  print("                ");
  setCursor(0, 0);
  print(Text);
  delay(2000);
}