/*
 * Written for use within the Arduino Uno board for use within the
 * Robogals' Arduino Hackathon 2019.
 */

#include <Servo.h>

// ultrasonic sweep angles
#define MIN_ANGLE 20
#define MAX_ANGLE 160

// maximum turn angles
#define MAX_TURN_LEFT 135
#define MAX_TURN_RIGHT 45

#define TURN_NONE 0
#define TURN_LEFT 1
#define TURN_RIGHT 2

// object detection
#define OBJ_FRONT 0
#define OBJ_LEFT 1
#define OBJ_RIGHT 2

// speed settings
#define SPEED_STOP 0
#define SPEED_LOW 1
#define SPEED_HIGH 2

#define ANALOG_SPEED_HIGH 255
#define ANALOG_SPEED_LOW 200
#define ANALOG_SPEED_STOP 0

// ultrasonic sensor sweep
long duration;
long lastDistance;
long tracking[MAX_ANGLE]; //each cell is [angle, signal]

// arduino uno pinout settings
int ssPin = 8;
int triggerPin = 2;
int echoPin = 4;
int pwmMotor1 = 9;
int pwmMotor2 = 10;
int turnPin = 11;

int front1 = 12;
int front2 = 13;

int back1 = 7;
int back2 = 6;

// TODO
int lastPos;
long lastAngle;

int turnTrack = 90;

Servo turningServo;
Servo sensorServo;
void setup()
{
  // put your setup code here, to run once:
  for (int i = MIN_ANGLE; i < MAX_ANGLE; i++)
  {
    tracking[i] = 0l; //initialize to 0
  }
  sensorServo.attach(ssPin); // attaches the servo on pin 9 to the servo object
  turningServo.attach(turnPin);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pwmMotor1, OUTPUT);
  pinMode(pwmMotor2, OUTPUT);
  //pinMode(testPin, OUTPUT);
  pinMode(front1, OUTPUT);
  pinMode(front2, OUTPUT);
  pinMode(back1, OUTPUT);
  pinMode(back2, OUTPUT);
  analogWrite(pwmMotor1, ANALOG_SPEED_HIGH);
  analogWrite(pwmMotor2, ANALOG_SPEED_HIGH);
  //analogWrite(testPin, ANALOG_SPEED_HIGH);

  digitalWrite(front1, HIGH);
  digitalWrite(front2, LOW);

  digitalWrite(back1, HIGH);
  digitalWrite(back2, LOW);
  Serial.begin(9600);
}

void loop()
{
  int dir;

  // put your main code here, to run repeatedly:
  //analogWrite(testPin, ANALOG_SPEED_HIGH);
  sweepRight();
  dir = findObject();
  turnTrack = turnCar(dir, turnTrack);
  engageEngines();
  sweepLeft();
  dir = findObject();
  turnTrack = turnCar(dir, turnTrack);
  engageEngines();
}

void sweepLeft()
{
  int pos;
  for (pos = MIN_ANGLE; pos < MAX_ANGLE; pos++)
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    sensorServo.write(pos); // tell servo to go to position in variable 'pos'
    sensorUpdate(pos);
    //engageEngines();// waits 15ms for the servo to reach the position
  }
}

void sweepRight()
{
  int pos;
  for (pos = MAX_ANGLE; pos > MIN_ANGLE; pos--)
  {                         // goes from 180 degrees to 0 degrees
    sensorServo.write(pos); // tell servo to go to position in variable 'pos'
    sensorUpdate(pos);
    //engageEngines();// waits 15ms for the servo to reach the position
  }
}

/*
 * TODO
 */
void sensorUpdate(int angle)
{
  sendPulse(10);
  duration = getEchoDuration();
  long distance = microsecondsToCentimeters(duration);
  lastDistance = distance;
  tracking[angle] = distance;
  lastAngle = angle;
  char buf[50];
  sprintf(buf, "{\"distance\": %ld, \"angle\": %ld}\n", distance, angle);
  Serial.print(buf);
//  Serial.printf("{\"distance\": %ld,\"angle\": %ld}\n", distance, angle);
  delay(8);
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
long getEchoDuration()
{
  return pulseIn(echoPin, HIGH);
}

/*
 * Convert the microseconds returned from the ultrasonic sensor to distance in cm.
 */
long microsecondsToCentimeters(long microseconds)
{
  return microseconds * 0.034 / 2;
}

/*
 * Find the location of the closest object in front of the vehicle at a 90 degree sweep angle.
 * The location finds whether the object is to the (offset) front-left or front-right of the vehicle.
 *
 * Note: This is done after the sweep has been performed.
 */
int findObject()
{

  // set angle to zero
  int angle = 0;
  long smallestDistance = tracking[0];

  // get the smallest distance logged in the sweep array
  for (int i = 0; i < MAX_ANGLE; i++)
  {
    if (tracking[i] < smallestDistance)
    {
      smallestDistance = tracking[i];
      angle = i;
    }
  }

  // find the approximate object direction
  if (angle <= 180 && angle >= 95)
    return OBJ_LEFT;
  if (angle >= 0 && angle <= 85)
  {
    return OBJ_RIGHT;
  }
  return OBJ_FRONT;
}

/*
 * Turn the car in the desired direction. Either begin turning left, right, or not at all depending
 * on where the object was found to be. The servo angle for the vehicle turning is incrementally updated
 * as to avoid a "snap-to-heading" turn.
 */
int turnCar(int turn, int lastTurn)
{

  // continue straight
  if (turn == TURN_NONE)
  {
    return lastTurn;
  }

  // turn left or right
  if (turn == TURN_LEFT && lastTurn < MAX_TURN_LEFT)
  {
    turningServo.write(++lastTurn); // left
  }
  else if (turn == TURN_RIGHT && lastTurn > MAX_TURN_RIGHT)
  {
    turningServo.write(--lastTurn); // right
  }

  // save for next iteration
  return lastTurn;
}

/*
 * Calculate the distance of the closest object in front of the car from the
 * sweep from the ultrasonic sensor.
 */
int calculateSpeed(long distance)
{
  // stop car
  if (distance <= 5.0)
    return SPEED_STOP;

  // set speed low
  if (distance > 5.0 && distance <= 15.0)
    return SPEED_LOW;

  // set speed high
  if (distance > 15.0)
    return SPEED_HIGH;
}

void engageEngines()
{
  int result = calculateSpeed(lastDistance);

  analogWrite(pwmMotor1, ANALOG_SPEED_HIGH);
  analogWrite(pwmMotor2, ANALOG_SPEED_HIGH);
  /*if (result == SPEED_HIGH)
  {
    Serial.print("Engines HIGH\n");
    analogWrite(pwmMotor1, ANALOG_SPEED_HIGH);
    analogWrite(pwmMotor2, ANALOG_SPEED_HIGH);
  }
  else if (result == SPEED_LOW)
  {
    Serial.print("Engines LOW\n");
    analogWrite(pwmMotor1, ANALOG_SPEED_LOW);
    analogWrite(pwmMotor2, ANALOG_SPEED_LOW);
  }
  else {
    Serial.print("Engines OFF\n");
    analogWrite(pwmMotor1, ANALOG_SPEED_STOP);
    analogWrite(pwmMotor2, ANALOG_SPEED_STOP);
  } */
 

}
