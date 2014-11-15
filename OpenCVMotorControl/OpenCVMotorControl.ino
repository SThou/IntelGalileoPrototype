/*
Motor control file for Intel Galileo 
Pins available for analogWrite (3, 5 , 6 10, 11)

set true value to clockwise & false to counter clockwise

Last update: Tuesday October 29, 2014 12:29 PM ST
*/


int enablePin = 6; 
int motorPin1 = 3;
int motorPin2 = 5;


void setup() 
{ 
  pinMode(motorPin1, OUTPUT); // change GPIO to output
  pinMode(motorPin2, OUTPUT); // change GPIO to output
  pinMode(enablePin, OUTPUT); // change GPIO to output
} 
 
 
void loop() 
{ 
      

    while(1){
    setMotor(255, true);
   // delay(1000);
    //setMotor(100,0);
    
    }
    
    

} 


void setMotor(int speeds, boolean reversedir)
{
  analogWrite(enablePin, speeds);
  digitalWrite(motorPin1, !reversedir);
  digitalWrite(motorPin2, reversedir);
  
}
