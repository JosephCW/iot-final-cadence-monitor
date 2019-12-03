/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "application.h"
#line 1 "c:/Users/s519653/Documents/IoT/Projects/iot-final-cadence-monitor/cadence_monitor/src/cadence_monitor.ino"
/*
 * Project cadence_monitor
 * Description: IOT Final Project Cadence Monitor
 * Author: Sammy Fields, Joseph Watts, David Small
 * Date: 11/17/2019
 */

void setup();
void loop();
void turnOffLeds();
void setLedBasedOnCadence(int currentCadence);
void handleCadence(const char *event, const char *data);
void handleStartRide(const char *event, const char *data);
#line 8 "c:/Users/s519653/Documents/IoT/Projects/iot-final-cadence-monitor/cadence_monitor/src/cadence_monitor.ino"
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

int currentCadence = 0;
int rideId = -1;
int TARGET_CADENCE = 60;
int B_LED = D1;
int G_LED = D2;
int R_LED = D3;

void setup() {
  // Subscribe to the following things
  // Callback on when we call the get cadence page
  Particle.subscribe("hook-response/getCadence/0", handleCadence, MY_DEVICES);
  // Callback to get the rideId when starting
  Particle.subscribe("hook-response/startRide/0", handleStartRide, MY_DEVICES);
  // a

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
    // Publish a cloud request that will allow us to get our rideId from the server
    Particle.publish("startRide", PRIVATE);
  }

  // It is set to reading and the device got a rideId from the server
  if (activeReading) {
    curTime = Time.now();

    lastReadHigh = curReadHigh;
    curReadHigh = digitalRead(monitor);

    if (lastReadHigh && !curReadHigh) {
      passes++;
    }

    if (curTime > lastPublishTime + 4){
      Serial.printf("{\"currentTime\":%d, \"strokesSinceLastPublish\":%d, \"rideId\": %d}", curTime, passes, rideId);
      Particle.publish("addReading", String::format("{\"currentTime\":%d, \"strokesSinceLastPublish\":%d, \"rideId\": %d}", curTime, passes, rideId));
      Particle.publish("getCadence", String::format("{\"rideId\": %d}", rideId), PRIVATE);

      lastPublishTime = Time.now();
      passes = 0;
    }    
  }

  if (digitalRead(stopButton)==LOW && Time.now() > btnLastPressTime + 1) {
    btnLastPressTime = Time.now();
    Serial.println("Stop Button Pressed");
    // Tell the nodejs server which ride to stop.
    // Server will use its current time to log the ride end.
    Particle.publish("stopRide", String::format("{\"rideId\": %d}", rideId), PRIVATE);
    turnOffLeds();
    activeReading = false;
  }
}

void turnOffLeds() {
  // Set all LED's to off
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, LOW);
  digitalWrite(B_LED, LOW);
}

void setLedBasedOnCadence(int currentCadence) {
  turnOffLeds();
  if (currentCadence > TARGET_CADENCE + 6) {
    digitalWrite(R_LED, HIGH);
  } else if (currentCadence < TARGET_CADENCE - 6) {
    digitalWrite(B_LED, HIGH);
  } else {
    digitalWrite(G_LED, HIGH);
  }
}

void handleCadence(const char *event, const char *data) {
  currentCadence = String(data).toInt();
  Serial.printf("Parsed Cadence: %d\n", currentCadence);
  setLedBasedOnCadence(currentCadence);
}

void handleStartRide(const char *event, const char *data) {
  rideId = String(data).toInt();
  Serial.printf("Ride Id: %d\n", rideId);
  // Set the device to start reading, and last publisht time to now
  activeReading = true;
  lastPublishTime = Time.now();
}
