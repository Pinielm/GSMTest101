
#include <SoftwareSerial.h>


// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 2;
int RX = 6;
int TX = 7;

String message = 'a';


SoftwareSerial mySerial(RX, TX); // RX

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(RX,INPUT);
  pinMode(TX,OUTPUT);
  pinMode(led, OUTPUT);   
  mySerial.begin(9600);  
}

// the loop routine runs over and over again forever:
void loop() {

 mySerial.println("AT");              //When baud rate matches, Print AT on Serial Monitor
 delay(5000);

 //Condition to check Whether Serial finds Ok response from ESP8266 or not
 //If It finds Ok response the call the connectWifi() Function
 if (mySerial.available() >0) {
   
      message = mySerial.readString();


  } 
  
     if (message.indexOf("OK")>=0)
      {
      
         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(200);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(200);
         
         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(200);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(200);

         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(200);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(200);    

         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(200);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(200);
         
         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(200);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(200);

         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(200);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(200);
        
      } else{
                  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(2000);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(2000);
         
         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(2000);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(2000);

         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(2000);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(2000);    

         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(2000);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(2000);
         
         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(2000);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(2000);

         digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
         delay(2000);               // wait for a second
         digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
         delay(2000);
      }      

message ='a';


}
