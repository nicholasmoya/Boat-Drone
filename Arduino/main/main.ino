/*  Nicholas Moya
 *  SRNL - Boat Drone
 *  Dr. Tad Whiteside
 *  7/4/2016
 *   
 *  BOAT DRONE - MAIN
 *  ------------------------------
 *  This code steers a boat toward multiple waypoints, starting with the first 
 *  one in a sequence and ending with the last one in the sequence. It 
 *  accomplishes this by using a GPS module, a Compass module, two DC motors, 
 *  and the BoatDrone library. The code works in these steps:
 *  
 *    1. Determine its current location (GPS)
 *    2. Determine its current heading (Compass)
 *    3. Calculate the angle, direction, and distance between its current position and the waypoint (BoatDrone)
 *    4. Using the DC motors, steer the boat to the waypoint until the waypoint is reached (DC Motors)
 *    5. Once the waypoint is reached, steer in th direction of the next waypoint
 *    6. Once all waypoints have been reached, turn off the DC motors.
 *    
 *  This will be done continuously, meaning that if the current postion changes,
 *  the speeds of the DC motors will change so that it continues to steer the boat to the next waypoint.
 *  
 *  CONNECTIONS:
 *  Compass:  SDA -> A4, SCL -> A5
 *  GPS:      RX -> D2, TX -> D4
 *  DC Motor: Left PWN -> D11, Right PWM -> D10
 *  Battery;  High -> VIN, Low -> GND
 */



// GPS LIBRARIES
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// COMPASS LIBRARIES
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

// BOATDRONE LIBRARY
#include <BoatDrone.h>



// OBJECTS
SoftwareSerial mySerial(4, 2); // Pins digital 4 (TX) and digital 2 (RX)
Adafruit_GPS GPS(&mySerial);
// Assign a unique ID to this sensor at the same time
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
BoatDrone prototype;



// GPS DECLARATIONS
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences. 
#define GPSECHO  false
// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy



// MY DECLARATIONS
//                                 {latitude, longitude}
//                  accurate to XX.XXXX____,  XX.XXXX____ places
const double waypoint[4][2] = {{33.34361936, -81.74101174},   // waypoint 1
                               {33.34399486, -81.7403841},    // waypoint 2
                               {33.34445751, -81.73954993},   // waypoint 3
                               {33.3439711, -81.73915431}};   // waypoint 4
// NOTE: The last waypoint must be where you are so that the boat comes back to you
const int number_of_waypoints = 4;
const double declinationAngle = 0.11222467; // constant, based on location, used in compass
double displacement; // distance from waypoint
double deltaY; // used to determine the desired heading
double deltaX; // used to determine the desired heading
int bearing; // compass heading (in degrees)
int heading; // The direction to the waypoint (in degrees)
bool reached_waypoint = false;
int i = 0;  // counter for waypoints

// PID declarations
int error3 = 0;
int error2 = 0;
int error1 = 0;
int u1 = 0;
int u = 0;

// DC motor declarations
const int base_percent_speed = 50;  // steady state speed
const int maxSpeed = 100;           // turning speed
const int RightMotorPin = 10;
const int LeftMotorPin = 11;
int right_motor_speed_percent = 0;
int left_motor_speed_percent = 0;



void setup() 
{
  
  // delay for 60 secs to make time for disconnecting the cable, closing the lid, and putting the boat in the water
  delay(60000);



  // **************************
  // *** INITIALIZE THE GPS ***
  // **************************
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  useInterrupt(true);
  delay(1000);
  mySerial.println(PMTK_Q_RELEASE);



  // ******************************
  // *** INITIALIZE THE COMPASS ***
  // ******************************
  if(!mag.begin())
  {
    while(1);
  }
}



// *******************************
// *** GPS: INTERUPT FUNCTIONS ***
// *******************************
SIGNAL(TIMER0_COMPA_vect) 
{
  char c = GPS.read();
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;
#endif
}

void useInterrupt(boolean v) 
{
  if (v) 
  {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else 
  {
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}



void loop() 
{

  while(i < number_of_waypoints)
  {
    do
    {


      
        // ******************************
        // *** GPS: PARSE COORDINATES ***
        // ******************************
        if (GPS.newNMEAreceived()) 
        {
          if (!GPS.parse(GPS.lastNMEA()))
            return;
        }

      
      
        // ****************************
        // *** COMPASS: GET BEARING ***
        // ****************************
        sensors_event_t event; 
        mag.getEvent(&event);
        bearing = prototype.GetHeading(event.magnetic.y, event.magnetic.x, declinationAngle);


      
        // **********************
        // *** GO TO WAYPOINT ***
        // **********************
        if (GPS.fix)
        {
          // get heading and distance to waypoint
          deltaY = waypoint[i][1] - GPS.longitudeDegrees;
          deltaX = waypoint[i][0] - GPS.latitudeDegrees;
          heading = prototype.GetHeading(deltaY, deltaX, declinationAngle);
          displacement = prototype.Haversine(GPS.latitudeDegrees, GPS.longitudeDegrees, waypoint[i][0], waypoint[i][1]);
        
          // use heading and bearing to get u from PID function
          prototype.PID(heading, bearing, &error1, &error2, &error3, &u1, &u);
          
          // use u to set speed of motors
          left_motor_speed_percent = base_percent_speed + u;
          right_motor_speed_percent = base_percent_speed - u;

          // impliment speed of motors to steer boat
          prototype.DCmotor(left_motor_speed_percent, maxSpeed, LeftMotorPin);
          prototype.DCmotor(right_motor_speed_percent, maxSpeed, RightMotorPin);
        }
        else
        {
          // stay still while fix is aquired
          analogWrite(LeftMotorPin, 0);
          analogWrite(RightMotorPin, 0);
        } 

        // has the waypoint been reached?
        if (displacement < 1.0)
          reached_waypoint = true;
        else
          reached_waypoint = false;
          
    } 
    while(reached_waypoint == false);

    // pause for 1 sec between waypoints
    analogWrite(LeftMotorPin, 0);
    analogWrite(RightMotorPin, 0);
    delay(1000);

    // queue up next waypoint
    i++;
    
  }

  // all waypoints have been reached
  done();
}

// stay still after all waypoints have been reached
void done()
{
  while(1)
  {
    analogWrite(LeftMotorPin, 0);
    analogWrite(RightMotorPin, 0);
  }
}
