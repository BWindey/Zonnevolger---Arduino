#ifndef ARRAY
#define ARRAY
#include <AFArray.h>
#endif

#ifndef ARDUINO
#define ARDUINO
#include <Arduino.h>
#endif


#ifndef RGB_LEDS
#define RGB_LEDS

// Alle variabelen die te maken hebben met de tijd bijhouden:
long unsigned int prev_time;
long unsigned int cur_time;
AFArray<int> kleurhemel;
bool lange_maanden[] = {true, false, true, false, true, false, true, true, false, true, false, true};


// De analoge LED-uitgangen
#define LED_1 14
#define LED_2 17
#define LED_3 20

// Digitale pinnen voor transistors
#define TRANS_1 2
#define TRANS_2 3
#define TRANS_3 4
#define TRANS_4 5
#define TRANS_5 6
#define TRANS_6 7
#define TRANS_7 8


int palet1[5][3] = { {255, 0, 0}, {255, 77, 0}, {255, 103, 0}, {255, 129, 0} };
int palet2[5][3] = { {255, 192, 135}, {255, 168, 151}, {255, 140, 147}, {255, 95, 157}};
int palet3[5][3] = { {199, 56, 102}, {254, 103, 110}, {253, 143, 82}, {255, 189, 113}};
int palet4[5][3] = { {252, 113, 116}};

void RGB_color(int LED, int red_light_value, int green_light_value, int blue_light_value, int brightness);
void switch_serial_LEDs();
void update_time();
bool is_schrikkeljaar();
void get_sun_set_rise();
bool nacht();
void update_leds();

#endif