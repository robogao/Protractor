/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to communicate between a 
Protractor and Arduino using Software Serial. The angle between the Protractor and a nearby object will be 
printed to the Serial Port.  The angle between the Protractor and the most open pathway will also be printed 
to the Serial Port.

ELECTRICAL CONNECTIONS

To use the Protractor with an Arduino using SoftwareSerial, make the following connections:
_________________________________________________________________
  PROTRACTOR    |   UNO     |  LEONARDO |   MEGA    |   DUE     |
--------------POWER----------------------------------------------
    GND         |   GND     |   GND     |   GND     |   GND     |  Connect Power Supply GND to Arduino GND and Protractor GND.
    Vin         |   Vin     |   Vin     |   Vin     |   Vin     |  NOTE: Vin must be between 6V to 14V.
--------------SERIAL---------------------------------------------
    DG/DGND     |   GND     |   GND     |   GND     |   GND     |
    VCC         |   5V      |   5V      |   5V      |   3.3V    |  Protractor VCC can be 3.3V to 5V. Used for communication only.
    TX          |   RX_pin  |   RX_pin  |   RX_pin  |   RX_pin  |  Protractor has built-in level shifters
    RX          |   TX_pin  |   TX_pin  |   TX_pin  |   TX_pin  |  Protractor has built-in level shifters
-----------------------------------------------------------------
For a complete tutorial on wiring up and using the Protractor go to:
    http://www.robogao.com/Protractor
*/

#include <Protractor.h>
#include <SoftwareSerial.h>

Protractor myProtractor;

// Use Software Serial to talk with Protractor
const int RX_pin = 10; // Not all pins can be used for RX. See https://www.arduino.cc/en/Reference/SoftwareSerial for limitations
const int TX_pin = 11;
SoftwareSerial mySerial(RX_pin,TX_pin); // RX,TX

void setup() {
  Serial.begin(9600); // For printing results to the COM port Serial Monitor
  mySerial.begin(9600); // Initialize SoftwareSerial object mySerial for communicating with Protractor
  myProtractor.begin(mySerial); // Use mySerial to talk with Protractor.
  
  Serial.println("Protractor Sensor Demo!");
  delay(500);
  
  // Check communication with the Protractor
  bool connected = myProtractor.read();
  if(connected) {
    Serial.println("Connected to Protractor");
  }else{
    Serial.println("Communication Error, Check Wiring and Baudrate are correct");
  }
}

void loop() {
  myProtractor.read();
  
  // How many objects are within view?
  int numObjects = myProtractor.objectCount();
  Serial.print("Number of Objects: ");
  Serial.println(numObjects);
  
  // If at least one object is in view, print the angle of the most visible object to the Serial Port
  if(numObjects > 0){
    int object = myProtractor.objectAngle(); // store the angle to the object in a variable
    Serial.print("Angle to the Object = ");
    Serial.print(object);             // Print the Angle
    Serial.println(" degrees");
  }
  
  // Print the angles to all objects within view. Objects are in rank order from most visible to least visible.
  Serial.println("Angles to all Objects Seen:");
  for(int i = 0; i < numObjects; i++){
	Serial.print("  Object ");
	Serial.print(i);
	Serial.print(": ");
	Serial.print(myProtractor.objectAngle(i));
	Serial.println(" degrees");
  }
  
  // How many pathways are within view?
  int numPaths = myProtractor.pathCount();
  Serial.print("Number of Paths: ");
  Serial.println(numPaths);
    
  // If at least one pathway is in view, print the angle of the pathway to the Serial Port
  if(numPaths > 0){
    int path = myProtractor.pathAngle(); // store the angle to the object in a variable
    Serial.print("Angle to the Path = ");
    Serial.print(path);             // Print the Angle
    Serial.println(" degrees");
  }
  
  // Print the angles to all paths within view. Paths are in rank order from most open to least open.
  Serial.println("Angles to all Paths Seen:");
  for(int i = 0; i < numPaths; i++){
	Serial.print("  Path ");
	Serial.print(i);
	Serial.print(": ");
	Serial.print(myProtractor.pathAngle(i));
	Serial.println(" degrees");
  }
  
  Serial.println();
  delay(1000);
}