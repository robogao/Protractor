/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to change the I2C address 
of the Protractor. Changes to the Protractor's I2C address do not take effect until after the Protractor
is rebooted.

ELECTRICAL CONNECTIONS

To use the Protractor with an Arduino over I2C, make the following connections:
_________________________________________________________________
  PROTRACTOR    |   UNO     |  LEONARDO |   MEGA    |   DUE     |
--------------POWER----------------------------------------------
    GND         |   GND     |   GND     |   GND     |   GND     |  Connect Power Supply GND to Arduino GND and Protractor GND.
    Vin         |   Vin     |   Vin     |   Vin     |   Vin     |  NOTE: Vin must be between 6V to 14V.
---------------I2C-----------------------------------------------
    DG/DGND     |   GND     |   GND     |   GND     |   GND     |  
    VCC         |   5V      |   5V      |   5V      |   3.3V    |  Protractor VCC can be 3.3V to 5V. Used for communication only.
    SDA         |   SDA/A4  |   SDA/2   |   SDA/20  |   SDA/20  |  Protractor has built-in level shifters
    SCL         |   SCL/A5  |   SCL/3   |   SCL/21  |   SCL/21  |  Protractor has built-in level shifters
-----------------------------------------------------------------

For a complete tutorial on wiring up and using the Protractor go to:
    http://www.will-moore.com/protractor/ProtractorAngleProximitySensor_UserGuide.pdf
*/


#include <Protractor.h>
#include <Wire.h>

Protractor myProtractor;

int currentI2Caddress = 69; // Whatever the Protractor's current I2C address is, default is 69
int newI2Caddress = 28; // Pick a number between 0 and 127 that is not already on the I2C bus

void setup() {
  Serial.begin(9600); // For printing results to the COM port Serial Monitor
  myProtractor.begin(Wire,currentI2Caddress); // Initialize Protractor at the current I2C address
  
  Serial.println("Protractor Sensor Demo!");
  delay(500);
  
  // Check communication with the Protractor
  bool connected = myProtractor.read();
  if(connected) {
    Serial.println("Connected to Protractor");
  }else{
    Serial.println("Communication Error, Check Wiring and I2C Address are correct");
  }
  
  // Once initialized, a new I2C address can be set at any time.
  // The new I2C address does not take affect until the Protractor is rebooted.
  if(connected) {
    myProtractor.setNewI2Caddress(newI2Caddress); // Next time the Protractor is rebooted, it will be on I2C address 28
	Serial.print("New I2C Address Set: ");
	Serial.println(newI2Caddress);
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
