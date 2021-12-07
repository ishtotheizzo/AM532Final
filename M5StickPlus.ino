#include <M5StickCPlus.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
WebSocketsClient webSocket;
WiFiClient client;


/*
 * WIFI BEGIN CODES 
 *     WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_DISCONNECTED     = 6
 */




/*
 * Code References: 
 * 
 */

//Set up Wifi 

const char* ssid = "wifi";  
const char* pass= "ThisIsASecurePasswordDjes?"; 
String ip = "000.000.000.000";

// get gyro data 
float gyroX, gyroY, gyroZ;

// get acceleration data 

float accX, accY, accZ;

// get altitude data 

float pitch, roll, yaw;


// string containing dictionary response for gyro data  
String gyro;  

//string containing dictionary response for accel data 
String accel; 

//string containing altitude data 

String alt; 

// String to send a bang to Max to show that M5 button was pressed 

String buttonA; 

// send everything to max!!! 

String toMax; 


float lastConnectionTime; 

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
  const uint8_t* src = (const uint8_t*) mem;
  Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for(uint32_t i = 0; i < len; i++) {
    if(i % cols == 0) {
      Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    Serial.printf("%02X ", *src);
    src++;
  }
  Serial.printf("\n");
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);

      // send message to server when Connected
      webSocket.sendTXT("Connected");
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);

      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}



void setup() {

  // Initalize M5 stick 
  M5.begin(true, true, true);
  M5.IMU.Init();

  // begin connecting to WiFi 

  WiFi.begin(ssid, pass); 
  delay(1000); 

  // connect to Wifi

  Serial.println(WiFi.status()); 

  //check if connected 

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Not connected. Trying again"); 
    delay(1000); 
    WiFi.begin(ssid, pass); 
    Serial.println(WiFi.status());
      M5.Lcd.setCursor(10,10); 
  M5.Lcd.printf("Wifi not connected "); 



    
  }
  


  M5.Lcd.setCursor(10,10); 
  M5.Lcd.printf("Wifi Connected");
    webSocket.begin(ip, 3000, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  // webSocket.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);

    webSocket.sendTXT(String(millis()).c_str());




}

void loop() {

  // start websocket and start M5 update 
  webSocket.loop();
  M5.update(); // need to call update()
  
  // get IMU data 
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
  M5.IMU.getAccelData(&accX, &accY, &accZ);  
   M5.IMU.getAhrsData(&pitch, &roll, &yaw);

   
  M5.Lcd.setCursor(0, 30);

  //Dictionary Passed to Max 
  gyro = " \"x\" : "+ String((gyroX*M5.IMU.gRes),2)+ " \"y\" : "+ String((gyroY*M5.IMU.gRes),2)+ " \"z\" : " + String((gyroZ*M5.IMU.gRes),2);

  //Alt data passed to Max 

  alt = " \"pitch\" : "+ String(pitch,2)+ " \"roll\" : "+ String(roll,2)+ " \"yaw\" : " + String(yaw,2);

  if (M5.BtnA.read() == 1) {
    M5.Lcd.printf("A button is pressed.");
    buttonA = "\"buttonA\": 1"; 
  } 

  else
  {
    buttonA = "\"buttonA\": 0";
  }

  //Dictionary being passed to Max 

  float acceleration = sqrt(((accX)*(accX))+((accY)*(accY))+((accZ )*(accZ))); 
  float vector = sqrt(((gyroX*M5.IMU.gRes)*(gyroX*M5.IMU.gRes))+((gyroY*M5.IMU.gRes)*(M5.IMU.gRes*gyroY))+((M5.IMU.gRes*gyroZ)*(M5.IMU.gRes*gyroZ)));

  
  accel = "\"accelX\" :"+String((accX), 2)+" \"accelY\" :"+String((accY),2) +" \"accelZ\" :"+String((accZ),2);
  Serial.println(accX);

  toMax = accel + gyro + alt + " " + buttonA+ " "+ "\"acceleration\" : "+ String(acceleration,2) + " \"vector\" :  " + String(vector,2);



  // pass data to web socket 
   webSocket.sendTXT(toMax.c_str());

   delay(100);

  
}
