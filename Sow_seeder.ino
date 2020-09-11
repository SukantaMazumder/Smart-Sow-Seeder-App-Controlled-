// for motor
#define R_R_En 24
#define R_L_En 25
#define R_R_PWM 6 //forward PWM
#define R_L_PWM 7 //reverse PWM

//defining L9110 pins for controlling Linear Actuators
const int IN1 = 4;
const int IN2 = 5;

//defining ultrasensor pins
const int trigPin = 31;
const int echoPin = 33;

long duration = 0;
int distance = 0;



int offset = 0;
int back = 0;

//defining servo motor
#include <Servo.h>
Servo seeding;
int pos = 0;

void setup()
{
  pinMode(R_R_En, OUTPUT);
  pinMode(R_L_En, OUTPUT);
  pinMode(R_R_PWM, OUTPUT);
  pinMode(R_L_PWM, OUTPUT);


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
 
  seeding.attach(9);
  seeding.write(0);
  Serial.begin(9600);
}


void forward( int a)
{

  digitalWrite(R_R_En, HIGH);
  digitalWrite(R_L_En, HIGH);
  analogWrite(R_R_PWM, a );
  analogWrite(R_L_PWM, 0 );
}

void backward( int a)
{

  digitalWrite(R_R_En, HIGH);
  digitalWrite(R_L_En, HIGH);
  analogWrite(R_R_PWM, 0 );
  analogWrite(R_L_PWM, a );
}
void stop()
{
  digitalWrite(R_R_En, LOW);
  digitalWrite(R_L_En, LOW);
  analogWrite(R_R_PWM, 0 );
  analogWrite(R_L_PWM, 0 );
}

void distance_calc()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
}

void loop()
{
  seeding.write(0);
  distance_calc(); //calculating distance
  Serial.println(distance);
  if (Serial.available() > 0)
  {
    offset = Serial.read();
  }
  if (Serial.available() > 0  && Serial.available() != offset)
  {
    back = Serial.read();
  }
  if (distance > 25)
  {
    forward(255); //change the speed according to your comfort
    delay(offset * 50);
    stop();
    delay(3000);
    digitalWrite(IN1, HIGH); //actuator will extend
    digitalWrite(IN2, LOW);
    delay(back * 100);
    digitalWrite(IN1, LOW); //actuator will retract
    digitalWrite(IN2, HIGH);
    delay(back * 100); //calibrate it according to your need
    if(offset!=0)
    { 
      for (pos = 0; pos < 90 ; pos++) //change the rotation in the place of 90
      {
        seeding.write(pos);
        delay(10);
      }
      for (pos = 90; pos >= 0 ; pos--)
      {
        seeding.write(pos);
        delay(10);
      }
    }
    else
    {
       for(pos = 0; pos < 0 ; pos--)
       {
         seeding.write(pos);
         delay(10);
       }
    
    }
    

  if (distance <= 25)
  {
     stop();
  }
  }
}
