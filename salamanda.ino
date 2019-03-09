/*
 * Written for use within the Arduino Uno board for use within the
 * Robogals' Arduino Hackathon 2019.
 */

#include <Servo.h>

// ultrasonic sweep angles
#define MIN_ANGLE 0;
#define MAX_ANGLE 180;

// maximum turn angles
#define MAX_TURN_LEFT 135;
#define MAX_TURN_RIGHT 45;

#define TURN_NONE = 0;
#define TURN_LEFT = 1;
#define TURN_RIGHT = 2;

// object detection
#define OBJ_FRONT 0;
#define OBJ_LEFT 1;
#define OBJ_RIGHT 2;

// speed settings
#define SPEED_STOP 0;
#define SPEED_LOW 1;
#define SPEED_HIGH 2;



// ultrasonic sensor sweep
long duration;
long distance;
long tracking[MAX_ANGLE]; //each cell is [angle, signal]



// arduino uno pinout settings
int ssPin = 8;
int triggerPin = 2;
int echoPin = 4;
int pwmMotor1 = 9;
int pwmMotor2 = 10;
int turnPin = 11;

// TODO
int pos = 0;
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


/*
 * TODO
 */
void sensorUpdate(int angle) {
  sendPulse(10);
  duration = getEchoDuration();
  distance = microsecondsToCentimeters(duration);
  lastDistance = distance;
  tracking[angle] = distance;
  lastAngle = pos;
  delay(75);
}

/*
 * Send the ultrasonic pulse over a specified microsecond (us) duration.
 */
void sendPulse(int usDuration)
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(usDuration);
  digitalWrite(triggerPin, LOW);
}


/*
 * TODO
 */
long getEchoDuration() {
  return pulseIn(echoPin, HIGH);
}

/*
 * Convert the microseconds returned from the ultrasonic sensor to distance in cm.
 */
long microsecondsToCenimeters(long microseconds) {
  return microseconds * 0.034 / 2;
}


/*
 * Find the location of the closest object in front of the vehicle at a 90 degree sweep angle.
 * The location finds whether the object is to the (offset) front-left or front-right of the vehicle.
 *
 * Note: This is done after the sweep has been performed.
 */
int findObject() {

  // set angle to zero
  int angle = 0;
  long smallestDistance = tracking[0];

  // get the smallest distance logged in the sweep array
  for (int i = 0; i < MAX_ANGLE; i++) {
    if (tracking[i] < smallestDistance) {
      smallestDistance = tracking[i];
      angle = i;
    }
  }

  // find the approximate object direction
  if (angle <= 180 && angle >= 95)
    return OBJ_LEFT;
  if (angle >= 0 && angle <= 85) {
    return OBJ_RIGHT;
  }
  return OBJ_FRONT;
}


/*
 * Turn the car in the desired direction. Either begin turning left, right, or not at all depending
 * on where the object was found to be. The servo angle for the vehicle turning is incrementally updated
 * as to avoid a "snap-to-heading" turn.
 */
int turnCar(int turn, int lastTurn) {

  // continue straight
  if (turn == TURN_NONE) {
    return lastTurn;
  }

  // turn left or right
  if (turn == TURN_LEFT && lastTurn < MAX_TURN_LEFT) {
    turnServo.write(++lastTurn); // left
  } else if (turn == TURN_RIGHT && lastTurn > MAX_TURN_RIGHT) {
    turnServo.write(--lastTurn); // right
  }

  // save for next iteration
  return lastTurn;
}


/*
 * Calculate the distance of the closest object in front of the car from the
 * sweep from the ultrasonic sensor.
 */
int calculateSpeed(long distance) {

  // stop car
  if (distance <= 5.0)
    return SPEED_STOP;

  // set speed low
  if (distance > 5.0 && distance <= 15.0)
    return SPEED_LOW;

  // set speed high
  if (distance > 15.0 && distance <= 30.0)
    return SPEED_HIGH;
}