#include "suncalc.h"

void get_rise_set_dawn_dusk(double Lat, double Lon, int year, int month, int day, AFArray<int> *kleurhemel)
{
  int TZ = +1 ; //west negative
  int sunrise,sunset,dawn,dusk;

  AFArray<int> suntimes;
  sunCalc(year, month, day, TZ, Lat, Lon, &sunrise, &sunset);

  // Start zonsopkomst
  suntimes.add(to_hour(sunrise));
  suntimes.add(to_minute(sunrise));
  // Stop zonsopkomst
  suntimes.add(to_hour(sunrise + 15));
  suntimes.add(to_minute(sunrise + 15));

  // Start zonsondergang
  suntimes.add(to_hour(sunset - 15));
  suntimes.add(to_minute(sunset - 15));
  // Stop zonsondergang  
  suntimes.add(to_hour(sunset));
  suntimes.add(to_minute(sunset));

  *kleurhemel = suntimes;
}

double FNday (int y, int m, int d, float h)
{
  //   Get the days to J2000
  //   h is UT in decimal hours
  //   FNday only works between 1901 to 2099 - see Meeus chapter 7
  long int count = - 7 * (y + (m + 9)/12)/4 + 275*m/9 + d;
  // type casting necessary on PC DOS and TClite to avoid overflow
  count+= (long int)y*367;
  return (double)count - 730531.5 + h/24.0;
}

double FNrange (double x)
{
  //  the function returns an angle in the range 0 to 2*PI
  double b = x / TWO_PI;
  double a = TWO_PI * (b - (long)(b));
  if (a < 0) a = TWO_PI + a;
  return a;
}

double f0(double lat, double declin)
{
  // Calculating the hourangle
  double fo,dfo;
  // Correction: different sign at S HS
  dfo = DEG_TO_RAD*(0.5*SUNDIAMETER + AIRREFRACTION); if (lat < 0.0) dfo = -dfo;
  fo = tan(declin + dfo) * tan(lat*DEG_TO_RAD);
  if (fo>0.99999) fo=1.0; // to avoid overflow //
  fo = asin(fo) + PI/2.0;
  return fo;
}

double f1(double lat, double declin)
{
  // Calculating the hourangle for twilight times
  double fi,df1;
  // Correction: different sign at S HS
  df1 = DEG_TO_RAD * 6.0; if (lat < 0.0) df1 = -df1;
  fi = tan(declin + df1) * tan(lat*DEG_TO_RAD);
  if (fi>0.99999) fi=1.0; // to avoid overflow //
  fi = asin(fi) + PI/2.0;
  return fi;
}

double FNsun (double d, double &L)
{
  //   Find the ecliptic longitude of the Sun
  double g;
  //   mean longitude of the Sun
  L = FNrange(280.461 * DEG_TO_RAD + .9856474 * DEG_TO_RAD * d);

  //   mean anomaly of the Sun
  g = FNrange(357.528 * DEG_TO_RAD + .9856003 * DEG_TO_RAD * d);

  //   Ecliptic longitude of the Sun
  return FNrange(L + 1.915 * DEG_TO_RAD * sin(g) + .02 * DEG_TO_RAD * sin(2 * g));
};

void sunCalc(int year, int month, int day, int timezone, double latitude, double longitude, int *sunrise, int *sunset)
// calculates times of sunrise, sunset
{
  double L, daylen;
  double h = 12; // assume sun position at high noon
  double d = FNday(year, month, day, h);

  //   Use FNsun to find the ecliptic longitude of the Sun
  double lambda = FNsun(d, L);

  //   Obliquity of the ecliptic
  double obliq = 23.439 * DEG_TO_RAD - .0000004 * DEG_TO_RAD * d;

  //   Find the RA and DEC of the Sun
  double alpha = atan2(cos(obliq) * sin(lambda), cos(lambda));
  double delta = asin(sin(obliq) * sin(lambda));

  // Find the Equation of Time
  // in minutes
  // Correction suggested by David Smith
  double LL = L - alpha;
  if (L < PI) LL += TWO_PI;
  double equation = 1440.0 * (1.0 - LL / TWO_PI);
  double ha = f0(latitude,delta);
  double hb = f1(latitude,delta);
  double twx = hb - ha; // length of twilight in radians
  twx = 12.0*twx/PI;    // length of twilight in hours
  // Conversion of angle to hours and minutes //
  daylen = RAD_TO_DEG*ha/7.5;
  if (daylen<0.0001) {daylen = 0.0;}
  // arctic winter //

  double riset = 12.0 - 12.0 * ha/PI + timezone - longitude/15.0 + equation/60.0;
  double settm = 12.0 + 12.0 * ha/PI + timezone - longitude/15.0 + equation/60.0;

  if (riset > 24.0) riset -= 24.0;
  if (settm > 24.0) settm -= 24.0;

  *sunrise=round(riset*60); //minutes past midnight
  *sunset=round(settm*60);
}

int to_hour(int x)
{
  return int(x/60);
}

int to_minute(int x)
{
  return x % 60;
}