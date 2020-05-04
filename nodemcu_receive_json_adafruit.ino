
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Galaxy A7"
#define WLAN_PASS       "Manas9927"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "manasvivek27"
#define AIO_KEY         "f90dd97a76474940948bcf5200ee814b"

/************ Global State (you don't need to change this!) ******************/
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
/****************************** Feeds ***************************************/

// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/mq4_reading");
Adafruit_MQTT_Publish feedmq7 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/mq7_reading");
Adafruit_MQTT_Publish dustbin_status = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Dustbin_Status");
Adafruit_MQTT_Publish dustbin_status_2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Dustbin_Status_2");
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe dash = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/dashboards/dashboard1");
Adafruit_MQTT_Subscribe dash2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/dashboards/dashboard2");
Adafruit_MQTT_Subscribe dash3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/dashboards/dashboard3");

/*************************** Sketch Code ************************************/

void MQTT_connect();


void setup() {
  Serial.begin(115200);
  s.begin(115200);
  while (!Serial) continue;
  // put your setup code here, to run once:



  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  mqtt.subscribe(&dash);
}


uint32_t x=0;


void loop() {
   MQTT_connect();
     
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
 
  if (root == JsonObject::invalid())
  {
    return;
  }
  //Print the data in the serial monitor
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.println("");
  Serial.print(" MQ4 Reading ");
  int data1=root["mq4"];
  Serial.println(data1);
  Serial.print("MQ7 Reading    ");
  int data2=root["mq7"];
  Serial.println(data2);
  Serial.print("Distance  ");
  int data3=root["distance"];
  Serial.println(data3);
  
  Serial.print("Load  ");
  int data4=root["load"];
  Serial.println(data4);
  Serial.print("grams  ");
  
  Serial.print("Temperature  ");
  int data5=root["temp"];
  Serial.print(data5);
  Serial.println(" Degree Celcius  ");

  
  Serial.print("Humidity  ");
  int data6=root["humidity"];
  Serial.print(data6);
  Serial.println("%");
  
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
 Serial.println("");

 delay(5000);
  // put your main code here, to run repeatedly:
  
Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &dash) {
      Serial.print(F("Got: "));
      //Serial.println((char *)onoffbutton.lastread);
      char * v = (char *)dash.lastread;
      Serial.print(v);
      Serial.println(" ");

    }
  }
  data4 = data4 - 265;
//if (data4 >= 0 && data3 >=1)
//{
  Serial.println("DATA NOW BEING SENT");
  feed.publish(data1);
  feedmq7.publish(data2);
  dustbin_status.publish(data4);
  dustbin_status_2.publish(data3);
  temperature.publish(data5);
  humidity.publish(data6);

  delay(10000);
//}
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(500);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
