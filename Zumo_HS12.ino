//Pin config
const int buttonPin = 2; 
//Sharp340
const int RD0_Pin =  0;//0 
const int RD1_Pin =  4;//4
const int RD2_Pin =  5;//5 
const int RD3_Pin =  6;//6
const int RD4_Pin =  7;//7 
const int RD5_Pin =  8;//8
const int RD6_Pin =  9;//9 
const int RD7_Pin =  10;//10

//Motor
const int M1_Dir_Pin =  12; 
const int M1_PWM_Pin =  3; 
const int M2_Dir_Pin =  13; 
const int M2_PWM_Pin =  11;



byte RD0, RD1, RD2, RD3, RD4, RD5, RD6, RD7;//Robot detection
byte RDM; //Robot detection Matrix
byte First_RDM=0xFF; //Robot detection Matrix, one loop before
int buttonState = 0; //Start buttun
int goPressed = 0;    //button pressed once


void left_f(){
    digitalWrite(M1_Dir_Pin, HIGH);   
    digitalWrite(M1_PWM_Pin, HIGH);    
    
    digitalWrite(M2_Dir_Pin, LOW);   
    digitalWrite(M2_PWM_Pin, LOW);   
}
void rigth_f(){
    digitalWrite(M1_Dir_Pin, LOW);   
    digitalWrite(M1_PWM_Pin, LOW);   
    
    digitalWrite(M2_Dir_Pin, HIGH);   
    digitalWrite(M2_PWM_Pin, HIGH);    
}


void left(){
    digitalWrite(M1_Dir_Pin, HIGH);   
    digitalWrite(M1_PWM_Pin, HIGH);   
    
    digitalWrite(M2_Dir_Pin, LOW);   
    digitalWrite(M2_PWM_Pin, HIGH);   
}
void rigth(){
    digitalWrite(M1_Dir_Pin, LOW);   
    digitalWrite(M1_PWM_Pin, HIGH);   
    
    digitalWrite(M2_Dir_Pin, HIGH);   
    digitalWrite(M2_PWM_Pin, HIGH);
}

void forward(){
    digitalWrite(M1_Dir_Pin, HIGH);   
    digitalWrite(M1_PWM_Pin, HIGH);   
    
    digitalWrite(M2_Dir_Pin, HIGH);   
    digitalWrite(M2_PWM_Pin, HIGH);
}

void back(){
    digitalWrite(M1_Dir_Pin, LOW);   
    digitalWrite(M1_PWM_Pin, HIGH);   
    
    digitalWrite(M2_Dir_Pin, LOW);   
    digitalWrite(M2_PWM_Pin, HIGH);
}
void stopBot(){
    digitalWrite(M1_Dir_Pin, LOW);   
    digitalWrite(M1_PWM_Pin, LOW);   
    
    digitalWrite(M2_Dir_Pin, LOW);   
    digitalWrite(M2_PWM_Pin, LOW);
}

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
Serial.print(", RD2:");
Serial.print(RD2);
Serial.print(", RD3:");
Serial.print(RD3);
Serial.print(", RD4:");
Serial.print(RD4);
Serial.print(", RD5:");
Serial.print(RD5);
Serial.print(", RD6:");
Serial.print(RD6);
Serial.print(", RD7:");
Serial.print(RD7);
Serial.print(" = ");
Serial.println(RDM, BIN);
};

void SharpRead()
{
RD0=digitalRead(RD0_Pin);//0 
RD1=digitalRead(RD1_Pin);//4 
RD2=digitalRead(RD2_Pin);//5 
RD3=digitalRead(RD3_Pin);//5 
RD4=digitalRead(RD4_Pin);//5 
RD5=digitalRead(RD5_Pin);//5 
RD6=digitalRead(RD6_Pin);//5 
RD7=digitalRead(RD7_Pin);//5 
  RDM=0xFF;
  RDM=RDM<<1|RD7;
  RDM=RDM<<1|RD6;
//  RDM=RDM<<1|RD5;
  RDM=RDM<<1|1;
//  RDM=RDM<<1|RD4;
  RDM=RDM<<1|1;
  RDM=RDM<<1|RD3;
  RDM=RDM<<1|RD2;
  RDM=RDM<<1|RD1;
  RDM=RDM<<1|RD0;
  RDM=~RDM;
};

void setup() {                
  Serial.begin(9600);
//---Motors init
  pinMode(M1_Dir_Pin, OUTPUT);  //M2   
  pinMode(M1_PWM_Pin, OUTPUT);     

  pinMode(M2_Dir_Pin, OUTPUT); //M1    
  pinMode(M2_PWM_Pin, OUTPUT);     
  stopBot();
//---StartButton
  pinMode(buttonPin, INPUT); 
//Sharp340
  pinMode(RD0_Pin, INPUT);//
  pinMode(RD1_Pin, INPUT);//
  pinMode(RD2_Pin, INPUT);// 
  pinMode(RD3_Pin, INPUT);// 
  pinMode(RD4_Pin, INPUT);// 
  pinMode(RD5_Pin, INPUT);// 
  pinMode(RD6_Pin, INPUT);// 
  pinMode(RD7_Pin, INPUT);// 
  
Serial.println("Init Complete.");
}

void loop() {
  
buttonState = digitalRead(buttonPin);
SharpRead();
SharpPrint();

if ((buttonState == HIGH) or (goPressed == HIGH)) //Start then button presed
{     
  if ((goPressed != HIGH)){
    Serial.println(millis());
    StartTimer5Sec(4700);//Delay into msec after start
    Serial.println(millis());
    goPressed = 1; //set to High after first press    
    left();
  };

if (First_RDM != RDM)//If no changes into RDM, do nothing
  {
    First_RDM = RDM;
    //MAin case
    switch(RDM){
    //Go forward
    case B00000011:
      forward();
    Serial.println("Forward");
    break;
    //Go rotate left
    case B00100000:
    case B01000000:
    case B10000000:
    case B01100000:
    case B11000000:
    case B10000001:
    case B11000001:    
      left();
      Serial.println("Left");
    break;
    //Go rotate right
    case B00000100:
    case B00001000:
    case B00010000:    
    case B00000110:
    case B00001100:
    case B00011000:
    case B00001110:
      rigth();
      Serial.println("Right");
    break;
    case B10000011:
    case B00000001:
      left_f();
    Serial.println("Forward Left");
    break;
    case B00000010:
    case B00000111:
      rigth_f();
    Serial.println("Forward Right");
    break;
    case B00000000:
    delay(50);
    Serial.println("delay");
    break;
    default:
      left();
      Serial.println("Find");
    };
  };
};

}
