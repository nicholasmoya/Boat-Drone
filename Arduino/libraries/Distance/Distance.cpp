#include "Arduino.h"
#include "Distance.h"

void Distance::DisplayCoordinates(double lat, double lon)
{
  if (lat > 0)
  {
    Serial.print(lat, 6);
    Serial.print(" N, ");
  }
  else
  {
    Serial.print(lat*(-1), 6);
    Serial.print(" S, ");
  }
  
  if (lon > 0)
  {
    Serial.print(lon, 6);
    Serial.println(" E.");
  }
  else
  {
    Serial.print(lon*(-1), 6);
    Serial.println(" W.");
  }
}

double Distance::Haversine(double lat_start, double lon_start, double lat_end, double lon_end)
{
	// convert coordinates from degrees to radians
	double lat_start_rad = lat_start*pi/180.0;
	double lon_start_rad = lon_start*pi/180.0;
	double lat_end_rad = lat_end*pi/180.0;
	double lon_end_rad = lon_end*pi/180.0;

  // haversine formula
  return 2.0*r*asin(sqrt(sq(sin((lat_end_rad - lat_start_rad)/2.0))+cos(lat_start_rad)*cos(lat_end_rad)*sq(sin((lon_end_rad - lon_start_rad)/2.0))));
}
