#ifndef PID_h
#define PID_h

#include "Arduino.h"

class PID
{
	double error3;
	double error2;
	double error1;
	double u1;
	double u;
  public:
	void PIDcalc(double measuredDistance, double *e1, double *e2, 
					double *e3, double *u1, double *u);
  private:
	const double uMAX = 100.0;
	const double uMIN = 0.0;
	const int idealDistance = 1;
	const double kp = 1.0;
	const double ki = 2.0;
	const double kd = 4.0;
	double k1 = kp + ki + kd;
	double k2 = -kp - 2*kd;
	double k3 = kd;
};

#endif