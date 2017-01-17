#include "Arduino.h"
#include "PID.h"

void PID::PIDcalc(double measuredDistance, double *e1, double *e2, 
					double *e3, double *u1, double *u)
{
  // update variables
  *e3 = *e2;
  *e2 = *e1;
  *u1 = *u;
  
  // do PID calculations
  *e1 = idealDistance - measuredDistance;
  *u = *u1 + k1*(*e1) + k2*(*e2) + k3*(*e3);
  
  // bound u
  if (*u > uMAX) *u = uMAX;
  if (*u < uMIN) *u = uMIN;
  
  // new values are passed by reference
}