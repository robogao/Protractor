/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to use the Protractor's
advanced functions:
 - Read the deviced Product Type "PR". This can be used to verify the device is a Protractor
 - Read the Protractors's Serial Number
 - Read the Voltage Vin sensed by the Protractor. If Vin is too low the Protractor will disable itself and flash the Blue LEDs.
 - Read the raw data from the Protractor. Returns 8 values which correspond to the amount of light reflected by nearby objects.

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
  bool connected = myProtractor.connected();
  if(connected) {
    Serial.println("Connected to Protractor");
  }else{
    Serial.println("Communication Error, Check Wiring and I2C Address are Correct");
  }

  // Read the Product Type
  char prodType[2]; // 2 element array in which the Product Type can be stored.
  myProtractor.productType(prodType); // Read the Product Type from the Protractor and store in the prodType array
  Serial.print("Product Type = ");
  Serial.print(prodType[0]);
  Serial.println(prodType[1]);

  // Read the Serial Number
  Serial.print("Serial Number = ");
  uint32_t serialNumber = myProtractor.serialNumber(); // Read the Serial Number from the Protractor
  Serial.println(serialNumber,HEX);
}

void loop() {
  
  // Read the Voltage
  Serial.print("Voltage (mV) = ");
  int voltage = myProtractor.voltage(); // Read the Voltage Vin sensed by the Protractor
  Serial.println(voltage);
  delay(500);

  // Read the Raw Data Reflections
  byte data[8]; // 8 element array into which the raw data can be stored. Each value represents the amount of light reflected from nearby objects.
  myProtractor.reflections(data); // Read the raw data from the Protractor and store in the data array
  Serial.print("Reflections = ");
  for(int i = 0; i < 8; i++){
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println();

  delay(1000);
}