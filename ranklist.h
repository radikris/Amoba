#ifndef RANKLIST_H_INCLUDED
#define RANKLIST_H_INCLUDED

typedef struct Ranklist{
    char nev[31];
    int pontszam;
    struct Ranklist *next;
}Ranklist;

void freelista(Ranklist * eleje);

void sorting(Ranklist *eleje);

void elejere(Ranklist **eleje, FILE *p);

void kiirlist(Ranklist *eleje);

#endif // RANKLIST_H_INCLUDED

