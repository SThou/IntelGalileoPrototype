int enablePin = 6;
  int motorPin1 = 3;
  int motorPin2 = 5;
  int counter = 0;
  char mot_en; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motorPin1, OUTPUT); // change GPIO to output
  pinMode(motorPin2, OUTPUT); // change GPIO to output
  pinMode(enablePin, OUTPUT); // change GPIO to output
}

void loop() {
  // put your main code here, to run repeatedly: 
   if(counter <= 180)
   {
     setMotor(255,true);
     delay(3);
     counter++;
   }
   else
   halt();
}



void halt()
{ 
  if(Serial.available() > 0)
  {
  mot_en = Serial.read();
 if(mot_en == 'c')
  {
   counter = 0;
  } 
  }
  setMotor(0,true);
 reverse(); 
}


void reverse()
{  if(Serial.available() > 0)
  {
  mot_en = Serial.read();
 if(mot_en == 'r')
  {
    while (counter != 90){
   counter--;
   setMotor(255, false);
   delay(3);
  } 
  }
 
}
}

void setMotor(int speeds, boolean reversedir)
{
  analogWrite(enablePin, speeds);
  digitalWrite(motorPin1, !reversedir);
  digitalWrite(motorPin2, reversedir);
  
}

/*
void Startposition(){
 // Center using endstop 
  
}

void Endstops( ) {
 // if switch is hit on left, motor switched off.
  // stay or move left

 // if switch is hot on right, motor switched off. 
   // stay or move right 
   
  
  
}

*/
