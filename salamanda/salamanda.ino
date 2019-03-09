
#include <Servo.h>
#define MAX_ANGLE 180;
long tracking[MAX_ANGLE];
//each cell is [angle, signal]
int pos = 0;
int ssPin = 8;
int triggerPin = 2;
int echoPin = 4;
int pwmMotor1 = 9;
int pwmMotor2 = 10;



long duration;
long distance;

int lastPos;
long lastAngle;

Servo sensorServo;
void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 180; i++) {
    tracking[i] = 0l; //initialize to 0
  }
  sensorServo.attach(ssPin);  // attaches the servo on pin 9 to the servo object
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  for (pos = 0; pos <= MAX_ANGLE; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    sensorServo.write(pos);              // tell servo to go to position in variable 'pos'
    sensorUpdate(pos);                      // waits 15ms for the servo to reach the position
  }
  for (pos = MAX_ANGLE; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    sensorServo.write(pos);              // tell servo to go to position in variable 'pos'
    sensorUpdate(pos);                     // waits 15ms for the servo to reach the position
  }
}


void sensorUpdate(int angle) {
  sendPulse(10);
  duration = getEchoDuration();
  distance = microsecondsToCentimeters(duration);
  lastDistance = distance;
  tracking[angle] = distance;
  lastAngle = pos;
  delay(75);
}

void sendPulse(int usDuration)
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(usDuration);
  digitalWrite(triggerPin, LOW);
}

long getEchoDuration() {
  return pulseIn(echoPin, HIGH);
}

long microsecondsToCenimeters(long microseconds) {
  return microseconds * 0.034 / 2;
}
