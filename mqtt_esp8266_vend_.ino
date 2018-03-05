/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "//put local SSID"
#define WLAN_PASS       "put your wifi password"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "put your adafruit username"
#define AIO_KEY         "put your adafruit key"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish confirm = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/confirm");                    //get the data from the particular node 
Adafruit_MQTT_Publish sensor_p = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/u sensor");
Adafruit_MQTT_Publish controllers_p = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/controllers");
Adafruit_MQTT_Publish breadboard_p = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/breadboard");
Adafruit_MQTT_Publish resistorbox_p = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/resistor box");
// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe usensor = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/u sensor");            // get the data from the particular node   
Adafruit_MQTT_Subscribe controller = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/controllers");
Adafruit_MQTT_Subscribe breadboard= Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/breadboard");
Adafruit_MQTT_Subscribe resistorbox = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/resistor box");
Adafruit_MQTT_Subscribe result = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/confirm");
/*************************** Sketch Code ************************************/
int con=0;
// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
void setup() {
  Serial.begin(9600);
  delay(10);

  //Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  //Serial.println(); Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println();

  //Serial.println("WiFi connected");
  //Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&usensor);  mqtt.subscribe(&resistorbox);  mqtt.subscribe(&breadboard);
  mqtt.subscribe(&result);
mqtt.subscribe(&controller);
  confirm.publish(0);
  
}
int bboard=0,rbox=0,cont=0,sensor=0;
uint32_t x=0;

void loop() 
{
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
     if (subscription == &result)
     {
       con= atoi((char *)result.lastread);
    
     }
    
    if (subscription == &usensor) 
    {

        sensor = atoi((char *)usensor.lastread);
 
    }
        if (subscription == &controller) {

        cont = atoi((char *)controller.lastread);
     

    }
        if (subscription == &breadboard) 
    {

        bboard = atoi((char *)breadboard.lastread);
   
    }
        if (subscription == &resistorbox) {

       rbox = atoi((char *)resistorbox.lastread);
     
              }
}

if(con==1)
{
//  Serial.println("confirmed the final result is: "+"rbox:"+String(rbox)+"bborad:"+String(bboard)+"controller:"+String(cont)+"sensor:"+String(sensor));
     delay(200);
     Serial.print("a"+String(rbox)+"b");     Serial.print("c"+String(bboard)+"d");     Serial.print("e"+String(cont)+"f");     Serial.print("g"+String(sensor)+"h");  //communicate data serially 
 confirm.publish(0);resistorbox_p.publish(0);breadboard_p.publish(0);sensor_p.publish(0);controllers_p.publish(0);
}
}
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  //Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
     //  Serial.println(mqtt.connectErrorString(ret));
   //    Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  //Serial.println("MQTT Connected!");
}
