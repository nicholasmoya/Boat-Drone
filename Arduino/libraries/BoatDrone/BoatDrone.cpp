/*

Nicholas Moya
SRNL - Boat Drone
Dr. Tad Whiteside
7/4/2016

BOAT DRONE - SOURCE CODE (.cpp)
---------------------------------------------------------------------------
This is the source code .cpp file for the Boat Drone main arduino .ino file.
Five functions, and several variables, are explicitly defined here and 
referenced in the main code. The functions are:

	DisplayCoordinates - prints coordinates to the serial monitor
	Haversine - uses the haversine formula to calculate and return the
				distance between two coordinates
	GetHeading - determines the current heading
	PID - calculates the control variable, u, needed to steer the boat
	DCmotor - sets the speed of a DC motor

These functions are referenced in the .h source code file.

*/

#include "Arduino.h"
#include "BoatDrone.h"

void BoatDrone::DisplayCoordinates(double lat, double lon)
{
  if (lat > 0)
  {
    Serial.print(lat, 4);
    Serial.print(" N, ");
  }
  else
  {
    Serial.print(lat*(-1), 4);
    Serial.print(" S, ");
  }
  
  if (lon > 0)
  {
    Serial.print(lon, 4);
    Serial.println(" E.");
  }
  else
  {
    Serial.print(lon*(-1), 4);
    Serial.println(" W.");
  }
}

double BoatDrone::Haversine(double lat_start, double lon_start, double lat_end, double lon_end)
{
	// convert coordinates from degrees to radians
	double lat_start_rad = lat_start*pi/180.0;
	double lon_start_rad = lon_start*pi/180.0;
	double lat_end_rad = lat_end*pi/180.0;
	double lon_end_rad = lon_end*pi/180.0;

  	// haversine formula
  	double haversine =  2.0*r*asin(sqrt(sq(sin((lat_end_rad - lat_start_rad)/2.0))+cos(lat_start_rad)*cos(lat_end_rad)*sq(sin((lon_end_rad - lon_start_rad)/2.0)))) - 5;
  	if (haversine <= 0.0) haversine = 0.0;
  
  return haversine;
}

int BoatDrone::GetHeading(double y, double x, double declinationAngle)
{
	double heading = atan2(y, x);
  	heading += declinationAngle;
  
  	// Correct for when signs are reversed.
 	 if(heading < 0)
    	heading += 2*pi;
    
  	// Check for wrap due to addition of declination.
  	if(heading > 2*pi)
   		heading -= 2*pi;
   	
   	// Convert radians to degrees for readability.
   	double headingDegrees = heading*180.0/pi;
   
 	return (int)headingDegrees;
}

void BoatDrone::PID(int heading, int bearing, int *e1, int *e2, int *e3, int *u1, int *u)
{
  	// update variables
  	*e3 = *e2;
  	*e2 = *e1;
  	*u1 = *u;
  
  	// steering algorithm
 	if (bearing < 180)
 	{
 		opp_bearing = bearing + 180;
 		if (heading - opp_bearing < 0)
 			*e1 = heading - bearing;
 		else
 			*e1 = -1*(360-heading+bearing);
	}
	else
	{
		opp_bearing = bearing - 180;
		if (heading - opp_bearing < 0)
			*e1 = 360 - bearing + heading;
		else
			*e1 = heading - bearing;
	}
 	
 	// PID calculation
  	*u = k1*(*e1) + k2*(*e2) + k3*(*e3);
  	// original: *u = *u1 + k1*(*e1) + k2*(*e2) + k3*(*e3);
  	// removed u1 to improve steady state error and overshoot
  	// but at the cost of rise time
  	
  	// bound u
  	if (*u > uMAX) *u = uMAX;
  	if (*u < uMIN) *u = uMIN;
  
  	// new values are passed by reference
}

void BoatDrone::DCmotor(int motor_speed_percent, int maxSpeed, int motorPin)
{
	// bound motor speed percent
	if (motor_speed_percent > maxSpeed) motor_speed_percent = maxSpeed;
	if (motor_speed_percent < 0) motor_speed_percent = 0;
  	//Serial.print(motor_speed_percent); Serial.println(" %");
  	
  	// write speed to motor
  	motor_real_speed = map(motor_speed_percent, 0, maxSpeed, 0, 255);
  	analogWrite(motorPin, motor_real_speed);
}


