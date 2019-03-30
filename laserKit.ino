#include <time.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

Servo xservo;
Servo yservo;


char auth[] = "X";
char ssid[] = "X";
char pass[] = "X";

// Laser on/off switch
BLYNK_WRITE(V0){

  // Laser off
  if (param.asInt() == 0){
    digitalWrite(4, HIGH);
  }
  // Laser on
  else if (param.asInt() == 1){
     digitalWrite(4, LOW);
  }
}

// Y Servo
BLYNK_WRITE(V1){
    digitalWrite(0, HIGH);
    yservo.write(param.asInt());
    delay(800);
    digitalWrite(0, LOW); 
}

// X Servo
BLYNK_WRITE(V2){
    digitalWrite(2, HIGH);
    xservo.write(param.asInt());
    delay(800);
    digitalWrite(2, LOW); 
}

// Pattern A
BLYNK_WRITE(V3){
    int xpos, rand_delay, elapsed_sec;

    // Y Servo can be within range 127-175
    digitalWrite(0, HIGH);
    yservo.write(127);
    delay(800);
    digitalWrite(0, LOW);

    // X servo on, laser on
    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);

    // Only run for 2 minute. (Function always runs twice for some reason)
    time_t start = time(0);
    elapsed_sec = difftime( time(0), start);
    while( elapsed_sec <= 60 ) {
      // X Servo can be within range 100-142
      xpos = rand() % 43 + 100;
      xservo.write(xpos);

      // Delay between 500ms and 4000ms
      rand_delay = rand() % 4000 + 500;
      delay(rand_delay);

      // Update elapsed time
      elapsed_sec = difftime( time(0), start);
    }

    // X servo off, laser off
    delay(1000);
    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);
}

// Pattern B
BLYNK_WRITE(V4){
    // X Servo can be within range 100-142
    // Y Servo can be within range 127-175 (restricted to 140)
    int elapsed_sec;
    int y_op = -1, x_op = 1;
    int xpos = rand() % 43 + 100;
    int ypos = rand() % 10 + 127;

    // X and Y servo on, laser on
    digitalWrite(0, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);

    // Only run for 2 minute. (Function always runs twice for some reason)
    time_t start = time(0);
    elapsed_sec = difftime( time(0), start);

    while( elapsed_sec <= 60 ) {
      // Does the movement, goes all the way to one direction and reverses
      if( xpos <= 100 ){x_op = 1;} else if( xpos >= 142 ){x_op = -1;}
      if( ypos <= 127 ){y_op = 1;} else if( ypos >= 150 ){y_op = -1;}
      xpos = xpos + x_op;
      ypos = ypos + y_op;

      xservo.write(xpos);
      yservo.write(ypos);
      delay(15);
      // Update elapsed time
      elapsed_sec = difftime( time(0), start);
    }

    // X and Y servo off, laser off
    delay(1000);
    digitalWrite(0, LOW);
    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);
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

}

void loop() {
  Blynk.run();
}
