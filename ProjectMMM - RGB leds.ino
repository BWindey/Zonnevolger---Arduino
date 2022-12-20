#include "RGB_leds.h"
#include "gebruiker.h"

void setup() 
{
  for(int i = 0; i < 10; i++)
  {
    pinMode(pinnen[i], OUTPUT);
    if(i <= 8)
      digitalWrite(i, HIGH);
  }
  vorig = millis();
  delay(10);
  huidig = millis();
  haal_zonsopkomst_ondergang();
}

//type een van de volgende functies en verwijder de bestaande binnen loop():
// multiplex_voorbeeld();
// zonne_volger();
// foto();
// alternerend_groen_rood_kerstsfeer();
void loop() 
{
  zonne_volger();
}

void RGB_kleur(int rood_licht, int groen_licht, int blauw_licht)
{
  analogWrite(LED, rood_licht);
  analogWrite(LED + 1, groen_licht);
  analogWrite(LED + 2, blauw_licht);
}

void multiplex_voorbeeld()
{
  for(int i = 0; i < 3; i++)
  {
    hulp_multiplex_voorbeeld(100);
  }
  for(int i = 0; i < 15; i++)
  {
    hulp_multiplex_voorbeeld(20);
  }
  for(int i = 0; i < 1500; i++)
  {
    hulp_multiplex_voorbeeld(1);
  }
}

void hulp_multiplex_voorbeeld(int pause)
{
  int kleur[3] = {255, 0, 0};
  for(int i = 0; i < 7; i++)
  {
    RGB_kleur(kleur[0], kleur[1], kleur[2]);
    digitalWrite(i + 2, LOW);
    delay(pause);
    digitalWrite(i + 2, HIGH);

    int temp = kleur[0];
    kleur[0] = kleur[1];
    kleur[1] = kleur[2];
    kleur[2] = temp;
  }
}

void foto()
{
  int kleur[3] = {255, 0, 0};
  for(int i = 0; i < 7; i++)
  {
    if(i % 2 == 1)
    {
      RGB_kleur(kleur[0], kleur[1], kleur[2]);
      digitalWrite(i + 2, LOW);
      // Schuif de waarden van kleur op
      int temp = kleur[2];
      kleur[2] = kleur[1];
      kleur[1] = kleur[0];
      kleur[0] = temp;
      delay(1);
      digitalWrite(i+2, HIGH);
    }
    else
      digitalWrite(i+2, HIGH);
  }
}

void alternerend_groen_rood_kerstsfeer()
{
  huidig = millis();
  int speed = 500;
  if(huidig - vorig >= speed)
  {
    int kleur[2] = {0, 255};
    for(int i = 0; i < 7; i++)
    {
      RGB_kleur(kleur[0], kleur[1], 0);
      digitalWrite(i + 2, LOW);
      delay(1);
      digitalWrite(i + 2, HIGH);

      int temp = kleur[0];
      kleur[0] = kleur[1];
      kleur[1] = temp;
    }
  }
  else
  {
    int kleur[2] = {255, 0};
    for(int i = 0; i < 7; i++)
    {
      RGB_kleur(kleur[0], kleur[1], 0);
      digitalWrite(i + 2, LOW);
      delay(1);
      digitalWrite(i + 2, HIGH);

      int temp = kleur[0];
      kleur[0] = kleur[1];
      kleur[1] = temp;
    }
  }
  if(huidig - vorig >= 2 * speed)
    vorig = huidig;
}

void update_tijd()
{
  huidig = millis();
  if((huidig - vorig) * snelheid >= 3600000)
  {
    vorig = huidig;
    minuut++;

    if(minuut >= 60)
    {
      uur++;
      // Serial.println(uur);
      minuut -= 60;

      if(uur >= 24)
      {
        dag++;
        uur -= 24;
        haal_zonsopkomst_ondergang();

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

void haal_zonsopkomst_ondergang()
{
  get_rise_set_dawn_dusk(latitude, longitude, jaar, maand, dag, &kleurhemel);
}

bool in_interval(int a, int b, int c)
{
  return (c >= a) && (c <= b);
}

bool in_interval(double a, double b, double c)
{
  return (c >= a) && (c <= b);
}

int deel_van_dag(int intervallen[7][2])
{
  int tijd = uur * 60 + minuut;

  for(int i = 0; i < 7; i++)
  {
    if(tijd > intervallen[i][0] && tijd < intervallen[i][1])
      return i;
  }
  return 10;
}

void zonne_volger()
{
  update_tijd();
  // daglengte in minuten
  int daglengte = kleurhemel[6] * 60 + kleurhemel[7] - (kleurhemel[0] * 60 + kleurhemel[1]);
  int interval_lengte = int(daglengte / 7);
  int zonsopkomst = kleurhemel[0] * 60 + kleurhemel[1];
  int intervallen[7][2];

  for(int i = 0; i < 7; i++)
  {
    // De nacht kan tot 6m te vroeg invallen doordat we enkel met gehele getallen werken.
    intervallen[i][0] = zonsopkomst + interval_lengte * i;
    intervallen[i][1] = zonsopkomst + interval_lengte * (i + 1);
  }

  int deel_dag = deel_van_dag(intervallen);
  if(deel_dag == 10)
  {
    RGB_kleur(0, 0, 0);
    return;
  }

  for(int i = 0; i < 7; i++)
  {
    if(i == deel_dag)
    {
      RGB_kleur(255, 255, 0);
      digitalWrite(i + TRANS_1, LOW);
      delay(1);
      digitalWrite(i + TRANS_1, HIGH);
    }
    else
    {
      RGB_kleur(0, 0, 255);
      digitalWrite(i + TRANS_1, LOW);
      delay(1);
      digitalWrite(i + TRANS_1, HIGH);
    }
  }
}



