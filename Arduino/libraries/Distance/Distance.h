#ifndef Distance_H
#define Distance_H

class Distance
{
	public:
		void DisplayCoordinates(double lat, double lon);
		double Haversine(double lat_start, 
						 double lon_start,
						 double lat_end,
						 double lon_end);
	private:
		const double r = 6371000.0;
		const double pi = 3.1416;
};

#endif
