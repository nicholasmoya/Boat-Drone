/*

Nicholas Moya
SRNL - Boat Drone
Dr. Tad Whiteside
7/4/2016

BOAT DRONE - SOURCE CODE (.h)
---------------------------------------------------------------------------
This is the source code .h file for the Boat Drone main arduino .ino file.
Five functions, and several variables, are declared here, wrapped in the
class BoatDrone, and referenced in the main code. The functions are:

	DisplayCoordinates - prints coordinates to the serial monitor
	Haversine - uses the haversine formula to calculate and return the
				distance between two coordinates
	GetHeading - determines the current heading
	PID - calculates the control variable, u, needed to steer the boat
	DCmotor - sets the speed of a DC motor

These functions are explicity defined in the .cpp source code file.

*/

#ifndef BoatDrone_H
#define BoatDrone_H

class BoatDrone
{
		// PID variables shared with main
		int error3;
		int error2;
		int error1;
		int u1;
		int u;
		
	public:
		void DisplayCoordinates(double lat, double lon);
		double Haversine(double lat_start, double lon_start,
						 double lat_end, double lon_end);
		int GetHeading(double y, double x, double declinationAngle);
		void PID(int heading, int bearing, int *e1, int *e2, 
					int *e3, int *u1, int *u);
		void DCmotor(int motor_speed_percent, int maxSpeed, int motorPin);
					
	private:
		const double r = 6371000.0; // radius of the earth in meters
		const double pi = 3.1416;
		
		// PID Parameters
		const int uMAX =  50; // This value must be such that this holds:
		const int uMIN = -50; // 100 = base_percent_speed +/- uMax/uMin.
		const double kp = 1.292;
		const double ki = 1.113;
		const double kd = -0.282;
		double k1 = kp + ki + kd;
		double k2 = -kp - 2*kd;
		double k3 = kd;
		int opp_bearing;
		
		// DC Motor variable
		int motor_real_speed;
};

#endif
