/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to change the behavior 
of the Protractor's Feedback LEDs. The behaviors available are:
 - Show Object: Visually indicates the angle to the most visible object
 - Show Path: Visually indicates the angle to the most open path
 - Off: Turn off the feedback LEDs
Changes to the LED behavior take effect immediately. Changes are not remembered and must be set each time 
the Protractor is rebooted.

This example assumes I2C communication, but the same methods can be applied whether I2C or Serial 
communication is used.

For a complete tutorial on wiring up and using the Protractor go to:
    http://www.robogao.com/Protractor
*/

#include <Protractor.h>
#include <Wire.h>

Protractor myProtractor;

void setup() {
  Serial.begin(9600); // For printing results to the COM port Serial Monitor
  while (! Serial); // Wait for Leonardo
  
  Wire.begin();
  myProtractor.begin(Wire,69); // Use I2C/Wire Library to talk with Protractor on default address 69
  
  Serial.println("Protractor Sensor Demo!");
  delay(500);
  
  // Check communication with the Protractor
  bool connected = myProtractor.read();
  if(connected) {
    Serial.println("Connected to Protractor");
  }else{
    Serial.println("Communication Error, Check Wiring and I2C Address are correct");
  }
  
  // LED SHOW PATH
  // Set the LEDs to show the location of the most open path.
  if(connected) {
    myProtractor.LEDshowPath(); // LEDs will indicate the location of the most visible object
	Serial.println("Showing Path");
  }
  delay(10000); // Play with the Protractor for 10 seconds.
  
  // LED OFF
  // Set the LEDs to Off. This may be useful to save power or avoid interference with other optical sensors.
  // NOTE: Setting the LEDs to off will also disable the Green status LED.
  if(connected) {
    myProtractor.LEDoff(); // LEDs are off
	Serial.println("LEDs Off");
  }
  delay(2000); // Wait 2 seconds.
  
  // LED SHOW OBJECT
  // Set the LEDs to show the location of the most visible object.
  if(connected) {
    myProtractor.LEDshowObject(); // LEDs will indicate the location of the most visible object
	Serial.println("Showing Objects");
  }
}

void loop() {
  myProtractor.read(); // Communicate with the sensor to get the data
  
  Serial.print("Number of Objects: ");
  Serial.println(myProtractor.objectCount());
  
  if(myProtractor.objectCount() > 0){
    Serial.print("Angle to most visible Object = ");
    Serial.print(myProtractor.objectAngle());
    Serial.println(" degrees");
  }

  Serial.print("Number of Paths: ");
  Serial.println(myProtractor.pathCount());
    
  if(myProtractor.pathCount() > 0){
    Serial.print("Angle to the Path = ");
    Serial.print(myProtractor.pathAngle());
    Serial.println(" degrees");
  }
  
  Serial.println();
  delay(1000);
}