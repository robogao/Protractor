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
*/

#include "Arduino.h"
#include "Protractor.h"

Protractor::Protractor()
{
}

void Protractor::begin(Stream &serial) // Initialize the Protractor with Serial communication
{
  _serial = &serial;
  _comm = SERIALCOMM;
}
void Protractor::begin(TwoWire &wire, int16_t address) // Initialize the Protractor with I2C communication
{
  _address = address;
  _wire = &wire;
  _wire->begin();
  _comm = I2CCOMM;
}

/////// BASIC FUNCTIONS ///////

bool Protractor::read() { // get all of the data from the protractor.
  return read(MAXOBJECTS);
}

bool Protractor::read(int16_t obs) { // gets obs number of objects and obs number of paths from protractor. Returns the most intense objects and most open pathways. Minimizes data transfer for time sensitive applications.
  if(obs > MAXOBJECTS) obs = MAXOBJECTS;
  _numdata = obs;
  uint8_t numBytes = 1 + obs*4;
  _requestData(numBytes); // Request bytes from the obstacle sensor
  int i = 0;
  unsigned long startTime = micros();
  unsigned long duration = 0;
  unsigned long maxWait = 20000; // Wait no longer than this many micro-seconds for the next byte to arrive
  while(i < numBytes && duration < maxWait){
	if(_available()) {
		_buffer[i] = _read(); 
		i++;
		startTime = micros();
	}
	duration =  micros() - startTime;
  }
  if(i == 0){
	  return 0;
  } else {
	  return 1;
  }
}

int16_t Protractor::objectCount() { // returns the number of objects detected
  return (int16_t)(_buffer[0] >> 4); // number of objects detected is the high nibble of _buffer[0]
}

int16_t Protractor::pathCount() { // returns the number of paths detected
  return (int16_t)(_buffer[0] &= 0b00001111); // number of paths detected is the low nibble of _buffer[0]
}

int16_t Protractor::objectAngle() { // returns the angle to the most intense object
  return objectAngle(0);
}
int16_t Protractor::objectAngle(int16_t ob) { // returns the angle to the object ob in the object list, indexed from 1. Left most object is 1. If ob exceeds number of objects detected, return zero.
  if(ob >= objectCount() || ob < 0){
    return -1;
  }else{
    int16_t angle = map(_buffer[1+4*ob],0,255,0,180);  // ob0->_buffer[1], ob1->_buffer[5], etc.
    return angle;
  }
}
int16_t Protractor::objectVisibility(int16_t ob) { // returns the visibility of the object ob in the object list, indexed from 1. Left most object is 1. If ob exceeds number of objects detected, return zero.
  if(ob >= objectCount() || ob < 0){
    return -1;
  }else{
    int16_t vis = _buffer[2+4*ob];  // ob0->_buffer[2], ob1->_buffer[6], etc.
    return vis;
  }
}

int16_t Protractor::pathAngle() { // returns the angle to the most open pathway
  return pathAngle(0);
}
int16_t Protractor::pathAngle(int16_t pa) { // returns the angle to the path pa in the pathway list, indexed from 1. Left most path is 1. If pa exceeds number of pathways detected, return zero.
  if(pa >= pathCount() || pa < 0){
    return -1;
  }else{
    int16_t angle = map(_buffer[3+4*pa],0,255,0,180);  // pa0->_buffer[3], pa1->_buffer[7], etc.
    return angle;
  }
}
int16_t Protractor::pathVisibility(int16_t pa) { // returns the angle to the path pa in the pathway list, indexed from 1. Left most path is 1. If pa exceeds number of pathways detected, return zero.
  if(pa >= pathCount() || pa < 0){
    return -1;
  }else{
    int16_t vis = _buffer[4+4*pa];  // pa0->_buffer[4], pa1->_buffer[8], etc.
    return vis;
  }
}

/////// ADVANCED FUNCTIONS ///////

