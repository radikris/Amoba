#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "keret.h"
#include "econio.h"
#include "structures.h"
#include "jatekvezerles.h"
#include "sablon.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"

bool vege(char **tomb, int *szamol, Jatek *vissza, int feltetx, int feltety, int i, int j)
{
	/**
	A fgv parameterkent kapja a felteteleket, es a mezot, amelyik az iranyt hatarozza meg
	es megszamolja hany db azonos jel van egymas mellett a mezotol a helyes iranyban
	Ha van olyan mezo amelyik elter az ellenorizni kivanttol, akkor falset ad vissza a fgv
	*/
	if (tomb[i][j] == tomb[feltetx][feltety] && tomb[i][j] != ' ')
	{
		(*szamol) ++;
	}
	else
		return false;
	if ((*szamol) == GYOZ)
	{
		(*vissza).vege = true;
		(*vissza).karakter = tomb[i][j];
	}

	return true;
}

Jatek jatekvege(Palya palya)
{
	/**
	Atadja a fgvnek a 4 iranyt, fuggoleges, vizszintes, es a 2 atlo, majd megnezi, hogy
	abban az iranyban van-e egymas mellett 5 azonos jel, es ha igen akkor milyen, ezzel ter vissza a fgv
	Ha nincs, akkor a Jatek struktura eleme false es '!' karaktert kap, vagyis ez nem lehetseges
	*/
	int szamol = 0;
	bool nemures;
	Jatek vissza;
	vissza.vege = false;
	vissza.karakter = '!';
	int n = palya.meret;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			szamol = 0;
			nemures = true;
			for (int k = 0; nemures == true && k < GYOZ && j + k < n; k++)	//vizszintesen jobbra
				nemures = vege(palya.tomb, &szamol, &vissza, i, j + k, i, j);

			szamol = 0;
			nemures = true;
			for (int k = 0; nemures == true && k < GYOZ && i + k < n; k++)	//fuggolegesen le
				nemures = vege(palya.tomb, &szamol, &vissza, i + k, j, i, j);

			szamol = 0;
			nemures = true;
			for (int k = 0; nemures == true && k < GYOZ && i + k < n && j + k < n; k++)	//atlosan jobbrale
				nemures = vege(palya.tomb, &szamol, &vissza, i + k, j + k, i, j);

			szamol = 0;
			nemures = true;
			for (int k = 0; nemures == true && k < GYOZ && i - k >= 0 && j + k < n; k++)	//atlosan balrale
				nemures = vege(palya.tomb, &szamol, &vissza, i - k, j + k, i, j);

		}
	}

	return vissza;
}

void lepj(Palya *palya, Gyoztes jatekos)
{
	/**
	Nem az elso lepes, van lehetoseg menteni a jatekot
	A bekert koordinatak, x es y (sor es oszlopnak felelnek meg a tombben), majd odarakjuk a jelet
	*/
	int x, y;
	bool elso = true;
	int sikerult;
	int n = palya->meret;
	if (n > 11)
	{
		elso = false;
	}

	for (int i = 0; i < n && elso == true; i++)
	{
		for (int j = 0; j < n && elso == true; j++)
		{
			if (palya->tomb[i][j] != ' ')
			{
				elso = false;
			}
		}
	}

	if (elso)
	{
		printf("Az elso lepesnel meg nem lehet menteni!\n");
		printf("A Te jeled a 'O', az enyem pedig az 'X'\n");
		printf("Hova szeretnel rakni, kedves %s? (sor-oszlop)\n", jatekos.nicknev);
	}
	else
	{
		mentes(palya, jatekos);
		printf("Hova szeretnel rakni, kedves %s? (sor-oszlop)\n", jatekos.nicknev);
	}

	while (1)
	{
		sikerult = scanf("%d %d", &x, &y);
		if (sikerult != 2)
		{
			econio_textbackground(COL_RED);
			printf("Amit megadtal, nem szam volt!\n");
			scanf("%*[^\n]");
		}
		else if (x > n || x < 0 || y > n || y < 0)
		{
			printf("A megadott szam nem volt megfelelo! (min: %d, max: %d)\n", 0, n);
		}
		else if (!szabade(palya, x, y))
		{;
		}
		else
		{
			econio_textbackground(COL_BLACK);
			break;
		}

		econio_textbackground(COL_BLACK);
	}

	palya->tomb[x][y] = 'O';
	return;
}

void elso(Palya *palya)
{
	/**
	A felhasznalo utani elso lepese a gepnek
	Megnezzuk melyik mezore lepett a jatekos, es a kornyezo mezok kozul valasztunk egyet random
	*/
	srand(time(0));
	int lutri;
	Mezo elsot;
	elsot.sor=-1;
	elsot.oszlop=-1;
	int n = palya->meret;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (palya->tomb[i][j] != ' ')
			{
				elsot.sor = i;
				elsot.oszlop = j;
			}
		}
	}

	lutri = rand() % 4 + 1;
	switch (lutri)
	{
		case 1:
			palya->tomb[elsot.sor + 1][elsot.oszlop] = 'X';
			break;
		case 2:
			palya->tomb[elsot.sor][elsot.oszlop + 1] = 'X';
			break;
		case 3:
			palya->tomb[elsot.sor - 1][elsot.oszlop] = 'X';
			break;
		case 4:
			palya->tomb[elsot.sor][elsot.oszlop - 1] = 'X';
			break;
	}
}
