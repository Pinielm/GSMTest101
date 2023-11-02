/*
 * Complete Project Details https://randomnerdtutorials.com
 */

// Include Software Serial library to communicate with GSM
#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial A9G(6, 7);

// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String lampState = "HIGH";

// Relay connected to pin 12
const int relay = 5;

void setup() {

  // Set relay as OUTPUT
  pinMode(relay, OUTPUT);

  // By default the relay is off
  digitalWrite(relay, HIGH);
  
  // Initializing serial commmunication
  //Serial.begin(9600); 
  A9G.begin(9600);

  // Give time to your GSM shield log on to network
  delay(20000);
  //Serial.print("SIM900 ready...");

  // AT command to set SIM900 to SMS mode
  A9G.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  A9G.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}

void loop(){
  
  
  if(A9G.available()>0){
    textMessage = A9G.readString();
       
    delay(10);
  } 
  if(textMessage.indexOf("ON")>=0){
    // Turn on relay and save current state
    digitalWrite(relay, LOW);
    lampState = "on";
     
    textMessage = "";   
  }
  if(textMessage.indexOf("OFF")>=0){
    // Turn off relay and save current state
    digitalWrite(relay, HIGH);
    lampState = "off"; 
    
    textMessage = ""; 
  }
  if(textMessage.indexOf("STATE")>=0){
    String message = "Lamp is " + lampState;
    sendSMS(message);
    
    textMessage = "";
  }
}  

// Function that sends SMS
void sendSMS(String message){
  // AT command to set A9G to SMS mode
  A9G.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  A9G.println("AT + CMGS = \"XXXXXXXXXX\""); 
  delay(100);
  // Send the SMS
  A9G.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  A9G.println((char)26); 
  delay(100);
  A9G.println();
  // Give module time to send SMS
  delay(5000);  
}
