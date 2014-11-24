
int led = 13; // test LED 



void setup() {
  pinMode(led, OUTPUT);   // initialize LED for testing
  Serial.begin(9600);  // initialize serial:
  
}

void loop() {
  
 // if (Serial.available() ) {
    
    while(1){
    char inChar = Serial.read();
    if(inChar == 'A'){
      Serial.write('a');
      }
      
    else if(inChar == 'B'){
      Serial.write('b');}
      
    else if(inChar == 'C'){
      Serial.write('c');}
    
    else if(inChar == 'o'){
      LED();
    }
    
    else{

    Serial.write("Nothing");  
    inChar = (char)Serial.read();
    
  }
  
//  } // end if
} // end loop
}


void LED() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}

