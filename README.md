![Protractor Angle Sensor](http://www.will-moore.com/images/ProtractorAngleSensor_sm.png)

# PROTRACTOR - A Proximity Sensor that Measures Angles

This Library works with the Protractor Sensor. A lot of sensors can tell the distance to an object, but determining the angle to an object is much harder. With a 180 degree field of view, the Protractor can sense open pathways and tell the angle to multiple objects up to 30cm (12 inches) away.  With a Protractor mounted to your mobile robot, you can easily find or avoid objects.

The Protractor is designed to work well with Mini Sumo robots, and can also be used as a general purpose proximity sensor.

For a complete tutorial on wiring up and using the Protractor go to:
http://www.will-moore.com/protractor/ProtractorAngleProximitySensor_UserGuide.pdf

The Protractor communicates with a master using either Serial Bus or I2C communication. Examples for using the Protractor with an Arduino are provided for both Serial and I2C. The Protractor has level shifters included on all communication lines and can be interfaced with either 5V or 3.3V microcontrollers.

### ELECTRICAL CONNECTIONS

To use the Protractor with an Arduino over I2C or Serial, connections must be made for Power and Communication.

I2C Connections:

|  PROTRACTOR    |   UNO     |  LEONARDO |   MEGA    |   DUE     |
| -------------- | --------- | --------- | --------- | --------- |
|    GND         |   GND     |   GND     |   GND     |   GND     |  Connect Power Supply GND to Arduino GND and Protractor GND.
|    Vin         |   Vin     |   Vin     |   Vin     |   Vin     |  NOTE: Vin must be between 6V to 14V.
|    DG/DGND     |   GND     |   GND     |   GND     |   GND     |  
|    VCC         |   5V      |   5V      |   5V      |   3.3V    |  Protractor VCC can be 3.3V to 5V. Used for communication only.
|    SDA         |   SDA/A4  |   SDA/2   |   SDA/20  |   SDA/20  |  Protractor has built-in level shifters
|    SCL         |   SCL/A5  |   SCL/3   |   SCL/21  |   SCL/21  |  Protractor has built-in level shifters

Serial Connections:

|  PROTRACTOR    |   UNO     |  LEONARDO |   MEGA    |   DUE     |
| -------------- | --------- | --------- | --------- | --------- |
|    GND         |   GND     |   GND     |   GND     |   GND     |  Connect Power Supply GND to Arduino GND and Protractor GND.
|    Vin         |   Vin     |   Vin     |   Vin     |   Vin     |  NOTE: Vin must be between 6V to 14V.
|    DG/DGND     |   GND     |   GND     |   GND     |   GND     |
|    VCC         |   5V      |   5V      |   5V      |   3.3V    |  Protractor VCC can be 3.3V to 5V. Used for communication only.
|    TX          |   RX      |   RX      |   RX      |   RX      |  Protractor has built-in level shifters
|    RX          |   TX      |   TX      |   TX      |   TX      |  Protractor has built-in level shifters

The following list of boards and communication ports have been tested with the Protractor as of Arduino IDE version 1.8.2:
 1. Uno, Nano: Serial,  Software Serial and Wire work
 2. Leonardo:  Serial1, Software Serial and Wire work
 3. Mega2560:  Serial,  Serial1, Serial2, Serial3, Software Serial and Wire work
 4. Due:       Serial,  Serial1, Serial2, Serial3 and Wire work; Wire1 does not work
 5. Teensy3.2: Serial1, Serial2, Serial3, AltSoftSerial, Wire work; Software Serial does not work
 6. Teensy3.6: Serial1, Serial2, Serial3, Serial4, Serial5, AltSoftSerial, Wire, Wire1, Wire2 work; Software Serial does not work

### BASIC FUNCTIONS

The Protractor library provides a command to read all available data from the Protractor Sensor. The data is stored inside the master's memory.

The Protractor library provides a command which tells the number of objects which are detected by the sensor. Another command tells the number of open paths detected by the sensor.

The Protractor library provides a command which tells the angle between the sensor and an object. Another command tells the angle between the sensor and an open path.

### ADVANCED FUNCTIONS

The Protractor library provides a command to change the sensor's I2C address. The default I2C address of all Protractors is set to 69 (0x45) during manufacture. Changing the default I2C address allows connecting multiple Protractors to a single host microcontroller. The Protractor must be reset for the new address to take effect. The Protractor will remember the new I2C address after it is powered down and restarted.

The Protractor library provides a command to change the sensor's Serial baud rate. The default baud rate is 9600. If the wiring connections to the host microcontroller are short, high baud rates can be used to achieve faster communications. If the wiring connections are long, slower baud rates can be used to reduce communication errors. Note: The baud rate of the Serial object used to initialize the library must match the baud rate setting within the Protractor. The Protractor must be reset for the new baud rate to take effect. The Protractor will remember the new Serial baud rate after it is powered down and restarted.

The Protractor has two Blue LEDs to provide visual feedback. By default, the LEDs indicate the location of the most visible object within view. The library provides a command to switch the LED behavior to indicate the location of the most open pathway. The library also provides a command to disable the feedback LEDs to save power or eliminate interference with other optical sensors nearby. Changes to the LED behavior are not remembered after the sensor is rebooted.

The Protractor library provides a command to change amount of time between sensor scans. By default, the sensor scans its field of view once every 15 milliseconds. The scan time can be as fast as every 15 milliseconds, or as slow as every 32 seconds. Significant power savings can be achieved by increasing the scan time. The average current consumption of the sensor can be estimated as 15 + TBD / scantime_ms = milliAmps. Changes to the scan time are not remembered after the sensor is rebooted.

If the Protractor's scan time is set to zero, continuous scanning will be disabled. The Protractor will scan for objects only when data is requested by the master. When data is requested, there will be a 15 millisecond delay before the Protractor responds with the requested data. Care must be taken to ensure the communication link with the master is able to accept this amount of delayed response without causing issues. To disable the Protractor, set the scan time to zero and don't make any requests for data.

### List of Available Functions
```
Function:     Protractor.begin(Serial)  - initialize a Protractor using Serial communication
Parameters:   (Stream)Serial: reference to a Serial object. Could be Serial, Serial1, or a SoftwareSerial object
Return:       none

Function:     Protractor.begin(Wire,address)  - initialize a Protractor using I2C communication
Parameters:   (TwoWire)Wire:    reference to a Wire object. Could be Wire, Wire1, etc.
              (int16_t)address: the I2C address of the Protractor, default Protractor address is 69 (0x45).
Return:       none

Function:     Protractor.read() - request all the available data from the Protractor
Parameters:   none
Return:       none

Function:     Protractor.read(dataPoints) - request a limited number of data points from the Protractor
Parameters:   (int16_t)dataPoints: ranges from 1 to 4
Return:       none

Function:     Protractor.objectCount() - returns the number of objects detected. The number of objects detected may change every time that Protractor.read() is called.
Parameters:   none
Return:       (int16_t) ranges from 0 to 4

Function:     Protractor.pathCount() - returns the number of paths detected. The number of paths detected may change every time that Protractor.read() is called.
Parameters:   none
Return:       (int16_t) ranges from 0 to 4

Function:     Protractor.objectAngle() - returns the angle to the most visible object
Parameters:   none
Return:       (int16_t) If object is seen, returns the angle from 0 to 180. Else, returns -1.

Function:     Protractor.objectAngle(ob) - returns the angle to an object ob
Parameters:   (int16_t) ob - ranges from 0 to 3, specifies which object we want to know the angle of. Objects are ranked from most intense to least intense.
Return:       (int16_t) If 0 <= ob < objectCount(), returns the angle 0 to 180. Else, returns -1.

Function:     Protractor.objectVisibility(ob) - returns the visibility of an object ob. Visibility is a relative measure of the amount of light reflected off an object. Visibility is generally not a good indicator of distance.
Parameters:   (int16_t) ob - ranges from 0 to 3, specifies which object we want to know the angle of. Objects are ranked from most intense to least intense.
Return:       (int16_t) If 0 <= ob < objectCount(), returns the visibility from 0 to 255. Else, returns -1.

Function:     Protractor.pathAngle() - returns the angle to the most open path
Parameters:   none
Return:       (int16_t) If path is seen, returns the angle from 0 to 180. Else, returns -1.

Function:     Protractor.pathAngle(pa) - returns the angle to a path pa
Parameters:   (int16_t) pa - ranges from 0 to 3, specifies which path we want to know the angle of. Paths are ranked from most open to least open.
Return:       (int16_t) If 0 <= pa < pathCount(), returns the angle 0 to 180. Else, returns -1.

Function:     Protractor.pathVisibility(pa) - returns the visibility of a path pa. Visibility is a relative measure of how little light is reflected from a pathway. Visibility can indicate which of several pathways is more open.
Parameters:   (int16_t) ob - ranges from 0 to 3, specifies which object we want to know the angle of. Objects are ranked from most intense to least intense.
Return:       (int16_t) If 0 <= ob < pathCount(), returns the visibility from 0 to 255. Else, returns -1.

Function:     Protractor.LEDshowObject() - Set the feedback LED behavior to indicate where the object is
Parameters:   none
Return:       none

Function:     Protractor.LEDshowPath() - Set the feedback LED behavior to indicate where the path is
Parameters:   none
Return:       none

Function:     Protractor.LEDoff() - Set the feedback LED off
Parameters:   none
Return:       none

Function:     Protractor.scanTime(milliSeconds) - Set the time between scans
Parameters:   (int16_t)milliSeconds - ranges from 0 to 32,767. If 0, scanning is only performed when data is requested. If 1 <= milliSeconds < 14, scan time is set to 15 milliSeconds. If 15 <= milliSeconds <= 32,767, scan time is set to milliSeconds. If milliSeconds > 32,767, scan time is not changed. Default scanTime is 15 milliSeconds.
Return:       none

Function:     Protractor.setNewI2Caddress(int16_t newAddress) - Change Protractor's I2C address. Default address is 69 (0x45) set during manufacture.
Parameters:   (int16_t)newAddress - ranges from 2 to 127. If address < 2 or address > 127, address is not changed.
Return:       none

Function:     Protractor.setNewSerialBaudRate(baudRate) - Change Protractor's Serial Baud Rate
Parameters:   (int32_t)baudRate - ranges from 1200 to 230400. Default is 9600. if baudRate <1200 or baudRate > 230400, baudRate is not changed. It is recommend to use standard baud rates such as 1200, 9600, 57600, 115200
Return:       none
```
