#include <Pushbutton.h>
#include <ZumoMotors.h>

#define HS13A
//#define HS13

//--- Pin config ---
// LED 
#define LED 13
//Sharp340
#ifdef HS13A
#define RD0_Pin 4 
#define RD1_Pin 5
#endif 

#ifdef HS13
#define RD0_Pin A4 
#define RD1_Pin 5
#endif 

//Remote Controll(StertModule)
#define RC_Start 2
#define RC_Kill 11
//--- 

//rotation direction
#define RIGHT 0
#define LEFT 1

// Speed: 0 is stopped, 400 is full speed, -400 reverce 
#ifdef HS13
#define FULL_SPEED        400
#define REVERSE_SPEED     200 
#define TURN_SPEED        300
#define FORWARD_SPEED     300
#define STOP              0
#endif 

#ifdef HS13A
#define FULL_SPEED        400
#define REVERSE_SPEED     200 
#define TURN_SPEED        250
#define FORWARD_SPEED     250
#define STOP              0
#endif 

// Time definitions in ms
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms

//Variables 
byte RD0, RD1;           //Robot detection vars
byte RDM;                //Robot detection Matrix
byte First_RDM=B10101010;//Robot detection Matrix, loop before
byte buttonState = 0;    //Start buttun
byte RC_Start_State = 0; //Start Module satart state
byte RC_Stop_State = 0;  //Kill Module satart state 
byte goPressed = 0;      //button pressed once
byte goStart = 0;        //started once
byte rotation = LEFT;    //defult rotation to left

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

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
RD0=digitalRead(RD0_Pin);
RD1=digitalRead(RD1_Pin); 
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
  pinMode(RC_Start, INPUT);
  pinMode(RC_Kill, INPUT);
    
  digitalWrite(LED, LOW);
  Serial.begin(115200);
  Serial.println("rotation = LEFT");
}

void loop()
{
SharpRead();
// SharpPrint(); //debug output of Sharp reading
  
buttonState = button.isPressed();
if ((goPressed != HIGH) and (buttonState == HIGH))
  {
    goPressed = 1; //set to High after first press    
    rotation = RIGHT;
    Serial.println("rotation = RIGHT");
  };
digitalWrite(LED, rotation);

RC_Start_State = digitalRead(RC_Start);
RC_Stop_State = digitalRead(RC_Kill);
if (RC_Stop_State == LOW)//Start then RC_Start pin HIGH, Kill pin LOW
{
   motors.setSpeeds( STOP, STOP);
   RC_Start_State = LOW;
   Serial.println("Command = Kill");
};

if (RC_Start_State == HIGH)//Start then RC_Start pin HIGH, Kill pin LOW
{     
  if (goStart != HIGH) //Never start before
    {
    Serial.println("Command = Start");
    goStart = 1; //set HIGH after first init  
    switch(rotation){
       case LEFT: motors.setSpeeds(-TURN_SPEED, TURN_SPEED); break;
       case RIGHT: motors.setSpeeds(TURN_SPEED, -TURN_SPEED);break;  
      };
    };
    
if (First_RDM != RDM)//If no changes into RDM, do nothing
  {
    First_RDM = RDM;//Save current RDM
    switch(RDM){//Main case, as FSM serch current state of RDM
    case B00000011://Go forward
      motors.setSpeeds(FULL_SPEED, FULL_SPEED); Serial.println("Forward");
    break;
    case B00000001://Go rotate left
      motors.setSpeeds(FORWARD_SPEED, FULL_SPEED); Serial.println("Left");
      rotation=LEFT;   
    break;
    case B00000010://Go rotate right
      motors.setSpeeds(FULL_SPEED, FORWARD_SPEED); Serial.println("Right");
      rotation=RIGHT;
    break;
    default:
      switch(rotation){
       case LEFT: motors.setSpeeds(-TURN_SPEED, TURN_SPEED); break;
       case RIGHT: motors.setSpeeds(TURN_SPEED, -TURN_SPEED);break;  
      };
    };
    Serial.println("Find");
  };
};
}
