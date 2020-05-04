/* SMART GARBAGE MONITORING AND MANAGEMENT SYSTEM

   MANAS VIVEK CHOUBAL
 
  SENSORS USED:

  1. MQ-4:    For Methana Detection
  2. MQ-7:    For Carbon Mono-Oxide Detection
  3. HC SR04: Ultrasonic Sensor for Distance Detection
  4. HX 711:  Weighing Sensor module required for Load Cell
  5. DHT 11:  For Moisture Detection

*/

#include "HX711.h"  
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <dht.h>

dht DHT;

#define DHT11_PIN 7


#define calibration_factor -440.0 //Calibration value for Load Cell

#define DOUT  3                   //Digital Pin 3 for HX711
#define CLK  2                    //Digital Pin 2 for HX711

const int AOUTpin=0;              //Analog Pin for MQ-4 Sensor
const int AOUT7pin=1;             //Analog Pin for MQ-7 Sensor
const int DOUTpin=8;              //Digital Pin for MQ-4 Sensor
const int DOUT7pin=9;             //Digital Pin for MQ-7 Sensor

const int trig=10;                //Trig Pin for Ultrasonic Sensor
const int echo=11;                //Echo Pin for Ultrasonic Sensor

const int ledPin=13;              //the anode of the LED connects to digital pin D13 of the arduino
const int ledPin7=12;             //the anode of the LED connects to digital pin D122 of the arduino

int limit;                        //Limit value from MQ-4 Sensor
int value;                        //Detected value from MQ-4 Sensor

int limit7;                       //Limit value from MQ-7 Sensor
int value7;                       //Detected value from MQ-7 Sensor

int distance,duration;

int load;

int humidity,temp;

HX711 scale;



SoftwareSerial s(5,6);

void setup() {

Serial.begin(115200);//sets the baud rate
s.begin(115200);
scale.begin(DOUT, CLK);
scale.set_scale(calibration_factor); 
scale.tare();                     //Initial Weight = 0
pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
pinMode(DOUT7pin, INPUT);
pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
pinMode(ledPin7,OUTPUT);
pinMode(trig,OUTPUT);
pinMode(echo,INPUT);

  // put your setup code here, to run once:
}

//creating JSON class object
StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();



void loop() {

int chk = DHT.read11(DHT11_PIN);
temp = DHT.temperature;
humidity = DHT.humidity;
  
value= analogRead(AOUTpin);//reads the analaog value from the methane sensor's AOUT pin
value7= analogRead(AOUT7pin);

limit= digitalRead(DOUTpin);//reads the digital value from the methane sensor's DOUT pin
limit7= digitalRead(DOUT7pin);

digitalWrite(trig, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trig, HIGH);
delayMicroseconds(10);
digitalWrite(trig, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echo, HIGH);
// Calculating the distance
distance= duration*0.034/2;

/*
Serial.print("Methane Value: ");
Serial.print(value);//prints the methane value
Serial.print("\t\tCarbon Mono-Oxide Value: ");
Serial.print(value7);
Serial.print("\t\tDistance: ");
Serial.println(distance);
Serial.print("Weight: ");
Serial.print(scale.get_units() - 7, 1); 
Serial.print(" grams" ); 
Serial.println();

delay(1000);
*/

load = scale.get_units()- 7 ;

if (limit == HIGH){
digitalWrite(ledPin, HIGH);
}
else{
digitalWrite(ledPin, LOW);
}

//checking for CO
if (limit7 == HIGH){
digitalWrite(ledPin7, HIGH);
}
else{
digitalWrite(ledPin7, LOW);
}



 root["mq4"] = value;
 root["mq7"] = value7;
 root["distance"] = distance;
 root["load"] = load;
 root["temp"] = temp;
 root["humidity"] = humidity;

/*
 Serial.print("Methane Value: ");
 Serial.print(value);//prints the methane value
 Serial.print("\t\tCarbon Mono-Oxide Value: ");
 Serial.print(value7);
 Serial.print("\t\tDistance: ");
 Serial.print(distance);
 Serial.print("Weight: ");
 Serial.print(load); 
 Serial.print(" grams" ); 
 Serial.print("Temp: ");
 Serial.print(temp); 
 Serial.print("Humidity: ");
 Serial.println(humidity); 

 delay(2000);
*/

if(s.available()>0)
{
  root.printTo(s);
 delay(2000);
}
  // put your main code here, to run repeatedly:

}









 /*
 s.write(value);
 */


 
 /*
 s.write("Methane Value: ");
 s.write(value);//prints the methane value
 s.write("\t\tCarbon Mono-Oxide Value: ");
 s.write(value7);
 s.write("\t\tDistance: ");
 s.write(distance);
 s.write("Weight: ");
 s.write(scale.get_units() - 7, 1); 
 s.write(" grams" ); 
 */
