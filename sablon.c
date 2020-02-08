#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "econio.h"
#include "sablon.h"
#include "ranklist.h"
#include "tombkezelo.h"
#include "structures.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"

void delay(double masodperc)
{
	/**
	A kesleltetes eleresehez a gep szamolja az idot, csak ezt masodpercbe kell valtani
	*/
	// A secundumot milli_secondumba konvertalni
	double millimasodperc = 1000 * masodperc;

	clock_t start_time = clock();

	// addig noveljuk az idot, amig el nem erjuk a konvertalt millisecet
	while (clock() < start_time + millimasodperc);
}

void loading()
{
	printf("A palya generalasa:\n");
	int szazalek = 0;
	for (int i = 25; i > 0; i--)
	{
		szazalek += 4;
		printf("[");
		for (int j = 0; j < 25 - i; j++)
		{
			printf("#");
		}

		for (int k = 0; k < i; k++)
		{
			printf(".");
		}

		printf("]");
		printf("   %d", szazalek);
		printf("%%");
		printf("\n");
		printf("Toltes folyamatban");
		delay(0.05);
		if (szazalek == 100)
			delay(0.1);
		clrscr();
	}

	printf("A palya generalasa sikeres volt!\n");
	clrscr();
}

void mentes(Palya *palya, Gyoztes jatekos)
{
	/**
	Minden kor elejen van dontese a jatekosnak mit szeretne, menteni(i/I), folytatni(n/N), kilepni(x/X)
	Ha el akarjuk menteni a jatekot, akkor van lehetosegunk ezzel a fuggvennyel fileba kiirni azt
	A fileban ebben a sorrendben lesznek az adatok: nev, eddig megtett lepes, palya merete, es a mezok (X=2, O=1, ures=0)
	Ha nem akarjuk menteni folytatodik a jatek, ha pedig kilepunk, akkor felszabaditjuk a tombot, es azonnal vege a jateknak
	*/
	int n = palya->meret;
	FILE * file;
	hiba:
		printf("Szeretned az allast elmenteni, es kesobb folytatni? (I/N)\n(Ha nem, akkor folytatodik a merkozes)\n");
	printf("Vagy nyomj 'X'-et a jatek azonnali feladasahoz!\n (Ha nem mentettel, akkor torlodik minden!)\n");
	char valasz;
	scanf(" %c", &valasz);
	if (valasz == 'I' || valasz == 'i')
	{
		printf("A jatek elmentese sikeres volt!\nVarunk vissza kesobb!\n");
		file = fopen("palya.txt", "w");
		fprintf(file, "%s\n", jatekos.nicknev);
		fprintf(file, "%d\n", jatekos.pontszam);
		fprintf(file, "%d\n", n);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (palya->tomb[i][j] == ' ')
					fprintf(file, "%d\n", 0);
				else if (palya->tomb[i][j] == 'O')
					fprintf(file, "%d\n", 1);
				else
					fprintf(file, "%d\n", 2);
			}
		}
	}
	else if (valasz == 'N' || valasz == 'n')
	{
		printf("Akkor folytassuk!\n");
		return;
	}
	else if (valasz == 'X' || valasz == 'x')
	{
		printf("A jatek befejezve!\n");
		felszabadit(palya->tomb);
		exit(0);
		getchar();
	}
	else
	{
		econio_textbackground(COL_RED);
		printf("Error 404, ismeretlen parancs!\n");
		printf("Biztos, hogy nyomtal I, N, vagy X-et?\n");
		econio_textbackground(COL_BLACK);
		goto hiba;
	}

	fclose(file);
	felszabadit(palya->tomb);
	exit(0);

}

