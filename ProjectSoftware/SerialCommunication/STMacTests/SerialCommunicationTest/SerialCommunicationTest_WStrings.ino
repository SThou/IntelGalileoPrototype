

int led = 13; // test LED 
String inputString = ""; // used to combine chars of different length
boolean fullStringRead = false;
char inChar; 
int i;

void setup() {
  pinMode(led, OUTPUT);   // initialize LED for testing
  Serial.begin(9600);  // initialize serial:
  
}

void loop() {
     fullStringRead = false; // initial to false
     
   // Assuming serial is connected
   
      while(i<100){ // buffer size
        
        inChar = Serial.read(); // read in the byte
        if( inChar == '9') {
          Serial.write('S');
          fullStringRead = true; // break out the loop
          i = 101;
        } // end if
        else {
         inputString += inChar; // add the chars to a larger string
         Serial.write('T');
       //  i = i+1;
        } // end else
      } // end while
    
    
   // unsigned int inCharConvert = inChar-'0';
    if(inputString == "1"){
      Serial.write('a');
      }
      
    else if(inputString == "2"){
      Serial.write('b');}
      
    else if(inputString == "3"){
      Serial.write('c');}
    
    else if(inputString == "4"){
        digitalWrite(led, HIGH); 
        delay(1000);
        digitalWrite(led, LOW); 
         // turn the LED on (HIGH is the voltage level)
        Serial.write('L');}
    else Serial.write('W');
   
   
} // end loop

