
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>



const byte rxPin = D3;
const byte txPin = D4;

// Set up a new SoftwareSerial object
SoftwareSerial SIM900(rxPin, txPin);

const char* ssid = "RUGARE MEDICAL AID";
const char* password = "rugarezzz";

String textMessage;
const char* serverName = "your server addres here";

unsigned long previousMillis = 0;



bool sendCommand(String command, String expectedResponse, bool printlnMode = true) {

  //clear buffer
  while (SIM900.available() > 0) {
    SIM900.read();
  }

  if (printlnMode) {
    SIM900.println(command);
  } else {
    SIM900.print(command);
  }
  delay(1000);

  String response = "";
  while (SIM900.available() > 0) {  // Read the response
    response = SIM900.readString();
    Serial.println(response);
    if (response.indexOf(expectedResponse) >= 0) {
      return true;
    }
    delay(10);
  }

  Serial.print("Trouble executing the command: ");
  Serial.println(command);
  return false;  // Expected response not found
}



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  SIM900.begin(9600);

  while (!sendCommand("AT", "OK")) {
    Serial.println("Error initialising module. Trying again");
    delay(1000);
  }

  Serial.print("SIM900 ready...");

  // AT command to set SIM900 to SMS mode
  while (!sendCommand("AT+CMGF=1\r", "OK", false)) {
    Serial.println("Error setting sms mode. Trying again");
    delay(1000);
  }
  delay(100);
  // Set module to send SMS data to serial out upon receipt
  while (!sendCommand("AT+CNMI=2,2,0,0,0\r", "OK", false)) {
    Serial.println("Error setting odule to send SMS data to serial out upon receipt. Trying again");
    delay(1000);
  }
  delay(100);
  while (!sendCommand("AT+CMGD=1,4", "OK")) {
    Serial.println("Error deleting smses. Trying again");
    delay(1000);
  }

  Serial.print("Setup Executed");
}

void loop() {
  // put your main code here, to run repeatedly:

  if (SIM900.available() > 0) {
    textMessage = SIM900.readString();
    Serial.print(textMessage);
    delay(10);
  }
  if (textMessage.indexOf("STATE") >= 0) {

    //digitalWrite(relay, HIGH);
    PostData(textMessage, serverName);
  }


  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 10000) {
    previousMillis = millis(); 

    String jsonData =  httpGETRequest(serverName);    

    StaticJsonDocument<200> doc;
    
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, jsonData);

    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    String number = doc["number"];
    String message = doc["message"];    
    
    SendMessage(number, message);
    
  }
}

String httpGETRequest(const char* ServerName) {
  WiFiClient client;
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(client, ServerName);


  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();

        
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void PostData(String Data, String ServerName) {
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, ServerName);

  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

  // Specify content-type header
  http.addHeader("Content-Type", "application/json");

  // Send HTTP POST request

  int httpResponseCode = http.POST(Data);






  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  // Free resources
  http.end();  
}

void SendMessage (String Message, String Number ){
    SIM900.print("AT+CMGF=1\r");
    delay(1000);
    SIM900.print("AT+CNMI=2,2,0,0,0\r");
    delay(1000);
    String command = "AT+CMGS=\"" + Number + "\"\r";
    // mySerial.print("AT+CMGS=\"+263788617067\"\r");
    SIM900.print(command);
    delay(1000);
    SIM900.print(Message);
    SIM900.write(0x1A);
  } 

