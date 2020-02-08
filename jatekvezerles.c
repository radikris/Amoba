#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "econio.h"
#include "jatekvezerles.h"
#include "structures.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"

bool szabade(Palya *palya, int x, int y)
{
	if (x >= palya->meret || x < 0 || y >= palya->meret || y < 0 || palya->tomb[x][y] != ' ')
	{
		econio_textbackground(COL_RED);
		printf("Hibas lepes, kerlek probald ujra!\n");
		econio_textbackground(COL_BLACK);
		return false;
	}

	return true;
}

bool irany5csekk(char ** tomb, int i, int j, Irany irany, int n){
    printf("Ez a kiindulopont: gyokeri es gyokerj %d es %d\n", i, j);
    switch(irany){
        case 0: //vizjobb
            if(j-1>=0 && j+5<n && (tomb[i][j-1]=='X' || tomb[i][j+5]=='X')){
                printf("vizszintakadalyX\n");
                return true;
            }
        break;

        case 1: //fuggle
            if(i-1>=0 && i+5<n && (tomb[i-1][j]=='X' || tomb[i+5][j]=='X')){
                printf("fuggleakadalyX\n");
                return true;
            }
        break;

        case 2: //atlojobble
            if(((i-1>=0 && j-1>=0) && (tomb[i-1][j-1]=='X')) || ((i+5<n && j+5<n) && tomb[i+5][j+5]=='X')){
                printf("atlojobbleakadalyx\n");
                return true;
            }
        break;

        case 3: //atloballe
            if(((i-1>=0 && j+1<n) && tomb[i-1][j+1]=='X') || ((i+5<n && j-5>=0) && tomb[i+5][j-5]=='X')){
                printf("atloballeakadalyx\n");
                return true;
            }
        break;

    }
    return false;
}

bool ellenoriz(int i, int j, int sori, int oszlopi, char **tomb, Mezo *vissza, int *szamol, bool *bennevan, int limit, Irany irany, int palyameret, int gyokeri, int gyokerj)
{
	/**
	Majd lejjebb a vedekezes fofuggvenyben hivjuk meg
	Ahol a fofgv atmenetileg rak egy felhasznaloi jelet az osszes ures mezore (nem egyszerre, mindig masikra a ciklusban),
	es igy nezi meg, hogyha ott lenne egy olyan jel (tehat ha a kovetkezo korben odalepnenk)
	(future predictionshoz hasonlit)
	Hany darab felhasznaloi jel lenne EGYMAS MELLETT a mezonek a 'GYOZ=5' sugaru kornyezeteben
	De mivel a gep prioritasa eloszor a tamadas, a fofgv csak akkor vesz figyelembe egy mezot, ha a mezonel
	valamelyik iranyban mar legalabb 2 azonos gep jel van egymas mellett, (ez a 2 mar az atmeneti jelet is tartalmazza, vedekezesnel ez legalább 3)
	ha nincs, akkor a Mezo struktura helyzet=falset kap es igy ezt a fgvt nem fogjuk meghivni ebben a korben
	Ha van, akkor truet kap, es visszaadjuk a maximumhoz tartozo mezo koordinatait
	*/
	if (tomb[sori][oszlopi] == tomb[i][j])
	{
		if (sori == i && oszlopi == j)
		{
			*bennevan = true;
		}

		(*szamol) ++;
	}
	else
	{
		return false;
	}

	if ((*szamol) > GYOZ - limit && (*szamol) > (*vissza).mennyi && *bennevan)
	{
	    if(*bennevan && tomb[sori][oszlopi]=='O' && irany5csekk(tomb, gyokeri, gyokerj, irany, palyameret) && (*szamol>=3 && *szamol!=GYOZ)){
            //kell meg parameter, meliyk iranyban kell nezni, az i-1 i+1 vagy j-1 j+1-et
            //1 az jobbra (j+), 2 az lefele (i-), 3 atlo jobbra le (i+, j+), 4, atlo balra (i-, j+)
            (*vissza).helyzet = false;
            (*vissza).sor = -1;
            (*vissza).oszlop = -1;
            (*vissza).mennyi = 0;
            printf("nemrakunk ide: %d es %d!\n", sori, oszlopi );
            return false;
		}

		(*vissza).helyzet = true;
		(*vissza).sor = sori;
		(*vissza).oszlop = oszlopi;
		(*vissza).mennyi = (*szamol);
	}

	return true;
}

