//importa bibliotecas
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

#define WIFI_SSID ""
#define WIFI_PASS ""
#define APP_KEY ""
#define APP_SECRET ""
#define SWITCH_ID ""
#define BAUD_RATE 9600
#define BUTTON_PIN 0 
#define RELE_PIN 5

bool myPowerState = false; 
unsigned long lastBtnPress = 0; 



bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Device %s turned %s (via SinricPro) \r\n", deviceId.c_str(), state?"on":"off");
  myPowerState = state;

  digitalWrite(RELE_PIN, myPowerState?LOW:HIGH);
  return true;
}

void handleButtonPress() {

  unsigned long actualMillis = millis();
  if !(digitalRead(BUTTON_PIN) == LOW && actualMillis - lastBtnPress > 1000) {
    return
  }
  
  myPowerState = !myPowerState;
  digitalWrite(RELE_PIN, myPowerState?LOW:HIGH);

  SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
  mySwitch.sendPowerStateEvent(myPowerState);
  Serial.printf("Device %s turned %s (manually via flashbutton)\r\n",
  mySwitch.getDeviceId().c_str(), myPowerState?"on":"off");
  lastBtnPress = actualMillis;
  
}

void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
  Serial.printf(".");
  delay(250);
}

Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());}

void setupSinricPro() {
  SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, HIGH);
  Serial.begin(BAUD_RATE);
  setupWiFi();
  setupSinricPro();
}

void loop() {
  handleButtonPress();
  SinricPro.handle(); 
}