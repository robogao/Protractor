/*
  Protractor.h - Library for using the Protractor Sensor
  Copyright (c) 2017 William Moore.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  ###########################################################################
  
  A lot of sensors can tell the distance to an object, but determining the angle to an object is much harder. 
  With a 180 degree field of view, the Protractor can sense open pathways and tell the angle to multiple 
  objects up to 30cm (12 inches) away.  With a Protractor mounted to your mobile robot, you can easily find 
  or avoid objects.

  The Protractor is designed to work well with Mini Sumo robots, and can also be used as a general purpose 
  proximity sensor.

  For a complete tutorial on wiring up and using the Protractor go to:
    http://www.robogao.com/Protractor
	
  ############################################################################
*/

//////////////////////BEGIN PROTRACTOR.H //////////////////////
#include <Wire.h>
#include <inttypes.h>
	
// Constants
#define SERIALCOMM 1
#define I2CCOMM  2
#define MAXOBJECTS 4
#define SHOWOBJ  1
#define SHOWPATH 2
#define LEDOFF   3
#define MINDUR   15

// PROTRACTOR COMMANDS
#define REQUESTDATA 0x15
#define SCANTIME 0x20
#define I2CADDR  0x24
#define BAUDRATE 0x26
#define LEDUSAGE 0x30

class Protractor
{
  public:
    Protractor();
    void begin(Stream &serial); // Initialize protractor using Serial
    void begin(TwoWire &wire, int16_t address); // Initialize protractor using I2C
    bool read(); // gets all the data for all objects and paths from the protractor. Up to 4 objects and paths may be sensed at a time.
    bool read(int16_t obs); // gets only obs number of objects and obs number of paths from protractor. Returns the most visible objects and most open pathways first. Minimizes data transfer for time sensitive applications. If obs > 4 then obs = 4.
    int16_t objectCount(); // returns the number of objects detected
    int16_t pathCount(); // returns the number of paths detected
    int16_t objectAngle(); // returns the angle to the most visible object
    int16_t objectAngle(int16_t ob); // returns the angle to the object ob in the object list. Valid values of ob are 0 to 3. Object are ranked by intensity.  Most visible object is ob = 0.  Least visible object is ob = 3. If ob exceeds number of data points returned from sensor, returns -1.
	int16_t objectVisibility(); // returns the visibility of the most visible object
    int16_t objectVisibility(int16_t ob); // returns the visibility of the object ob in the object list. Valid values of ob are 0 to 3. Visibility is a relative measure of the amount of light reflected off an object. Visibility is generally not a good indicator of distance. If ob exceeds number of data points returned from sensor, returns -1.
    int16_t pathAngle(); // returns the angle to the most open pathway
    int16_t pathAngle(int16_t pa); // returns the angle to the path pa in the pathway list. Valid values of pa are 0 to 3. Pathways are ranked by openness.  Most open pathway is pa = 0.  Least open pathway is pa = 3. If pa exceeds number of data points returned from sensor, returns -1.
	int16_t pathVisibility(); // returns the visibility of the most open pathway
    int16_t pathVisibility(int16_t pa); // returns the visibility of a path pa in the path list. Valid values of pa are 0 to 3. Visibility is a relative measure of how little light is reflected from a pathway. Visibility can indicate which of several pathways is more open. If pa exceeds number of data points returned from sensor, returns -1.
    void LEDshowObject(); // Set the feedback LEDs to follow the most visible Objects detected
    void LEDshowPath(); // Set the feedback LEDs to follow the most open pathway detected
    void LEDoff(); // Turn off the feedback LEDs
    void scanTime(int16_t milliSeconds); // 0 = scan only when called. 1 to 15 = rescan every 15ms, >15 = rescan every milliSeconds, max 32767.  Default time_ms is set to 15ms.
    void setNewI2Caddress(int16_t newAddress); // Change the I2C address. Will be remembered after Protractor shutdown. Protractor must be reset to take effect. See manual for instructions on restoring defaults. Default = 0x45 (69d).
    void setNewSerialBaudRate(int32_t baudRate); // Change the Serial Bus baud rate. Will be remembered after Protractor shutdown. Protractor must be reset to take effect. See manual for instructions on restoring defaults. Default = 9600 baud.
  private:
    uint8_t _read();
    void _write(uint8_t arrayBuffer[], uint8_t arrayLength);
    uint8_t _available();
    void _requestData(uint8_t numBytes);
    uint8_t _buffer[1+4*MAXOBJECTS]; // store data received from Protractor.
    uint8_t _address; // Stores the I2C bus address when communicating over I2C
    uint8_t _numdata; // Number of data points requested from sensor during most recent read
    uint8_t _comm; // Tracks whether we are using I2C or Serial for communication
    Stream* _serial; // Handle for the Serial object. May be a HW or SW serial.
    TwoWire* _wire; // Handle for the TwoWire object (i2c). Allows usage of boards with multiple Wire ports.
};
