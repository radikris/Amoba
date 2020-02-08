#ifndef JATEKVEZERLES_H
#define JATEKVEZERLES_H

#include "structures.h"

typedef enum Irany{
        vizjobb, fuggle, atlojobble, atloballe
}Irany;

bool szabade(Palya * palya, int x, int y);

bool ellenoriz(int i, int j, int sori, int oszlopi, char **tomb, Mezo *vissza, int *szamol, bool *bennevan, int limit, Irany irany, int palyameret, int gyokeri, int gyokerj);

Mezo ujtaktika(Palya * palya, int limit, char babu);

void veszelyes(Palya * palya);

#endif // JATEKVEZERLES_H
