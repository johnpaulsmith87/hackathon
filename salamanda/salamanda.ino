
#include <Servo.h>
#define MAX_ANGLE 180;
#define TURN_LEFT 135;
#define TURN_RIGHT 45;
long tracking[MAX_ANGLE];
//each cell is [angle, signal]
int pos = 0;
int ssPin = 8;
int triggerPin = 2;
int echoPin = 4;
int pwmMotor1 = 9;
int pwmMotor2 = 10;
int turnPin = 11;


long duration;
long distance;

int lastPos;
long lastAngle;

Servo turningServo;
Servo sensorServo;
void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 180; i++) {
    tracking[i] = 0l; //initialize to 0
  }
  sensorServo.attach(ssPin);  // attaches the servo on pin 9 to the servo object
  turningServo.attach(turnPin);
  
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

//takes as input tracking array
//outputs 0 for none, 1 for left, 2 for right
int findObject(){
  int angle = 0;
  long smallestDistance = tracking[0];
  for (int i = 0; i < MAX_ANGLE; i++) {
    if(tracking[i] < smallestDistance) {
      smallestDistance = tracking[i];
      angle = i;
    }
  }
  if (angle <= 180 && angle >= 95)
    return 1;
  if (angle >= 0 && angle <= 85) {
    return 2;
  }
  return 0;
}


int turnCar(int turn, int lastTurn) {
  if (!turn) {
    return lastTurn;
  }
  if (turn == 1 && lastTurn < LEFT_TURN) {
    turnServo.write(++lastTurn);
  } else if (turn == 2 && lastTurn > RIGHT_TURN) {
    turnServo.write(--lastTurn);
  }
  return lastTurn;
}


//checks distance
//returns 0 for stop, 1 for low speed and 2 for high
int calculateSpeed(long distance) {
  if (distance <= 5.0)
    return 0;
  if (distance > 5.0 && distance <= 15.0)
    return 1;
  if (distance > 15.0 && distance <= 30.0)
    return 2;
}