void betolt(Palya *palya, Gyoztes *jatekos)
{
	/**
	Ha elmentettuk a jatekot, akkor van lehetosegunk ezzel a fuggvennyel filebol visszatolteni azt
	A fileban ebben a sorrendben vannak adatok: nev, eddig megtett lepes, palya merete, es a mezok (X, O, ures)
	Beolvassuk, majd generalunk egy uj dinamikus 2d tombot, amelybe attoltjuk a filebol az adatokat, es ezt az
	ujonnan foglalt tombre mutatunk a palya tombjevel (es mivel cimszerint vettuk at a jatekost, es a palyat
	a jatekos neve, pontja is megmarad, es a regi palya merete is)
	*/
	FILE * file;;
	file = fopen("palya.txt", "r");

	char neve[31];
	int siker = fscanf(file, "%[^\n]", neve);
	if (siker != 1)
		return NULL;

	strcpy(jatekos->nicknev, neve);
	int pontja;
	fscanf(file, "%d", &pontja);
	jatekos->pontszam = pontja;
	int size;
	fscanf(file, "%d", &size);
	palya->meret = size;

	char **newtomb;
	newtomb = (char **) malloc(1* sizeof(char*));
	newtomb[0] = (char*) malloc(1* sizeof(char));
	newtomb = (char **) realloc(newtomb, (size) *sizeof(char*));
	newtomb[0] = (char*) realloc(newtomb[0], (size) *(size) *sizeof(char));
	for (int y = 1; y < (size); ++y)
		newtomb[y] = newtomb[0] + y *(size);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			int mezo;
			fscanf(file, "%d", &mezo);
			if (!feof(file))
			{
				if (mezo == 0)
					newtomb[i][j] = ' ';
				else if (mezo == 1)
					newtomb[i][j] = 'O';
				else
					newtomb[i][j] = 'X';
			}
		}
	}

	palya->tomb = newtomb;
}

void clrscr()
{
	/**
	Torli a kepernyot
	*/
	econio_clrscr();
}

void top10(FILE *fp)
{
	/**
	Feltoltunk a file adataibol egy lancolt listat, rendezzuk, es csak az elso 10et iratjuk ki
	*/
	Ranklist * eleje;
	eleje = NULL;

	elejere(&eleje, fp);
	sorting(eleje);
	kiirlist(eleje);
	freelista(eleje);
}

bool menu()
{
	/**
	1-re jatekszabalyt kiirja, es maradunk a menuben
	2-vel ugrunk a mainben oda, ahol folytatodhat a befejezett jatek, egyedul itt terunk vissza trueval, mindenhol false
	3-mal a main elejere ugrunk, kezdodik az uj jatek
	4-kiirja a ranglistat filebol es maradunk a menuben
	5-kilep ha a felhasznalo szeretne (es persze felszabaditja a tombot), kulonben maradunk a menuben
	*/
	econio_textcolor(COL_LIGHTYELLOW);
	FILE * fp;
	int menupont=-1;

    while(menupont!=2 && menupont!=3){
        printf("\n\n\tA menupont kivalasztasahoz nyomj egy gombot!\n");
        printf("1-Jatekszabaly\n2-Elozo jatek betoltese\n3-Uj jatek\n4-Dicsoseglista\n5-Kilepes\n");
        int sikerult = scanf(" %d", &menupont);
        while (sikerult != 1)
        {
            econio_textbackground(COL_RED);
            printf("HIBA! Kerlek egy szamot adj meg! (1-5)\n");
            scanf("%*[^\n]");
            sikerult = scanf(" %d", &menupont);
        }

        econio_textbackground(COL_BLACK);

        switch (menupont)
        {
            case 1:
                clrscr();
                printf("A ket jatekos felvaltva tesz egy-egy babut a tablara. \nA jatek celja, hogy vizszintes, fuggoleges vagy atlos iranyban megszakitas nelkul ot db sajat babut sikeruljon letenni a negyzet alaku jatekteren. \nAz ellenfel ezt a kialakulni latszo vonal vegere tett babukkal probalja megakadalyozni.");
                break;
            case 2:
                clrscr();
                return true;
                break;
            case 3:
                break;

            case 4:
                clrscr();
                fp = fopen("file.txt", "r");
                top10(fp);
                fclose(fp);
                break;

            case 5:
                clrscr();
                printf("Biztosan ki szeretnel lepni? (I/N)\n");
                char valasz;
                scanf(" %c", &valasz);
                if (valasz == 'I' || valasz == 'i')
                {
                    printf("Sikeresen befejezted!\nVarunk vissza kesobb!\n");
                    getchar();
                    exit(0);
                }
                else if (valasz == 'N' || valasz == 'n')
                    printf("Rendben!\n");
                else{
                    printf("Hiba, visszakerultel a menube!\n");
                }
                break;
            default:
                econio_textbackground(COL_RED);
                printf("Hibas pont. Kerlek valassz egy uj gombot!\n");
                econio_textbackground(COL_BLACK);
        }
	}



	econio_textcolor(COL_WHITE);
	return false;
}
