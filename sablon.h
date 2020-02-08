#ifndef SABLON_H
#define SABLON_H

#include "structures.h"
#include "tombkezelo.h"

void delay(double masodperc);

void loading();

void mentes(Palya *palya, Gyoztes jatekos);

void betolt(Palya * palya, Gyoztes *jatekos);

void clrscr();

void top10(FILE *fp);

bool menu();

#endif // SABLON_H
