#ifndef KERET_H
#define KERET_H

#include "structures.h"

bool vege(char **tomb, int * szamol, Jatek *vissza, int feltetx, int feltety, int i, int j);

Jatek jatekvege(Palya palya);

void lepj(Palya * palya, Gyoztes jatekos);

void elso(Palya * palya);


#endif // KERET_H
