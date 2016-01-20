#include <avr/io.h>
#define F_CPU 8000000UL // Fréquence de l'horloge
#include <util/delay.h>
#include "buzzer.h"

#define DO 109
#define RE 6
#define MI 86
#define FA 82
#define SOL 72
#define LA 64
#define SI 57

/**
* \fn void start_timer0(unsigned short int ocr0a)
* \brief Demarre le timer 0 et configure la fréquence du buzzer
* \param [IN] ocr0a est la valeur calcule pour obtenir la frequence souhaiter
*/
void start_timer0(unsigned short int ocr0a)
{
    DDRB = DDRB|(1<<PB7); // On configure OCO en sortie, PB7
    TCNT0 = 0;            // Remise a zero du registre de comptage du timer 0
    OCR0A = ocr0a;
    TCCR0A = (1<<WGM01)|(1<<COM0A0)|(1<<CS01)|(1<<CS00);// Parametrage du mode CTC, du prédiviseur 64 et sorti en mode Toggle
}

/**
*\fn stop_timer0()
*\brief Arrete le timer 0
*/
void stop_timer0(void)
{
    PORTB = PORTB & 0b01111111;        //force l'emisssion sur PB7
    TCCR0A = 0;                       // Arret du comptage
}

/**
* \fn void start_timer1(unsigned short int ocr1a)
* \brief Demarre le timer 1 et configure le delai
* \param ocr1a Valeur du delai (en nb de periodes de 57600Hz)
*/
void start_timer1(unsigned short int ocr1a)
{
    OCR1A=ocr1a;
    TCCR1B=(1<<WGM12)|(1<<CS11)|(1<<CS10); // Prédiviseur 64 et mode CTC
    TCNT1 = 0;                             // Remise à zéro du registre de comptage du timer 1
}

/**
*\fn stop_timer1()
*\brief Arrete le timer 1
*/
void stop_timer1(void)
{
    TCCR1B = 0;          // Arret du timer 1
}


/**
* \fn void wait_OCR1A_timer1(void)
* \brief Attend le delai prévu pour  le timer 1 (ocr1a)
*/
void wait_OCR1A_timer1(void)
{
    char flag1;
    do
    {
        flag1 = TIFR1 & (1<<OCF1A);
    }
    while(flag1==0);  // Arret du compteur quand TCNT1 = OCR1A
    TIFR1 = TIFR1|(1<<OCF1A);// Remise à 0 d'OCF1A
}

/**
* \fn void note(char temps, int note)
* \brief Permet de jouer une note pendant un certain temps
* \param [IN] temps est la duree pendant laquelle on souhaite jouer la note
            - temps ne doit pas depasser la valeur 11 ( 2^8 / 5760 )
         [IN] note est la fréquence d'OCR0A que l'on souhaite jouer
*/
void note(char temps, int note)
{
    start_timer0(note);
    start_timer1(temps * 5760); // 100 ms
    wait_OCR1A_timer1();
    stop_timer1();
    stop_timer0();
}


/**
* \fn void init_Buzzer(void)
* \brief Initialisation du Buzzer
*/
void init_Buzzer(void)
{
    PORTB = PORTB | 0b10000000; // PB7 doit être en sortie pour activer le piézo électrique du buzzer
}

/**
* \fn void activer_Buzzer(char mode)
* \brief Activer le buzzer en mode Alarme ou Incendie
* \param [IN] mode est le mode de fonctionnement de l'alarme

   - 0 : Desactiver le buzzer
   - 1 : Activer le buzzer en mode Incendie
   - 2 : Activer le buzzer en mode Intrusion
   - 3 : Activer le buzzer en mode erreur code

*/
void activer_Buzzer(char mode)
{
    if(mode==0)
    {
        // On ne fait rien, le buzzer n'est pas active
    }
    if(mode==1) // Incendie
    {
        note(10,LA);	
        note(10,LA);
        note(10,SI);
        note(10,SI);
    }
    if(mode==2) //Intrusion
    {
        note(10,RE);   
        note(10,LA);
    }
    if(mode==3) // Code faux
    {
        note(10,FA);    
        note(10,LA);		
    }
}
