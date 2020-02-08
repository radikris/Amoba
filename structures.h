#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>

typedef struct{
    char nicknev[31];
    int pontszam;
}Gyoztes;

typedef struct{
    char karakter;
    bool vege;
}Jatek;

typedef struct{
    //int veszely;
    //int lehetoseg;
    int sor;
    int oszlop;
    int mennyi;
    bool helyzet;
}Mezo;

typedef struct {
    char **tomb;
    int meret;
}Palya;


enum { GYOZ = 5 };

#endif // STRUCTURES_H
