
int led1 = 13; // test LED 
int led2 = 12; // test LED
int enable1 = 11; // motor 1 enable
int Input1A = 10; // motor 1 Input 1A
int Input1B = 9;  // motor 1 Input 2A
char buffArray[100]; // array to capture C++ read and select

String inputString = ""; // used to combine chars of different length
boolean fullStringRead = false;
char inChar; 
int i;

void setup() {
  pinMode(led1, OUTPUT);   // initialize LED for testing
  pinMode(led2, OUTPUT);   // initialize LED for testing
  pinMode(enable1, OUTPUT); // initialize pin for Enable
  pinMode(Input1A, OUTPUT); // initialize pin for Input 1A
  pinMode(Input1B, OUTPUT); // initialize pin for Input 2A
  Serial.begin(9600);  // initialize serial:
  
}

void setMotor(int speeds, boolean reversedir)
{
  analogWrite(enable1, speeds);
  digitalWrite(Input1A, !reversedir);
  digitalWrite(Input1B, reversedir);
  
}


void loop() {
  
      // test openCV
      
      for(int i = 0; i<10; i++){  // 15 is ideal
        char inChar = Serial.read();
        buffArray[i] = inChar;
      }
        
      
      char comp = buffArray[5];  // 13 is ideal
    
    // test manual 
  // char comp = Serial.read();
   
    if(comp == '1'){
      LED();
      Serial.write('a');
      }
      
    else if(comp == '2'){
      LEDL();
      Serial.write('b');
    }
      
    else if(comp == 'C'){
      
      Serial.write('c');}
    
    else if(comp == 'o'){
     // LED();
      Serial.write('D');
    }
  
 

}

void LED() {
 setMotor(255, true); // clockwise
//  digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);               // wait for a second
//  digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);               // wait for a second
  Serial.write('F');
}

void stop(){
  setMotor(0, true); // stop
}

void LEDL() {
  setMotor(255, false); // clockwise
 // digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
 // delay(1000);               // wait for a second
 // digitalWrite(led2, LOW);    // turn the LED off by making the voltage LOW
 // delay(1000);               // wait for a second
  Serial.write('F');

}
