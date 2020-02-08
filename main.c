#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "ranklist.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"
#include "econio.h"
#include "tombkezelo.h"
#include "jatekvezerles.h"
#include "keret.h"
#include "sablon.h"
#include "structures.h"


//A GEP jele az X, a felhasznaloe az O

int main(int argc, char *argv[])
{
    Gyoztes parbaj;         //Ebben fogjuk a jatekos nevet es pontjat tarolni

    Jatek jatekellenor;
    parbaj.pontszam=0;
                            //Kialakitjuk az alap palyat

    Palya palya;
    palya.meret=11;

                            //A menubol elindul a jatek, ha be kell tolteni az elozo jatekot akkor azt az ifben folytatjuk, ha
                            //uj jatekot kezdunk, akkor az elsebe megyunk egybol
    bool betoltes=menu();
    if(betoltes){
        printf("Folytassuk akkor a jatekot onnan, ahonnan abbahagytuk!\nA folytatashoz, nyomj egy tetszoleges billentyut!\n");
        getchar();
        getchar();
        loading();
        betolt(&palya, &parbaj);
        if(palya.tomb==NULL){
            betoltes=false;
        }else{
            kiir(palya);
        }
        jatekellenor=jatekvege(palya);
    }
    if(!betoltes){                  //Uj jatek, felhasznalo adatai megadasa
        clrscr();
        palya.meret=11;
        palya.tomb = (char**) malloc(palya.meret * sizeof(char*));
        palya.tomb[0] = (char*) malloc(palya.meret * palya.meret * sizeof(char));
        for (int y = 1; y < palya.meret; ++y)
            palya.tomb[y] = palya.tomb[0] + y*(palya.meret);
        nullaz(&palya);
        jatekellenor=jatekvege(palya);

        econio_textcolor(COL_LIGHTGREEN);
        printf("Udvozollek az amoba jatekomban!\n");
        printf("Kerlek add meg a nickneved!\n");
        scanf(" %s", parbaj.nicknev);
        clrscr();
        loading();
        clrscr();
        kiir(palya);          //Kiirjuk a palyat, rak a jatekos, rak a gep, noveljuk a pontokat, es noveljuk a palyat ha szukseges
        getchar();
        lepj(&palya, parbaj);
        parbaj.pontszam++;
        if(peremterulet(palya))
            atir(&palya);
        elso(&palya);
        parbaj.pontszam++;
    }
    //Ha folytatjuk az elozot, akkor egybol ideugrunk
    while(!jatekellenor.vege){          //Kepernyo torlese, kiirasa, lepesek, pontok novelesek, palyanoveles, jatekvege ellenorzes
        //clrscr();
        kiir(palya);

        lepj(&palya, parbaj);
        parbaj.pontszam++;

        if(peremterulet(palya))
            atir(&palya);
        econio_textbackground(COL_LIGHTRED);
        printf("Hmm, ugyes lepes, gondolkodom!");
        for(int i=0; i<=3; i++){
            delay(0.5);
            printf(".");
        }
        econio_textbackground(COL_BLACK);
        veszelyes(&palya);
        parbaj.pontszam++;
        if(peremterulet(palya)){
            atir(&palya);
        }

        jatekellenor=jatekvege(palya);
    }                               //Ha kilepunk a ciklusbol akkor vege a jateknak, es ezt kiirjuk a kepernyore, hogy kinyert
    clrscr();
    kiir(palya);
    econio_textcolor(COL_YELLOW);
    if(jatekellenor.vege==true && jatekellenor.karakter=='O'){
        printf("Gratulalok %s, nyertel, a pontszamod: %d!\n", parbaj.nicknev, parbaj.pontszam);
    }
    else if(jatekellenor.vege==true && jatekellenor.karakter=='X'){
        printf("Ezt most en nyertem, sajnalom! A pontom: %d\n", parbaj.pontszam);
        strcpy(parbaj.nicknev, "RobotAI");
    }
    delay(1.5);             //A jatek vegen automatikusan kiiratjuk a ranglistat

    FILE *fp;

    fp = fopen("file.txt", "a");
    if(fp==NULL){
        fprintf(stderr, "Nem lehet megnyitni a fajlt\n");
        exit(0);
    }

    fprintf(fp, "\n%s %d", parbaj.nicknev, parbaj.pontszam);       //A gyoztes neve es pontszama
    fclose(fp);

    fp=fopen("file.txt", "r");
    if(fp==NULL){
        fprintf(stderr, "Nem lehet megnyitni a fajlt\n");
        exit(0);
    }

    top10(fp);                  //Vegul bezarjuk a filet, felszabaditjuk a tombunket, es befejezzuk a progrmat

    fclose(fp);
    felszabadit(palya.tomb);
    getchar();
    econio_textcolor(COL_WHITE);

    return 0;
}
