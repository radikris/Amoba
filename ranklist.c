#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ranklist.h"
#include "structures.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"

void freelista(Ranklist *eleje)
{
	Ranklist *mozgo = eleje;
	while (mozgo != NULL)
	{
		Ranklist *kov = mozgo->next;
		free(mozgo);
		mozgo = kov;
	}
}

void sorting(Ranklist *eleje)
{
	/**
	Buborekrendezes lancolt listan: mindig csak az egymas melletti elemeket cserelgetjuk, a pontok nagysaga alapjan
	Ezzel rendezzuk az egesz listat novekvo sorrendbe, es igy ha csak az elso 10, 50, x elemet akarjuk kiirni, csak ezt
	a szamot kell modositani a kiir fgvben
	*/
	Ranklist * mozg;
	int temp;
	char tempnev[31];
	while (eleje->next != NULL)
	{
		mozg = eleje->next;
		while (mozg != NULL)
		{
			if (eleje->pontszam > mozg->pontszam)
			{
				temp = eleje->pontszam;
				strcpy(tempnev, eleje->nev);

				eleje->pontszam = mozg->pontszam;
				strcpy(eleje->nev, mozg->nev);

				mozg->pontszam = temp;
				strcpy(mozg->nev, tempnev);
			}

			mozg = mozg->next;
		}

		eleje = eleje->next;
	}
}

void elejere(Ranklist **eleje, FILE *p)
{
	/**
	Dupla indirekcioval vesszuk at a lancolt listankat, ezert nem kell visszaterni a lista elejevel
	Minden egyes alkalommal, a filebol beolvasott nev es pont part szurjuk a lista elejere
	*/
	char eloszor[50];
	while (!feof(p))
	{
		fscanf(p, " %[^\n]", eloszor);
		Ranklist *uj = (Ranklist*) malloc(sizeof(Ranklist));
		sscanf(eloszor, "%s %d", uj->nev, &uj->pontszam);
		uj->next = *eleje;
		*eleje = uj;
	}
}

void kiirlist(Ranklist *eleje)
{
	/**
	Ez a fgv mar a rendezett listat veszi at parameterkent, es igy csak az elso 10, 50, x elemet irja ki, amit mi megadunk neki
	En ezesetben az elso 10-et kivanom kiirni
	*/
	printf("\tTOP 10\n");
	printf("**************************\n");
	for (int i = 0; i < 10 && eleje != NULL; i++)
	{
	    if(i!=9){
            printf("%d  | %13s : %2d pont\n", i + 1, eleje->nev, eleje->pontszam);
	    }
	    else{
            printf("%d |  %12s : %2d pont\n", i + 1, eleje->nev, eleje->pontszam);
	    }
		eleje = eleje->next;
	}
}
