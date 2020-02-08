#ifndef TOMBKEZELO_H
#define TOMBKEZELO_H

#include "structures.h"

void felszabadit(char **tomb);

void kiir(Palya palya);

void nullaz(Palya * palya);

void atir(Palya * palya);

bool szelso_ket_sor_oszlop(int kulsofor, int kulsofeltet, int belsofor, int belsofeltet, char **tomb);

bool peremterulet(Palya palya);

#endif // TOMBKEZELO
