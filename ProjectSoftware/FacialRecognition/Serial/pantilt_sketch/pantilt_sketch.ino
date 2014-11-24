/*
#include <Servo.h>

// Title:   Auto Pan-Tilt Servo/Cam Control
// Subject: This Sketch receives X,Y coordinates from srial then 
//          moves the camera to center of those coordinates. 
// Remixed: TechBitar / Hazim Bitar
// Date:    Dec 2011
// Credit:  I based this sketch on zagGrad's (SparkFun) code.  

#define  servomaxx   180   // max degree servo horizontal (x) can turn
#define  servomaxy   180   // max degree servo vertical (y) can turn
#define  screenmaxx   320   // max screen horizontal (x)resolution
#define  screenmaxy   240    // max screen vertical (y) resolution
#define  servocenterx   90  // center po#define  of x servo
#define  servocentery   90  // center po#define  of y servo
#define  servopinx   9   // digital pin for servo x
#define  servopiny   10  // digital servo for pin y
#define  baudrate 115200  // com port speed. Must match your C++ setting
#define distancex 1  // x servo rotation steps
#define distancey 1  // y servo rotation steps

int prediction;
int valx = 0;       // store x data from serial port
int valy = 0;       // store y data from serial port
int posx = 0;
int posy = 0;
int incx = 10;  // significant increments of horizontal (x) camera movement
int incy = 10;  // significant increments of vertical (y) camera movement
boolean continue_program = false;

Servo servox;
Servo servoy;

short MSB = 0;  // to build  2 byte integer from serial in byte
short LSB = 0;  // to build  2 byte integer from serial in byte
int   MSBLSB = 0;  //to build  2 byte integer from serial in byte

void setup() {

  Serial.begin(baudrate);        // connect to the serial port
  Serial.println("Starting Cam-servo Face tracker");

  pinMode(servopinx,OUTPUT);    // declare the LED's pin as output
  pinMode(servopiny,OUTPUT);    // declare the LED's pin as output

  servoy.attach(servopiny); 
  servox.attach(servopinx); 

  // center servos

  servox.write(servocenterx); 
  delay(200);
  servoy.write(servocentery); 
  delay(200);
}

void loop () {
  while(Serial.available() <=0); // wait for incoming serial data
  
  if(Serial.available() > 0 && Serial.available() < 4 && continue_program == false)
  {
    if(continue_program == false) // initial setup of the prediction label
    {
     MSB = Serial.read();
     delay(5);
     LSB = Serial.read();
     MSBLSB = word(MSB,LSB);
     prediction = MSBLSB;
     delay(5);
     Serial.print("Enter prediction label");
     prediction = Serial.read() - '0';
     Serial.print("Received prediction label");
     continue_program = true;
    }
  }
  
  if (Serial.available() >= 4)  // wait for 4 bytes. 
  {
    
    //if(continue_program == true)
    
    {
    //get current prediction from serial
    MSB = Serial.read();
    delay(5);
    LSB = Serial.read();
    MSBLSB = word(MSB,LSB);
    delay(5);
    if(MSBLSB == prediction)
    {
      
    // get X axis 2-byte integer from serial
    MSB = Serial.read();
    delay(5);
    LSB = Serial.read();
    MSBLSB=word(MSB, LSB);
    valx = MSBLSB; 
    delay(5);

    // get Y axis 2-byte integer from serial
    MSB = Serial.read();
    delay(5);
    LSB = Serial.read();
    MSBLSB=word(MSB, LSB);
    valy = MSBLSB; 
    delay(5);

    // read last servos positions
    //posx = servox.read(); 
    //posy = servoy.read();

    //Find out if the X component of the face is to the left of the middle of the screen.
    if(valx < (screenmaxx/2 - incx)){
      if( posx >= incx )
      {
      posx += distancex; //Update the pan position variable to move the servo to the left.
      Serial.print("panning left");
    }
    //Find out if the X component of the face is to the right of the middle of the screen.
    else if(valx > screenmaxx/2 + incx){
      if(posx <= servomaxx-incx){
      posx -=distancex; //Update the pan position variable to move the servo to the right.
      Serial.print("panning right");
      }
    }

    //Find out if the Y component of the face is below the middle of the screen.
    if(valy < (screenmaxy/2 - incy)){
      if(posy >= 5){
      posy += distancey; //If it is below the middle of the screen, update the tilt position variable to lower the tilt servo.
      Serial.print("tilting down");
      }
    }
    //Find out if the Y component of the face is above the middle of the screen.
    else if(valy > (screenmaxy/2 + incy)){
      if(posy <= 175){
      posy -= distancey; //Update the tilt position variable to raise the tilt servo.
      Serial.print("tilting up");
      }
    }
    // Servos will rotate accordingly 
    servox.write(posx);
    servoy.write(posy);
    }
    else
    {
      delay(20); // skipping the X and Y
    // Servos will rotate accordingly 
    servox.write(posx);
    servoy.write(posy);
    }
    }
  }   
}
}
*/

