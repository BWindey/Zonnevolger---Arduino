#include "RGB_leds.h"
#include "gebruiker.h"
#include "suncalc.h"


void setup() 
{
  // pinMode(LED_1, OUTPUT);       // Red
  // pinMode(LED_1 + 1, OUTPUT);   // Green
  // pinMode(LED_1 + 2, OUTPUT);   // Blue

  pinMode(LED_2, OUTPUT);       // Red
  pinMode(LED_2 + 1, OUTPUT);   // Green
  pinMode(LED_2 + 2, OUTPUT);   // Blue
  
  pinMode(LED_3, OUTPUT);

  // Alle digitale pins voor de transistors
  pinMode(TRANS_1, OUTPUT);
  pinMode(TRANS_2, OUTPUT);
  pinMode(TRANS_3, OUTPUT);
  pinMode(TRANS_4, OUTPUT);
  pinMode(TRANS_5, OUTPUT);
  pinMode(TRANS_6, OUTPUT);
  pinMode(TRANS_7, OUTPUT);

  digitalWrite(TRANS_1, HIGH);
  digitalWrite(TRANS_2, HIGH);
  digitalWrite(TRANS_3, HIGH);
  digitalWrite(TRANS_4, HIGH);
  digitalWrite(TRANS_5, HIGH);
  digitalWrite(TRANS_6, HIGH);
  digitalWrite(TRANS_7, HIGH);

  prev_time = millis();
  delay(20);
  cur_time = millis();
  get_sun_set_rise();
}


void loop() 
{
  
}


void RGB_color(int LED, int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(LED, red_light_value);
  analogWrite(LED + 1, green_light_value);
  analogWrite(LED + 2, blue_light_value);
}


void alternerend_groen_rood_kerstsfeer()
{
  int foo = 0;
  while(true)
  {
    if(foo <= 300)
    {
      RGB_color(LED_2, 255, 0, 0);
      digitalWrite(TRANS_1, LOW);
      delay(1);
      digitalWrite(TRANS_1, HIGH);

      RGB_color(LED_2, 0, 255, 0);
      digitalWrite(TRANS_2, LOW);
      delay(1);
      digitalWrite(TRANS_2, HIGH);

      RGB_color(LED_2, 255, 0, 0);
      digitalWrite(TRANS_3, LOW);
      delay(1);
      digitalWrite(TRANS_3, HIGH);

      RGB_color(LED_2, 0, 255, 0);
      digitalWrite(TRANS_4, LOW);
      delay(1);
      digitalWrite(TRANS_4, HIGH);

      RGB_color(LED_2, 255, 0, 0);
      digitalWrite(TRANS_5, LOW);
      delay(1);
      digitalWrite(TRANS_5, HIGH);

      RGB_color(LED_2, 0, 255, 0);
      digitalWrite(TRANS_6, LOW);
      delay(1);
      digitalWrite(TRANS_6, HIGH);

      RGB_color(LED_2, 255, 0, 0);
      digitalWrite(TRANS_7, LOW);
      delay(1);
      digitalWrite(TRANS_7, HIGH);
    }

    else
    {
      RGB_color(LED_2, 0, 255, 0);
      digitalWrite(TRANS_1, LOW);
      delay(1);
      digitalWrite(TRANS_1, HIGH);

      RGB_color(LED_2, 255, 0, 0);
      digitalWrite(TRANS_2, LOW);
      delay(1);
      digitalWrite(TRANS_2, HIGH);

      RGB_color(LED_2, 0, 255, 0);
      digitalWrite(TRANS_3, LOW);
      delay(1);
      digitalWrite(TRANS_3, HIGH);

      RGB_color(LED_2, 255, 0, 0);
      digitalWrite(TRANS_4, LOW);
      delay(1);
      digitalWrite(TRANS_4, HIGH);

      RGB_color(LED_2, 0, 255, 0);
      digitalWrite(TRANS_5, LOW);
      delay(1);
      digitalWrite(TRANS_5, HIGH);

      RGB_color(LED_2, 255, 0, 0);
      digitalWrite(TRANS_6, LOW);
      delay(1);
      digitalWrite(TRANS_6, HIGH);

      RGB_color(LED_2, 0, 255, 0);
      digitalWrite(TRANS_7, LOW);
      delay(1);
      digitalWrite(TRANS_7, HIGH);
    }
    foo++;
    if(foo >= 600)
      foo = 0;
  }
}


void update_time()
{
  cur_time = millis();
  if(cur_time - prev_time >= 3600000/snelheid)
  {
    prev_time = cur_time;
    minuut++;

    if(minuut >= 60)
    {
      uur++;
      minuut -= 60;

      if(uur >= 24)
      {
        dag++;
        uur -= 24;
        get_sun_set_rise();

        if(maand == 2)
        {
          if(is_schrikkeljaar() && dag == 29)
          {
            maand++;
            dag -= 29;
          }
          else if(!is_schrikkeljaar() && dag >= 28)
          {
            maand++;
            dag -= 28;
          }
        }
        else if(!lange_maanden[maand] && dag >= 30)
        {
          maand++;
          dag -= 30;          
        }
        else
        {
          maand ++;
          dag -= 31;          
        }

        if(maand >= 12)
        {
          jaar++;
          maand -= 12;
        }
      }
    }
  }  
}


bool is_schrikkeljaar()
{
  if(jaar % 4 != 0) {return false; }
  if(jaar % 400 == 0) {return true; }
  if(jaar % 100 == 0) {return false; }
  return true;
}


void get_sun_set_rise()
{
  get_rise_set_dawn_dusk(latitude, longitude, jaar, maand, dag, &kleurhemel);
}



bool nacht()
{
  if((kleurhemel[6] < uur && kleurhemel[7] < minuut) || (kleurhemel[0] > uur && kleurhemel[1] > minuut))
  {
    return true;
  }

  return false;
}

bool in_interval(int a, int b, int c)
{
  return (c >= a) && (c <= b);
}

bool in_interval(double a, double b, double c)
{
  return (c >= a) && (c <= b);
}


void update_leds()
{
  update_time();
  // daglengte in minuten
  daglengte = kleurhemel[6] * 60 + kleurhemel[7] - (kleurhemel[0] * 60 + kleurhemel[1]);
  int interval_lengte = int(daglengte / 7);
   
  int intervallen[7][2];
  for(int i = 0; i < 7; i++)
  {
    // De nacht kan tot 6m te vroeg invallen doordat we enkel met gehele getallen werken.
    intervallen[i][0] = interval_lengte * i;
    intervallen[i][1] = interval_lengte * (i + 1);
  }
}



