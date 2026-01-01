#ifndef ULOHY_H
#define ULOHY_H

#include <Adafruit_TCS34725.h>

extern Adafruit_TCS34725 tcs;

extern int zadek_od_stredu;
extern int predek_od_stredu;
extern int jedno_pole;
extern int od_steny_na_stred_pole;


// Helper functions for maze navigation
bool is_free_right();
bool is_free_front();
bool is_free_left();

void srovnej_se_v_pravo();
void srovnej_se_v_levo();
void zavrit_klepeto();
void otevrit_klepeto();
void vysun_zhazovadlo();
void zasun_zhazovadlo();
void srovnani();

void srovnej_na_caru();

void sledovani_cary();

bool cervena();

void sprint_m_d();
void sprint_cara();
void slalom(bool right);
void medved();
void kulicky();
void bludiste();

#endif // ULOHY_H
