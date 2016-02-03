#include <Pushbutton.h>
#include <ZumoMotors.h>

 
//Pin config
#define LED 13
//Sharp340
#define RD0_Pin A4 
#define RD1_Pin 5
  
// these might need to be tuned for different motor types
#define FULL_SPEED        400
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms

//Variables 
byte RD0, RD1;//Robot detection
byte RDM; //Robot detection Matrix
byte First_RDM=0xFF; //Robot detection Matrix, one loop before
int buttonState = 0; //Start buttun
int goPressed = 0;    //button pressed once
byte First_loop = 0;

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

void StartTimer5Sec(int time_ms)
{
  long int StartTime = millis();
  long int CurTime = millis();
  while(CurTime - StartTime < time_ms){
  CurTime = millis();};
};

void SharpPrint()
{
Serial.print("RD0:");
Serial.print(RD0);
Serial.print(", RD1:");
Serial.print(RD1);
Serial.print(" = ");
Serial.println(RDM, BIN);
};

void SharpRead()
{
RD0=digitalRead(RD0_Pin);//0 
RD1=digitalRead(RD1_Pin);//4 
  RDM=0xFF;
  RDM=RDM<<1|1;
  RDM=RDM<<1|1;
  RDM=RDM<<1|1;
  RDM=RDM<<1|1;
  RDM=RDM<<1|1;
  RDM=RDM<<1|1;
  RDM=RDM<<1|RD1;
  RDM=RDM<<1|RD0;
  RDM=~RDM;
};

 
void setup()
{   
  pinMode(LED, OUTPUT);
  pinMode(RD0_Pin, INPUT);
  pinMode(RD1_Pin, INPUT);
  
  digitalWrite(LED, LOW);
  Serial.begin(115200);
}

void loop()
{
buttonState = button.isPressed();

if ((buttonState == HIGH) or (goPressed == HIGH)) //Start then button presed
{     
  if ((goPressed != HIGH)){
    digitalWrite(LED, HIGH);
    Serial.println(millis());
    StartTimer5Sec(4700);//Delay into msec after start
    Serial.println(millis());
    goPressed = 1; //set to High after first press    
    digitalWrite(LED, LOW);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
  };

SharpRead();
SharpPrint();

if (First_RDM != RDM)//If no changes into RDM, do nothing
  {
    First_RDM = RDM;
    //MAin case
    switch(RDM){
    //Go forward
    case B00000011:
      motors.setSpeeds(FULL_SPEED, FULL_SPEED);
      Serial.println("Forward");
    break;
    //Go rotate left
    case B00000001:
      motors.setSpeeds(FORWARD_SPEED, FULL_SPEED);
      Serial.println("Left");
    break;
    //Go rotate right
    case B00000010:
      motors.setSpeeds(FULL_SPEED, FORWARD_SPEED);
      Serial.println("Right");
    break;
//    case B00000000:
//    delay(50);
//    Serial.println("delay");
//    break;
    default:
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      Serial.println("Find");
    };
  };
};
}
