/*
Motor control file for Intel Galileo 
Pins available for analogWrite (3, 5 , 6 10, 11)

Last update: Tuesday October 28, 2014 11:13 PM ST
*/

int motorPin = 7;
 
void setup() 
{ 
  pinMode(motorPin, OUTPUT); // change GPIO to output
} 
 
 
void loop() 
{ 

      analogWrite(motorPin, -100); // half speed
    

} 
