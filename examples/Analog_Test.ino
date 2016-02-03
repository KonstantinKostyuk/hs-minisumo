
// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin0 = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPin1 = A1;  // Analog input pin that the potentiometer is attached to

int sensorValue0 = 0;        // value read from the pot
int sensorValue1 = 0;        // value read from the pot

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  pinMode(analogInPin0, INPUT);
  pinMode(analogInPin1, INPUT);  
}

void loop() {
  // read the analog in value:
  sensorValue0 = analogRead(analogInPin0);            
  sensorValue1 = analogRead(analogInPin1);            

  // print the results to the serial monitor:
  Serial.print("A0 = " );                       
  Serial.print(sensorValue0);      
  Serial.print("\t A1 = ");      
  Serial.println(sensorValue1);   

  delay(500);                     
}
