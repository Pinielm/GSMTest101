

// Include Software Serial library to communicate with GSM
#include <SoftwareSerial.h>
uint8_t RX = 6;
uint8_t TX = 7;
uint8_t relay = 2;


// Configure software serial port
SoftwareSerial SIM900(RX, TX);

// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String lampState = "OFF";

// Relay connected to pin 12
//const int relay = 11;

void setup() {
  // Automatically turn on the shield
  //digitalWrite(9, HIGH);
  
  
  // Set relay as OUTPUT
  pinMode(relay, OUTPUT);

  // By default the relay is off

  // Initializing serial commmunication
  Serial.begin(115200); 
  SIM900.begin(9600);
  Serial.print("Starting...");

  // Give time to your GSM shield log on to network
  delay(10000);
  Serial.print("SIM900 ready...");

  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  SIM900.println("AT+CMGD=1,4");
  delay(1000);
  SIM900.println("AT+CMGDA= \"DEL ALL\"");
  delay(1000); 
  Serial.print("Setup Executed");
    digitalWrite(relay, HIGH);
  delay(2000);
  digitalWrite(relay, LOW);
}

void loop(){
  if(SIM900.available()>0){
    textMessage = SIM900.readString();
   // digitalWrite(relay, HIGH);
    //delay(2000);
    //digitalWrite(relay, LOW);
    Serial.print(textMessage);    
    delay(10);
  } 
  if(textMessage.indexOf("ON")>=0){
   
     digitalWrite(relay, HIGH);
   
    //digitalWrite(relay, HIGH);
    lampState = "on";
    Serial.println("Relay set to ON");  
    textMessage = "";   
  }
    if(textMessage.indexOf("?")>=0){
    // Turn on relay and save current state
    digitalWrite(relay, HIGH);
    lampState = "on";
    Serial.println("Relay set to ON");  
    textMessage = "";   
  }
  if(textMessage.indexOf("OFF")>=0){
    // Turn off relay and save current state   
    digitalWrite(relay, LOW);
    lampState = "off"; 
    Serial.println("Relay set to OFF");
    textMessage = ""; 
  }
  if(textMessage.indexOf("STATE")>=0){
    String message = "Lamp is " + lampState;
    sendSMS(message);
    Serial.println("Lamp state resquest");
    textMessage = "";
  }
}  

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);



  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  delay(100);


  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"+263775031781\"\r"); 
  delay(100);
  // Send the SMS
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.write(0x1A); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);  
}
