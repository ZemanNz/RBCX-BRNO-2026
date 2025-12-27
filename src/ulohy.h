#ifndef ULOHY_H
#define ULOHY_H

#include <Adafruit_TCS34725.h>

extern Adafruit_TCS34725 tcs;

// Helper functions for maze navigation
bool is_free_right(int threshold = 250);
bool is_free_front(int threshold = 250);
bool is_free_left(int threshold = 250);

bool cervena();

void sprint(int distance);
void slalom(bool right);
void medved();
void kulicky();
void bludiste();

#endif // ULOHY_H
