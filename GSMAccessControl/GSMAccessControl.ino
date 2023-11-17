

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

bool sendCommand(String command, String expectedResponse) {

  //clear buffer
  while (SIM900.available() > 0) {
      SIM900.read();
  }
  
  SIM900.println(command);
  delay(1000);

  String response = "";
  while(SIM900.available() > 0) { // Read the response
    response = SIM900.readString();
    if(response.indexOf(expectedResponse) >= 0) {
      return true;
    }
    delay(10);
  }

  Serial.print("Trouble executing the command: ");
  Serial.println(command);
  return false; // Expected response not found
}

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
  while (!sendCommand("AT", "OK")) {
    Serial.println("Error initialising module. Trying again");
    delay(1000);
  }

  Serial.print("SIM900 ready...");

  // AT command to set SIM900 to SMS mode
  while (!sendCommand("AT+CMGF=1", "OK")) {
    Serial.println("Error setting sms mode. Trying again");
    delay(1000);
  }
  delay(100);
  // Set module to send SMS data to serial out upon receipt
  while (!sendCommand("AT+CNMI=2,2,0,0,0", "OK")) {
    Serial.println("Error setting odule to send SMS data to serial out upon receipt. Trying again");
    delay(1000);
  }
  delay(100);
  while (!sendCommand("AT+CMGD=1,4", "OK")) {
    Serial.println("Error deleting smses. Trying again");
    delay(1000);
  }

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