void Protractor::scanTime(int16_t milliSeconds) {//0 = scan only when called. 1 to 15 = rescan every 15ms, >15 = rescan every time_ms milliseconds.  Default time_ms is set to 30ms.
  if(milliSeconds >= 1 && milliSeconds <= MINDUR-1) {  // Values within 1 and 14 milliSeconds aren't allowed, the sensor requires a minimum 15 seconds to complete a scan.
    uint8_t sendData[3] = {SCANTIME,MINDUR,'\n'};
    _write(sendData,3); // Send a signal (char SCANTIME) to tell Protractor that it needs to change its time between scans to milliSeconds.
  }else if(milliSeconds >= 0 && milliSeconds <= 32767) {  // Values less than 0 or greater than 32767 aren't allowed.
    uint8_t sendData[4] = {SCANTIME,(byte)(milliSeconds & 0x00FF),(byte)(milliSeconds >> 8),'\n'};
    _write(sendData,4); // Send a signal (char SCANTIME) to tell Protractor that it needs to change its time between scans to milliSeconds. 
  }
}

void Protractor::setNewI2Caddress(int16_t newAddress) { // change the I2C address. Will be stored after shutdown. See manual for instructions on restoring defaults. Default = 0x45 (69d).
  if(newAddress >= 2 && newAddress <= 127) {
	uint8_t sendData[3] = {I2CADDR,(byte)newAddress,'\n'};
    _write(sendData,3); // Send a signal (char I2CADDR) to tell Protractor that it needs to change its I2C address to the newAddress
  }
}

void Protractor::setNewSerialBaudRate(int32_t newBaudRate) { // change the Serial Bus baud rate. Will be stored after shutdown. See manual for instructions on restoring defaults. Default = 9600 baud. 0 = 1200, 1 = 2400, 2 = 4800, 3 = 9600, 4 = 19200, 5 = 28800, 6 = 38400, 7 = 57600, 8 = 115200, 9 = 230400
  if(newBaudRate >= 1200 && newBaudRate <= 1000000) {
	uint8_t sendData[5] = {BAUDRATE,(byte)(newBaudRate & 0x00FF),(byte)(newBaudRate >> 8),(byte)(newBaudRate >> 16),'\n'};
    _write(sendData,5); // Send a signal (char BAUDRATE) to tell Protractor that it needs to change its baudrate to the newBaudRate
  }
}

void Protractor::LEDshowObject() { // Set the feedback LEDs to follow the most intense Objects detected
  uint8_t sendData[3] = {LEDUSAGE,SHOWOBJ,'\n'};
  _write(sendData,3); // Send a signal (char LEDUSAGE) to tell Protractor that it needs to SHOWOBJ
}

void Protractor::LEDshowPath() { // Set the feedback LEDs to follow the most open pathway detected
  uint8_t sendData[3] = {LEDUSAGE,SHOWPATH,'\n'};
  _write(sendData,3); // Send a signal (char LEDUSAGE) to tell Protractor that it needs to SHOWPATH
}

void Protractor::LEDoff() { // Turn off the feedback LEDs
  uint8_t sendData[3] = {LEDUSAGE,LEDOFF,'\n'};
  _write(sendData,3); // Send a signal (char LEDUSAGE) to tell Protractor that it needs to turn the feedback LEDOFF
}

/////// PRIVATE FUNCTIONS ///////

uint8_t Protractor::_available() {
  if(_comm == I2CCOMM){
    return _wire->available();
  }
  else if(_comm == SERIALCOMM){
    return _serial->available();
  }
  else{
	return 0;
  }
}

uint8_t Protractor::_read() {
  if(_comm == I2CCOMM){
    return _wire->read();
  }
  else if(_comm == SERIALCOMM){
    return _serial->read();
  }
  else{
	return 0;
  }
}

void Protractor::_write(uint8_t arrayBuffer[], uint8_t arrayLength) {
  if(_comm == I2CCOMM){
    _wire->beginTransmission(_address);
    _wire->write(arrayBuffer,arrayLength);
    _wire->endTransmission();
  }
  else if(_comm == SERIALCOMM){
    _serial->write(arrayBuffer,arrayLength);
  }
}

void Protractor::_requestData(uint8_t numBytes) {
  if(_comm == I2CCOMM){
    _wire->requestFrom(_address, numBytes);
  }
  else if(_comm == SERIALCOMM) {
    uint8_t sendData[3] = {REQUESTDATA,numBytes,'\n'};
    _write(sendData,3); // Send a signal (char SENDDATA) to tell Protractor that it needs to send data, tell it the number of data points to send.
  }
}