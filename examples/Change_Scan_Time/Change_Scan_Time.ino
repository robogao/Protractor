/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to change the scan time 
of the Protractor.
 - By default, the Protractor scans 180 degrees every 15 milliseconds. This example will demonstrate how 
   to increase the time between scans to save power.
 - By default, the Protractor scans continuously. This example will demonstrate how to disable continuous 
   scanning so that scans occur only when data is requested. There will be approximately 15 milliseconds 
   delay between when the data is requested and when the Protractor responds.
Changes to the scan time take effect immediately. Changes are not remembered and must be set each time the 
Protractor is rebooted.

This example assumes I2C communication, but the same methods can be applied whether I2C or Serial communication 
is used.

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
  
  // Increase the Scan Time to 300 milliSecond
  // Notice that the Blue LEDs won't update as fast or smoothly now, but the average power consumption will drop significantly
  if(connected) {
    bool scanTimeSet = myProtractor.scanTime(300); // scan time must be between 1 and 32767
    if(scanTimeSet){ // It will now take 300 milliSeconds to complete a 180 degree scan
	  Serial.println("New Scan Time Set");
	}else{
	  Serial.println("Unable to set new scan time, scan time is not valid");
	}
  }
  delay(10000); // Play with the Protractor for 10 seconds. 
  
  // Disable Continuous Scanning
  if(connected) {
    myProtractor.scanTime(0); // A Scan Time of Zero will disable continuous scanning.
	Serial.println("Continuous Scan Disabled"); // Note that the Blue LEDs will only update after a scan is requested
  }
}

void loop() {
  // Continuous scanning was disabled in the setup.
  myProtractor.read(); // Request the Protractor to perform a scan and send out the data
  
  Serial.print("Number of Objects: ");
  Serial.println(myProtractor.objectCount());
  
  if(myProtractor.objectCount() > 0){
    Serial.print("Angle to most visible Object = ");
    Serial.print(myProtractor.objectAngle());
    Serial.println(" degrees");
  }
  
  Serial.println();
  delay(3000);
}