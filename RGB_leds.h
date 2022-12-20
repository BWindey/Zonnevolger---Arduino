#ifndef ARRAY
#define ARRAY
#include <AFArray.h>
#endif

#ifndef RGB_LEDS
#define RGB_LEDS

#include "zonberekening.h"

// Alle variabelen die te maken hebben met de tijd bijhouden:
long unsigned int vorig;
long unsigned int huidig;
AFArray<int> kleurhemel;
bool lange_maanden[] = {true, false, true, false, true, false, true, true, false, true, false, true};
int pinnen[10] = {17, 18, 19, 2, 3, 4, 5, 6, 7, 8};


// De analoge LED-uitgangen
#define LED 17


// Digitale pinnen voor transistors
#define TRANS_1 2
#define TRANS_2 3
#define TRANS_3 4
#define TRANS_4 5
#define TRANS_5 6
#define TRANS_6 7
#define TRANS_7 8

void RGB_kleur(int rood_licht, int groen_licht, int blauw_licht);
void update_tijd();
bool is_schrikkeljaar();
void haal_zonsopkomst_ondergang();
bool nacht();
void zonne_volger();
void alternerend_groen_rood_kerstsfeer();
void foto();

#endif