/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to communicate between a 
Protractor and Arduino using I2C.  The angle between the Protractor and a nearby object will be printed to 
the Serial Port.  The angle between the Protractor and the most open pathway will also be printed to the 
Serial Port.

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

void setup() {
  Serial.begin(9600); // For printing results to the COM port Serial Monitor
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
}

void loop() {
  myProtractor.read(); // Communicate with the sensor to get the data
  
  // How many objects are within view?
  int numObjects = myProtractor.objectCount();
  Serial.print("Number of Objects: ");
  Serial.print(numObjects);
  Serial.print(", ");
  
  // If at least one object is in view, print the angle of the most visible object to the Serial Port
  if(numObjects > 0){
    int object = myProtractor.objectAngle(); // store the angle to the object in a variable
    Serial.print("Angle of Most Visible Object = ");
    Serial.print(object);             // Print the Angle of the object
    Serial.println(" degrees");
  }
  
  // Print the angles to all objects within view. Objects are in rank order from most visible to least visible.
  Serial.println("Angles, Visibility");
  for(int i = 0; i < numObjects; i++){
    Serial.print("   ");
	if(myProtractor.objectAngle(i) < 100) Serial.print(" ");
	if(myProtractor.objectAngle(i) < 10)  Serial.print(" ");
	Serial.print(myProtractor.objectAngle(i));
	Serial.print(", ");
	if(myProtractor.objectVisibility(i) < 100) Serial.print(" ");
	if(myProtractor.objectVisibility(i) < 10)  Serial.print(" ");
	Serial.println(myProtractor.objectVisibility(i));
  }
  Serial.println();
  
  // How many pathways are within view?
  int numPaths = myProtractor.pathCount();
  Serial.print("Number of Paths: ");
  Serial.print(numPaths);
  Serial.print(", ");
    
  // If at least one pathway is in view, print the angle of the most visibile pathway to the Serial Port
  if(numPaths > 0){
    int path = myProtractor.pathAngle(); // store the angle to the object in a variable
    Serial.print("Angle of Most Visible Path = ");
    Serial.print(path);             // Print the Angle
    Serial.println(" degrees");
  }
  
  // Print the angles to all paths within view. Paths are in rank order from most open to least open.
  Serial.println("Angles, Visibility");
  for(int i = 0; i < numPaths; i++){
    Serial.print("   ");
	if(myProtractor.pathAngle(i) < 100) Serial.print(" ");
	if(myProtractor.pathAngle(i) < 10)  Serial.print(" ");
	Serial.print(myProtractor.pathAngle(i));
	Serial.print(", ");
	if(myProtractor.pathVisibility(i) < 100) Serial.print(" ");
	if(myProtractor.pathVisibility(i) < 10)  Serial.print(" ");
	Serial.println(myProtractor.pathVisibility(i));
  }
  
  Serial.println();
  delay(1000);
}
