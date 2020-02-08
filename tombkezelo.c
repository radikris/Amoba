#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "econio.h"
#include "tombkezelo.h"
#include "structures.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"

void felszabadit(char **tomb){
    /**Kulon kell szabaditani a sort es az oszlopot, hisz dinamikus 2D array
    */
    free(tomb[0]);
    free(tomb);
}

void kiir(Palya palya){
    /**
    A palya kiirasaert felelos fgv, -, |, space, + jelek segitsegevel iratja ki a palyat
    Amely igy negyzetracsoskent fog megjelenni, es a negyzet kozepen van a rakott jel, X, O, vagy ures
    A negyzet tetejen es oldalan szamozas talalhato, hogy egyszerubb legyen a mezok koordinatait beazonisitani
    */
    int n=palya.meret;
    econio_textcolor(COL_WHITE);
    printf("\n");
    printf(" ");
    for(int i=0; i<n; i++){
        printf("%2d %c", i, 186);
    }
    printf(" %c ", 197);
    printf("\n");
    printf(" ");
    /*for(int l=0; l<4*n+4; l++){
        printf("%c", 205);  //-
    }*/
    for(int l=0; l<4*n+4; l++){
            if(((l+1)%4==0 || l==3) && (l!=0 && l!=4*n+3))
                printf("%c", 186);
            else
                printf("%c", 205);
    }
    printf("\n");
    for(int i=0; i<n; i++){
        printf(" ");
        for(int l=0; l<n; l++){
            if(palya.tomb[i][l]=='X'){
                printf(" ");
                econio_textcolor(COL_LIGHTGREEN);
                printf("%c", palya.tomb[i][l]);
                econio_textcolor(COL_WHITE);
                printf(" %c", 186);             //|
            }else if(palya.tomb[i][l]=='O'){
                printf(" ");
                econio_textcolor(COL_LIGHTBLUE);
                printf("%c", palya.tomb[i][l]);
                econio_textcolor(COL_WHITE);
                printf(" %c", 186);
            }else
                printf(" %c %c", palya.tomb[i][l], 186);
            econio_textcolor(COL_WHITE);
        }
        printf(" %d", i);
        printf("\n");
        printf(" ");
        for(int l=0; l<4*n+4; l++){
            if(((l+1)%4==0 || l==3) && (l!=0 && l!=4*n+3))
                printf("%c", 186);
            else
                printf("%c", 205);
        }
        printf("\n");
    }
}

void nullaz(Palya * palya){
    /**
    Ha uj jatekot kezdunk, akkor hivjuk meg ezt a fuggvenyt, es a tablank minden mezojet 'lenullazzuk'
    vagyis ures mezo lesz mindenhol
    */
    int n=palya->meret;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            palya->tomb[i][j]=' ';
        }
    }
}
void atir(Palya* palya){
    /**
    Novelni akarjuk a palyank meretet +2 sorral es +2 oszloppal a szelen
    Ehhez foglalunk egy uj 2d tombot az uj meretekkel (negyzetes), es atmasoljuk elcsusztatva a regi elemeket
    Majd felszabaditjuk a regi tombot, es visszaterunk az ujjal
    */
    int regimeret=(palya->meret);
    (palya->meret)+=4;
    char **newtomb;
    newtomb=(char**)malloc(1*sizeof(char*));
    newtomb[0]=(char*)malloc(1*sizeof(char));
    newtomb=(char**)realloc(newtomb, (palya->meret)*sizeof(char*));
    newtomb[0]=(char*)realloc(newtomb[0], (palya->meret)*(palya->meret)*sizeof(char));
    for (int y = 1; y < (palya->meret); ++y)
       newtomb[y] = newtomb[0] + y*(palya->meret);

    for(int l=0; l<(palya->meret); l++){
        for(int s=0; s<(palya->meret); s++){
            newtomb[l][s]=' ';
        }
    }

    for(int i=0; i<regimeret; i++){
        for(int j=0; j<regimeret; j++){
            newtomb[i+2][j+2]=palya->tomb[i][j];
        }
    }
    felszabadit(palya->tomb);
    palya->tomb = newtomb;
}

bool szelso_ket_sor_oszlop(int kulsofor, int kulsofeltet, int belsofor, int belsofeltet, char **tomb){
    /**
    Ellenorizzuk, hogy a megadott 2 sor, vagy 2 oszlopban van-e jel,
    es ha igen, return true, ha nincsen akkor return false (ez alapjan fogjuk novelni a palya meretet)
    */
    for(int i=kulsofor; i<kulsofeltet; i++){
        for(int j=belsofor; j<belsofeltet; j++){
            if(tomb[i][j]!=' '){
                return true;
            }
        }
    }
    return false;
}

bool peremterulet(Palya palya){
    /**
    Ellenorizzuk, hogy a felso vagy also 2 sorban, illetve bal vagy jobb 2 oszlopban van-e jel,
    es ha igen, return true, ha nincsen akkor return false (ez alapjan fogjuk novelni a palya meretet)
    */
    bool perem=false;
    int szamlalo=0;
    while(!perem && szamlalo<4){
        switch (szamlalo)
		{
			case 0:
				perem = szelso_ket_sor_oszlop(0, 2, 0, palya.meret, palya.tomb);
				break;
			case 1:
				perem = szelso_ket_sor_oszlop(0, palya.meret, 0, 2, palya.tomb);
				break;
			case 2:
				perem = szelso_ket_sor_oszlop(palya.meret - 2, palya.meret, 0, palya.meret, palya.tomb);
				break;
			case 3:
				perem = szelso_ket_sor_oszlop(0, palya.meret, 0, 2, palya.tomb);
				break;
		}
        szamlalo++;

    }
    return perem;
}
