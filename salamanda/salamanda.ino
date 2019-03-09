
#include <Servo.h>
long tracking[180];
//each cell is [angle, signal]
int pos = 0;  
int servoSweep = 9;

int triggerPin = 2;
int echoPin = 4;

long duration;
long distance;
Servo myservo;
void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 180; i++) {
    tracking[i] = 0; //initialize to 0
  }
    myservo.attach(servoSweep);  // attaches the servo on pin 9 to the servo object
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);

    //Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    sensorUpdate(pos);                      // waits 15ms for the servo to reach the position
    
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    sensorUpdate(pos);                     // waits 15ms for the servo to reach the position
  }
}


void sensorUpdate(int angle){
  sendPulse(10);
  duration = getEchoDuration();
  distance = microsecondsToCentimeters(duration);
  tracking[angle] = distance;
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
  return microseconds*0.034/2;
}
