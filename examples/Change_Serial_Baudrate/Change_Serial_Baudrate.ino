/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to change the Serial 
baud rate of the Protractor.

The Protractor supports serial baud rates between 1200 and 250000 baud. Requests to operate at baud rates 
outside this range will be ignored by the Protractor.

Requests to change the Protractor's serial baud rate do not take effect until after the Protractor is
rebooted. To reboot, both the power supply connected to Protractor's Vin pin and any micro-controller 
connected to the Protractor's communication lines must be unplugged or powered down.

ELECTRICAL CONNECTIONS

To use the Protractor with an Arduino using Serial, make the following connections:
_________________________________________________________________
  PROTRACTOR    |   UNO     |  LEONARDO |   MEGA    |   DUE     |
--------------POWER----------------------------------------------
    GND         |   GND     |   GND     |   GND     |   GND     |  Connect Power Supply GND to Arduino GND and Protractor GND.
    Vin         |   Vin     |   Vin     |   Vin     |   Vin     |  NOTE: Vin must be between 6V to 14V.
--------------SERIAL---------------------------------------------
    DG/DGND     |   GND     |   GND     |   GND     |   GND     |
    VCC         |   5V      |   5V      |   5V      |   3.3V    |  Protractor VCC can be 3.3V to 5V. Used for communication only.
    TX          |   RX      |   RX      |   RX      |   RX      |  Protractor has built-in level shifters
    RX          |   TX      |   TX      |   TX      |   TX      |  Protractor has built-in level shifters
-----------------------------------------------------------------
For a complete tutorial on wiring up and using the Protractor go to:
    http://www.robogao.com/Protractor
*/

#include <Protractor.h>
#include <SoftwareSerial.h> // Remove if using Hardware Serial

Protractor myProtractor;

// Remove these 3 lines if using Hardware Serial
const int RX_pin = 10; // Not all pins can be used for RX. See https://www.arduino.cc/en/Reference/SoftwareSerial for limitations
const int TX_pin = 11;
SoftwareSerial mySerial(RX_pin,TX_pin); // RX,TX

// SoftwareSerial library supports a maximum 57600 baud.
long currentBaudRate = 9600; // Whatever the Protractor's current baud rate is, default is 9600
long newBaudRate = 57600; // Pick a new baud rate for the Protractor.

void setup() {
  Serial.begin(9600); // For printing results to the COM port Serial Monitor
  while (! Serial); // Wait for Leonardo
  
  // Remove these 2 lines if using Hardware Serial
  mySerial.begin(currentBaudRate); // Initialize Serial object for communicating with Protractor
  myProtractor.begin(mySerial); // Initialize Protractor
  
//  Serial1.begin(currentBaudRate); // Uncomment if using Hardware Serial
//  myProtractor.begin(Serial1); // Uncomment if using Hardware Serial
  
  Serial.println("Protractor Sensor Demo!");
  delay(500);
  
  // Check communication with the Protractor
  bool connected = myProtractor.read();
  if(connected) {
    Serial.print("Connected to Protractor over Serial at ");
	Serial.print(currentBaudRate);
	Serial.println(" baud");
  }else{
    Serial.println("Communication Error, Check Wiring and Baud Rate are Correct");
  }
  
  // Once initialized, a new baud rate can be set at any time.
  // The new baud rate does not take affect until the Protractor is rebooted.
  if(connected) {
    bool baudrateSet = myProtractor.setNewSerialBaudRate(newBaudRate); // Next time the Protractor is rebooted, serial communication will be at the new baud rate
	delay(10);
	if(baudrateSet){
	  Serial.print("New Baud Rate Set: ");
	  Serial.println(newBaudRate);
	  Serial.println();
	  Serial.println("-----------------------------------------------------------------");
	  Serial.println("Protractor must be Power Cycled for new baud rate to take effect.");
	  Serial.println("-----------------------------------------------------------------");
	  Serial.println();
	}else{
	  Serial.println("Unable to set new baud rate, baud rate is not valid");
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