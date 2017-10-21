/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to change the I2C address 
of the Protractor.

The Protractor supports I2C addresses between 2 and 127. Requests to operate on addresses outside this range
will be ignored by the Protractor.

Requests to change the Protractor's I2C address do not take effect until after the Protractor is
rebooted. To reboot, both the power supply connected to Protractor's Vin pin and any micro-controller 
connected to the Protractor's communication lines must be unplugged or powered down.

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
    http://www.robogao.com/Protractor
*/


#include <Protractor.h>
#include <Wire.h>

Protractor myProtractor;

int currentI2Caddress = 69; // Whatever the Protractor's current I2C address is, default is 69
int newI2Caddress = 28; // Pick a number between 0 and 127 that is not already on the I2C bus

void setup() {
  Serial.begin(9600); // For printing results to the COM port Serial Monitor
  while (! Serial); // Wait for Leonardo
  
  Wire.begin();
  myProtractor.begin(Wire,currentI2Caddress); // Use I2C/Wire Library to talk with Protractor on default address 69
  
  Serial.println("Protractor Sensor Demo!");
  delay(500);
  
  // Check communication with the Protractor
  bool connected = myProtractor.read();
  if(connected) {
    Serial.print("Connected to Protractor on I2C Address ");
	Serial.println(currentI2Caddress);
  }else{
    Serial.println("Communication Error, Check Wiring and I2C Address are correct");
  }
  
  // Once initialized, a new I2C address can be set at any time.
  // The new I2C address does not take affect until the Protractor is rebooted.
  if(connected) {
    bool addressSet = myProtractor.setNewI2Caddress(newI2Caddress); // Next time the Protractor is rebooted, it will be on I2C address 28
	delay(10);
	if(addressSet){
	  Serial.print("New I2C Address Set: ");
	  Serial.println(newI2Caddress);
	  Serial.println();
	  Serial.println("-------------------------------------------------------------------");
	  Serial.println("Protractor must be Power Cycled for new I2C address to take effect.");
	  Serial.println("-------------------------------------------------------------------");
	  Serial.println();
	}else{
	  Serial.println("Unable to set new address, address is not valid");
	}
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