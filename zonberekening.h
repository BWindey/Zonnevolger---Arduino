// Arduino library version of sunrise/sunset calculations by 'jurs' based on:
// A simple C++ program calculating the sunrise and sunset for
// the current date and a set location (latitude,longitude)
// e-mail: jjlammi@yahoo.com
// C++ program calculating the sunrise and sunset for
// the current date and a constant location(latitude,longitude)
// Jarmo Lammi 1999 - 2004
// Last update
// Jan 3rd, 2004 - change to compile with gcc-3.3.2 by Stephan Wynhoff
// http://wynhoff.home.cern.ch/wynhoff/weather/gex.html


#ifndef SUNCALC_H
#define SUNCALC_H

#ifndef ARRAY 
#define ARRAY
#include <AFArray.h>
#endif

#include <Arduino.h>

#define SUNDIAMETER 0.53  // Sun diameter in degrees
#define AIRREFRACTION (34.0/60.0) // athmospheric refraction in degrees


void get_rise_set_dawn_dusk(double Lat, double Lon, int year, int month, int day, AFArray<int> *kleurhemel);
double FNday (int y, int m, int d, float h);
double FNrange (double x);
double f0(double lat, double declin);
double f1(double lat, double declin);
double FNsun (double d, double &L);
void sunCalc(int year, int month, int day, int timezone, double latitude, double longitude, int *sunrise, int *sunset);
int to_hour(int x);
int to_minute(int x);

#endif