#include <Servo.h>

// Title:   Auto Pan-Tilt Servo/Cam Control
// Subject: This Sketch receives X,Y coordinates from srial then 
//          moves the camera to center of those coordinates. 
// Remixed: TechBitar / Hazim Bitar
// Date:    Dec 2011
// Credit:  I based this sketch on zagGrad's (SparkFun) code.  

#define  servomaxx   180   // max degree servo horizontal (x) can turn
#define  servomaxy   180   // max degree servo vertical (y) can turn
#define  screenmaxx   400   // max screen horizontal (x)resolution
#define  screenmaxy   400    // max screen vertical (y) resolution
#define  servocenterx   90  // center po#define  of x servo
#define  servocentery   90  // center po#define  of y servo
#define  servopinx   9   // digital pin for servo x
#define  servopiny   10  // digital servo for pin y
#define  baudrate 9600  // com port speed. Must match your C++ setting
#define distancex 1  // x servo rotation steps
#define distancey 1  // y servo rotation steps

int valx = 0;       // store x data from serial port
int valy = 0;       // store y data from serial port
int posx = 0;
int posy = 0;
int incx = 10;  // significant increments of horizontal (x) camera movement
int incy = 10;  // significant increments of vertical (y) camera movement
int led = 12;
//char incomingByte;
//String incomingString;

Servo servox;
Servo servoy;

short MSB = 0;  // to build  2 byte integer from serial in byte
short LSB = 0;  // to build  2 byte integer from serial in byte
int   MSBLSB = 0;  //to build  2 byte integer from serial in byte

void setup() {

  Serial.begin(baudrate);        // connect to the serial port
  Serial.println("Starting Cam-servo Face tracker");

  pinMode(servopinx,OUTPUT);    // declare the LED's pin as output
  pinMode(servopiny,OUTPUT);    // declare the LED's pin as output
  pinMode(led,OUTPUT);

  servoy.attach(servopiny); 
  servox.attach(servopinx); 

  // center servos

  servox.write(servocenterx); 
  delay(200);
  servoy.write(servocentery); 
  delay(200);
}

void loop () {
  if (Serial.available() > 0)  // wait for 4 bytes. 
  {
    /*
    Serial.println("Serial > 0");
    incomingByte = Serial.read();
    //delay(5);
    while (incomingByte != '\n') {
      Serial.println("inside char loop");
      incomingString += incomingByte;
      incomingByte = Serial.read();
      //delay(5);
    }
    Serial.print(incomingString.length());
    Serial.print("\t"); Serial.println(incomingString);
    incomingString = "";
    */
    
    Serial.println("Entered >= 2");
    // get X axis 2-byte integer from serial
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    MSB = Serial.read();
    delay(5);
    LSB = Serial.read();
    MSBLSB=word(MSB, LSB);
    valx = MSBLSB; 
    delay(5);
    Serial.println(valx);

    // get Y axis 2-byte integer from serial
    MSB = Serial.read();
    delay(5);
    LSB = Serial.read();
    MSBLSB=word(MSB, LSB);
    valy = MSBLSB; 
    delay(5);
    Serial.println(valy);

    // read last servos positions
    posx = servox.read(); 
    posy = servoy.read();
    
    //Find out if the X component of the face is to the left of the middle of the screen.
    if(valx < (screenmaxx/2 - incx)){
      Serial.println("panning left");
      if( posx >= incx )
      {
      posx += distancex; //Update the pan position variable to move the servo to the left.    
    }
    }

    //Find out if the X component of the face is to the right of the middle of the screen.
    if(valx > screenmaxx/2 + incx){
      Serial.println("panning right");
      if(posx <= servomaxx-incx){
      posx -=distancex; //Update the pan position variable to move the servo to the right.
      }
    }

    //Find out if the Y component of the face is below the middle of the screen.
    if(valy < (screenmaxy/2 - incy)){
      Serial.println("tilting down");
      if(posy >= 5){
      posy += distancey; //If it is below the middle of the screen, update the tilt position variable to lower the tilt servo.
      }
    }
    //Find out if the Y component of the face is above the middle of the screen.
     if(valy > (screenmaxy/2 + incy)){
      Serial.println("tilting up");
      if(posy <= 175){
      posy -= distancey; //Update the tilt position variable to raise the tilt servo.
      }
    }

    // Servos will rotate accordingly 
    servox.write(posx);
    servoy.write(posy);
  }   
}






























