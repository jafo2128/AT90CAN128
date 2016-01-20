#include <stdio.h>


#define F_CPU 8000000UL // Frequence de l'horloge
#include <util/delay.h>

// La description de ces fonctions est faites dans echo.c
void uart_init(int ubrr);
void uart_putchar(char data, FILE *stream); // Pour visualiser les infos avec le printf
void uart_emettre(char data); // Pour emettre des caracteres sans l'afficher


void analyse_Requete();
void clear_CMD();
void envoie_Reponse();