Mezo ujtaktika(Palya *palya, int limit, char babu)
{
	/**
	Atmenetileg rak egy sajat (gep) vagy felhasznaloi jelet (ez a paramaterkent atvett babutol fugg) az osszes ures mezore (nem egyszerre, mindig masikra a ciklusban),
	es igy nezi meg, hogyha ott lenne egy olyan jel (tehat ha a kovetkezo korben odalepnenk)
	(future predictionshoz hasonlit)
	akkor hany darab sajat jel (gep) vagy felhasznaloi jel lenne EGYMAS MELLETT a mezonek a 'GYOZ=5' sugaru kornyezeteben
	De mivel a gep prioritasa eloszor a tamadas, mar akkor is figyelembe vesz egy mezot, ha a mezonek
	valamelyik iranyban mar legalabb 2 azonos gep jel van egymas mellett, ellenben a védekezésnél, ahol
	legalább 3 azonos felhasznaloi jelnek kell egymás mellett lennie (ez a 2, illetve 3 mar az atmeneti jelet is tartalmazza, ez a limit)
	ha nincs, akkor a Mezo struktura helyzet=falset kap es igy ezt a fgvt nem fogjuk meghivni ebben a korben
	Ha van, akkor truet kap, es visszaadjuk a maximumhoz tartozo mezo koordinatait
	*/

	int szamol = 0;
	Mezo vissza;
	vissza.helyzet = false;
	vissza.mennyi = 0;
	int n = palya->meret;
	for (int x = 0; x < n; x++)
	{
		for (int y = 0; y < n; y++)
		{
			int sori = x;
			int oszlopi = y;
			if (palya->tomb[sori][oszlopi] != ' ')
			{
				continue;
			}

			palya->tomb[sori][oszlopi] = babu;
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					bool bennevan = false;
					bool csekk = true;
					szamol = 0;
					for (int k = 0; csekk == true && k < GYOZ && j + k < n; k++)	//vizszintesen jobbra
						csekk = ellenoriz(i, j + k, sori, oszlopi, palya->tomb, &vissza, &szamol, &bennevan, limit, vizjobb, n, i, j);

					szamol = 0;
					bennevan = false;
					csekk = true;
					for (int k = 0; csekk == true && k < GYOZ && i + k < n; k++)	//fuggolegesen le
						csekk = ellenoriz(i + k, j, sori, oszlopi, palya->tomb, &vissza, &szamol, &bennevan, limit, fuggle, n, i, j);

					szamol = 0;
					bennevan = false;
					csekk = true;
					for (int k = 0; csekk == true && k < GYOZ && i + k < n && j + k < n; k++)	//atlosan jobbrale
						csekk = ellenoriz(i + k, j + k, sori, oszlopi, palya->tomb, &vissza, &szamol, &bennevan, limit, atlojobble, n, i ,j);

					szamol = 0;
					bennevan = false;
					csekk = true;
					for (int k = 0; csekk == true && k < GYOZ && i + k < n && j - k >= 0; k++)	//atlosan balrale
						csekk = ellenoriz(i + k, j - k, sori, oszlopi, palya->tomb, &vissza, &szamol, &bennevan, limit, atloballe, n, i, j);

				}
			}

			palya->tomb[sori][oszlopi] = ' ';
		}
	}

	return vissza;
}

void veszelyes(Palya *palya)
{
	/**
	Itt tortenik a gepnek a lepese
	2 fuggveny a tamadasra, 2 fuggveny a vedekezesre
	Ezeknek a kifejteset lasd feljebb!
	*/

	Mezo ujtax = ujtaktika(palya, 3, 'O');
	Mezo ujattack = ujtaktika(palya, 4, 'X');

    printf("ket tax meghivva!\n");

	if (ujattack.mennyi >= ujtax.mennyi)
	{
		if (ujattack.helyzet)
		{
		    printf("Koordin: %d es %d\n", ujattack.sor, ujattack.oszlop);
			palya->tomb[ujattack.sor][ujattack.oszlop] = 'X';
			return;
		}
	}
	else
	{
		if (ujtax.helyzet)
		{
		    printf("Koordin: %d es %d\n", ujattack.sor, ujattack.oszlop);
			palya->tomb[ujtax.sor][ujtax.oszlop] = 'X';
			return;
		}
	}
}



