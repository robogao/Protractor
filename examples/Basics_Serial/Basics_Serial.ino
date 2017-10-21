/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to communicate between a 
Protractor and Arduino using serial communication. The angle between the Protractor and a nearby object 
will be printed to the Serial Port.  The angle between the Protractor and the most open pathway will also 
be printed to the Serial Port.

Because the primary hardware serial port is typically used for debugging, it is recommended to use an 
alternate hardware serial port. Boards such as the Mega, Leonardo, Micro, Due, etc. have extra hardware 
serial ports that can be used. This example shows how to use the Serial1 port. When using an Arduino such 
as the Uno or Nano which does not have extra hardware serial ports, it is recommended to use Software Serial 
or I2C to communicate with the Protractor. See the Protractor Software Serial and Protractor I2C examples 
for details.

ELECTRICAL CONNECTIONS

To use the Protractor with an Arduino using Serial1/RX1/TX1, make the following connections:
_____________________________________________________
  PROTRACTOR    |  LEONARDO |   MEGA    |   DUE     |
--------------POWER----------------------------------
    GND         |   GND     |   GND     |   GND     |  Connect Power Supply GND to Arduino GND and Protractor GND.
    Vin         |   Vin     |   Vin     |   Vin     |  NOTE: Vin must be between 6V to 14V.
--------------SERIAL---------------------------------
    DG/DGND     |   GND     |   GND     |   GND     |
    VCC         |   5V      |   5V      |   3.3V    |  Protractor VCC can be 3.3V to 5V. Used for communication only.
    TX          |   0       |   19      |   19      |  
    RX          |   1       |   18      |   18      |  
-----------------------------------------------------
For a complete tutorial on wiring up and using the Protractor go to:
    http://www.robogao.com/Protractor
*/

#include <Protractor.h>

Protractor myProtractor;

void setup() {
  Serial.begin(9600); // For printing results to the COM port
  while (! Serial); // Wait for Leonardo
  
  Serial1.begin(9600); // Initialize Serial1 for communicating with Protractor
  myProtractor.begin(Serial1); // Use Serial1 to talk with Protractor.
  
  Serial.println("Protractor Sensor Demo!");
  delay(500);
  
  // Check communication with the Protractor
  bool connected = myProtractor.read();
  if(connected) {
    Serial.println("Connected to Protractor");
  }else{
    Serial.println("Communication Error, Check Wiring and Baud Rate are correct");
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
