#include <time.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

Servo xservo;
Servo yservo;

// Eastern
int timezone = -4;

char auth[] = "X";
char ssid[] = "X";
char pass[] = "X";

bool workHours(){
  
  // Get current hour
  time_t now = time(nullptr);
  int hour = (now / 3600) % 24;
  Serial.println("Current hour: " + hour);

  // Check that it is between 9am and 5pm
  if (hour > 8 && hour < 17){
    return true;
  } else {
    return false;
  }
}

// Laser on/off switch
BLYNK_WRITE(V0){

  // You can turn the laser off at any time of day
  if (param.asInt() == 0){
    digitalWrite(4, HIGH);
  }
  // You can only turn it on between 9am and 5pm EST
  else if (param.asInt() == 1){
    if (workHours() == 1){
      digitalWrite(4, LOW);
    } else {
      Serial.println("Laser unavailable at this time of day");
    }
  }
}

// Y Servo
BLYNK_WRITE(V1){
  if (workHours() == 1){
    digitalWrite(0, HIGH);
    yservo.write(param.asInt());
    delay(800);
    digitalWrite(0, LOW); 
  } else {
    Serial.println("Y servo unavailable at this time of day.");
  }
}

// X Servo
BLYNK_WRITE(V2){
  if (workHours() == 1){
    digitalWrite(2, HIGH);
    xservo.write(param.asInt());
    delay(800);
    digitalWrite(2, LOW); 
  } else {
    Serial.println("X servo unavailable at this time of day.");
  }
}

void setup(){ 
  // Debug output
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // Start Blynk
  Blynk.begin(auth, ssid, pass);

  // Initialize and silence pins
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  yservo.attach(0);
  xservo.attach(2);
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, HIGH);

  //Configure time
  configTime(timezone * 3600, 0, "pool.ntp.org", "time.nist.gov");

}

void loop() {
  Blynk.run();
}
