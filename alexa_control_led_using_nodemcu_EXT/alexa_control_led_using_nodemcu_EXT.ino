/**********************************************************************************
* Home Automation with Alexa and NodeMCU - UPDATE
*
* Code based on original article by Marcelo Rovai:  
* Alexa - NodeMCU: WeMo Emulation Made Simple: 
* https://www.hackster.io/mjrobot/alexa-nodemcu-wemo-emulation-made-simple-c6c1f2
* 
* and updated from WeMo Emulation using FauxmoESP Library to Espalexa library: 
* https://github.com/Aircoookie/Espalexa
* 
* Espalexa is a library for ESP8266-based devices and ESP32 that emulates parts of 
* the SSDP protocol and the Philips hue API, just enough so it can be discovered 
* and controlled by Alexa-powered devices like the Amazon Echo or the Dot. 
*  
* 7 "devices" to be discovered:
*  
*  4 Simple IoT devices
*   ==> Red Light    (Light1)    ==> Relay 1 ==> NodeMCU D1 (GPIO5)  - Living Room 
*   ==> Blue Light   (Light2)    ==> Relay 3 ==> NodeMCU D5 (GPIO14) - Bed Room
*   ==> Green Light  (Outlet1)   ==> Relay 2 ==> NodeMCU D2 (GPIO4)  - Living Room
*   ==> Yellow Light (Outlet2)   ==> Relay 4 ==> NodeMCU D6 (GPIO12) - Bed Room
*  
*  3 Groups of devices
*   ==> All Home Devices 
*   ==> Living Room (Light1 and Outlet1 or Red/Green leds on testing phase)
*   ==> Bed Room (Light2 and Outlet2 or Blue/Yellow leds on testing phase)
*   
*  To discovery devices for the first time, use Alexa app on your smartphoneuse or 
*  a direct voice commnad:  "Alexa, discovery devices" (wait around 45 seconds)
*  
*  To activate a device or a group of devices (Device functions), you should use 
*  voice commands, like: 
*  "Alexa, turn on (off) Light1" or "..., turn on (off) Red Light" on testing phase
*  "Alexa, turn on Living Room" or "..., turn off Living Room"
*  "Alexa, turn on All Home Devices" or "..., turn off All Home Devices"
*  
*  For Device connected on Relay 1, a "dimmer" was also coded. so you can say:
*   "Alexa, turn on Red Light 50 percent" on testing phase and you will get proportional light
*
*  Developed by Marcelo Rovai on 29Dec20
*  Visit my blog: https://MJRoBot.org 
  *********************************************************************************/

#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>


// define the GPIO connected with Relays

#define RelayPin1 5  //D1
#define RelayPin2 4  //D2
#define RelayPin3 14 //D5
#define RelayPin4 12 //D6

//direct callback functions
void device1Changed(uint8_t brightness);
void device2Changed(uint8_t brightness);
void device3Changed(uint8_t brightness);
void device4Changed(uint8_t brightness);

//special callback functions
void devFunc1Changed(uint8_t brightness);
void devFunc2Changed(uint8_t brightness);
void devFunc3Changed(uint8_t brightness);

// device names
String Device_1_Name = "Red light";
String Device_2_Name = "Green light";
String Device_3_Name = "Blue light";
String Device_4_Name = "Yellow light";

// device_function names
String Dev_Func_1_Name = "Living Room";
String Dev_Func_2_Name = "Bed Room";
String Dev_Func_3_Name = "All Home Devices";

Espalexa espalexa;

// prototypes
boolean connectWifi();

// WiFi Credentials
const char* ssid = "YOUR SSID HERE";
const char* password = "YOUR PASSWORD HERE";

boolean wifiConnected = false;

void setup()
{
  Serial.begin(115200);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected)
  {
    // Define your devices here.
    espalexa.addDevice(Device_1_Name, device1Changed);
    espalexa.addDevice(Device_2_Name, device2Changed);
    espalexa.addDevice(Device_3_Name, device3Changed);
    espalexa.addDevice(Device_4_Name, device4Changed);

    // Define your devices functions here.
    espalexa.addDevice(Dev_Func_1_Name, devFunc1Changed);
    espalexa.addDevice(Dev_Func_2_Name, devFunc2Changed);
    espalexa.addDevice(Dev_Func_3_Name, devFunc3Changed);

    espalexa.begin();
  }
  else
  {
    while (1)
    {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}

void loop()
{
  espalexa.loop();
  delay(1);
}

//our callback functions
void device1Changed(uint8_t brightness){

  //Control the device
  if (brightness == 255)
    {
      digitalWrite(RelayPin1, HIGH);
      Serial.println("Device1 ON");
    }
  else if (brightness == 0)
  {
    digitalWrite(RelayPin1, LOW);
    Serial.println("Device1 OFF");
  }
  else
  {
    int brigh_perc = (brightness/255.)*100;
    analogWrite(RelayPin1, brightness);
    Serial.print("Device1 Brightness: ");
    Serial.print(brigh_perc);
    Serial.println("%");
    
  }
}

void device2Changed(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin2, HIGH);
      Serial.println("Device2 ON");
    }
  else
  {
    digitalWrite(RelayPin2, LOW);
    Serial.println("Device2 OFF");
  }
}

void device3Changed(uint8_t brightness)
{
  //Control the device  
  if (brightness == 255)
    {
      digitalWrite(RelayPin3, HIGH);
      Serial.println("Device3 ON");
    }
  else
  {
    digitalWrite(RelayPin3, LOW);
    Serial.println("Device3 OFF");
  }
}

void device4Changed(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin4, HIGH);
      Serial.println("Device4 ON");
    }
  else
  {
    digitalWrite(RelayPin4, LOW);
    Serial.println("Device4 OFF");
  }
}


void devFunc1Changed(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin1, HIGH);
      digitalWrite(RelayPin2, HIGH);
      Serial.println("Living Room ON");
    }
  else
  {
      digitalWrite(RelayPin1, LOW);
      digitalWrite(RelayPin2, LOW);
      Serial.println("Living Room OFF");
  }
}

void devFunc2Changed(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin3, HIGH);
      digitalWrite(RelayPin4, HIGH);
      Serial.println("Bed Room ON");
    }
  else
  {
      digitalWrite(RelayPin3, LOW);
      digitalWrite(RelayPin4, LOW);
      Serial.println("Bed Room OFF");
  }
}

void devFunc3Changed(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin1, HIGH);
      digitalWrite(RelayPin2, HIGH);
      digitalWrite(RelayPin3, HIGH);
      digitalWrite(RelayPin4, HIGH);
      Serial.println("All Home Devices ON");
    }
  else
  {
      digitalWrite(RelayPin1, LOW);
      digitalWrite(RelayPin2, LOW);
      digitalWrite(RelayPin3, LOW);
      digitalWrite(RelayPin4, LOW);
      Serial.println("All Home Devices OFF");
  }
}

// connect to wifi  returns true if successful or false if not
boolean connectWifi()
{
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
