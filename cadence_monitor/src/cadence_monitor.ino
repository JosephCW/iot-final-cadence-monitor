/*
 * Project cadence_monitor
 * Description: IOT Final Project Cadence Monitor
 * Author: Sammy Fields, Joseph Watts, David Small
 * Date: 11/17/2019
 */

int monitor = D0;
int startButton = A0;
int stopButton = A1;
int passes = 0;
int prevTime;
int curTime;
bool lastReadHigh = false;
bool curReadHigh = false;

void setup() {
  pinMode(monitor, INPUT);
  pinMode(startButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);

  Serial.begin(9600);

  Particle.function("start", start);
  Particle.function("stop", stop);
  
  Particle.subscribe("tapped", getCadence, MY_DEVICES);

}

void loop() {
  if(digitalRead(startButton)==LOW){
    Serial.print("Start Button Pressed");
    pubData();
  }
}

void pubData(){
  curTime = Time.now();
  while(digitalRead(stopButton)==HIGH){
    lastReadHigh = curReadHigh;
    curReadHigh = digitalRead(monitor);
    
    if (lastReadHigh && !curReadHigh) {
      passes++;
    }

    if (Time.now() - curTime >= 5){
      Serial.printf("{\"temperature\":%d, \"relative humidity\":%d}", Time.now(), passes);
      //Particle.publish("addReading", String::format("{\"currentTime\":%d, \"strokesSinceLastPublish\":%d}", Time.now(), passes));
      curTime = Time.now();
      passes = 0;
    }
  }
  Serial.print("Stop Button Pressed");
}

void start(){

}

void stop(){

}

void getCadence(String event, String data){
  Serial.printf("%s\n", data.c_str());
}