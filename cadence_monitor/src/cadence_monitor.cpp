/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "application.h"
#line 1 "c:/Users/josep/Documents/IoT/iot-final-cadence-monitor/cadence_monitor/src/cadence_monitor.ino"
/*
 * Project cadence_monitor
 * Description: IOT Final Project Cadence Monitor
 * Author: Sammy Fields, Joseph Watts, David Small
 * Date: 11/17/2019
 */

void setup();
void loop();
void setLedBasedOnCadence(int currentCadence);
#line 8 "c:/Users/josep/Documents/IoT/iot-final-cadence-monitor/cadence_monitor/src/cadence_monitor.ino"
int monitor = D0;
int startButton = A0;
int stopButton = A1;
int passes = 0;
int startTime;
int lastPublishTime;
int curTime;
bool lastReadHigh = false;
bool curReadHigh = false;
bool activeReading = false;
int btnLastPressTime = 0;


int exampleCadence[] = {25, 50, 55, 60, 60, 67, 70, 69, 65, 60, 60};
int currentExampleCadence = 0;

int TARGET_CADENCE = 60;
int B_LED = D1;
int G_LED = D2;
int R_LED = D3;

void setup() {
  pinMode(monitor, INPUT);
  pinMode(startButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);

  pinMode(B_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);

  Serial.begin(9600);

  // Particle.function("startNode", start);
  // Particle.function("stopNode", stop);
  
  // Particle.subscribe("getCadence", getCadence, MY_DEVICES);
}

void loop() {
  if(digitalRead(startButton)==LOW && Time.now() > btnLastPressTime + 1) {
    btnLastPressTime = Time.now();
    Serial.println("Start Button Pressed");
    activeReading = true;
    startTime = Time.now();
    lastPublishTime = Time.now();
  }

  if (activeReading) {
    curTime = Time.now();

    lastReadHigh = curReadHigh;
    curReadHigh = digitalRead(monitor);

    if (lastReadHigh && !curReadHigh) {
      passes++;
    }

    if (curTime > lastPublishTime + 3){
      Serial.printf("{\"currentTime\":%d, \"strokesSinceLastPublish\":%d}\n", curTime, passes);
      //Particle.publish("addReading", String::format("{\"currentTime\":%d, \"strokesSinceLastPublish\":%d}", curTime, passes));
      
      // Imitating the subscribe to the cloud function call.
      // REMOVE LATER
      Serial.printf("CURRENT EXAMPLE CADENCE: %d\n", exampleCadence[currentExampleCadence]);
      setLedBasedOnCadence(exampleCadence[currentExampleCadence]);
      currentExampleCadence++;
      currentExampleCadence %= 11;
      
      lastPublishTime = Time.now();
      passes = 0;
    }    
  }

  if (digitalRead(stopButton)==LOW && Time.now() > btnLastPressTime + 1) {
    btnLastPressTime = Time.now();
    Serial.println("Stop Button Pressed");
    activeReading = false;
  }
}

void setLedBasedOnCadence(int currentCadence) {
  // Set all LED's to off
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, LOW);
  digitalWrite(B_LED, LOW);

  if (currentCadence > TARGET_CADENCE + 4) {
    digitalWrite(R_LED, HIGH);
  } else if (currentCadence < TARGET_CADENCE - 4) {
    digitalWrite(B_LED, HIGH);
  } else {
    digitalWrite(G_LED, HIGH);
  }
}

// void start(){

// }

// void stop(){

// }

// int getCadence(String event, String data){
//   Serial.printf("%s\n", data.c_str());
//   return 0;
// }