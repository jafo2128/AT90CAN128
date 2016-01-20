// Centrale d'affichage et de commande
// Ce programme fonctionne sous ATMEGA16 et "devrait" fonctionner sur AT90CAN128
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define F_CPU 8000000UL  // frequence de l'horloge pour les delais

#include <util/delay.h>
#include "Lcd.h"
#include "buzzer.h"

int ETAT_VERROUILLAGE = 0; // Prend la valeur 0 si la maison est deverrouillee, la valeur 1 si elle est verrouillee, 2 si l'alarme est active

/**
* \fn void attente(char var)
* \brief Cette fonction permet d'effectuer un delay de (var * 100ms)
* \param [IN] var est le nombre de repetition
         [IN] Il est necessaire d'inclure la librairie <util/delay.h>
*/
void attente(int var)
{
    char i;
    for(i=0; i<var; i++)
    {
        _delay_ms(25);
	    _delay_ms(25);
	    _delay_ms(25);
		_delay_ms(25);
    }
}
//======================================================================

int main()
{

    /* Initialisation  */
	DDRD = 0xFF;
	PORTD = 0x00;
    // LCD
    stdout = &std_out_lcd;   // on associe le lcd avec std OUT
    LCD_init();              //initialisation du LCD

    // Clavier
    init_Clavier();
    int lock = 2; // 0 si on souhaite deverrouiller, 1 si on souhaite verrouiller, 2 sinon

    int i=0;
	char mon_tab[25]="0123456789abcdefghijklmno";
    /////////////////////////////////////////
    do{
          i = menu();
          cursor_xy(0,1);
          printf("MENU");
          cursor_xy(0,2);
          switch (i)
          {
                case 'A' : // Verrouillage
                lock = 1;
                break;

                case 'B' : // Deverrouillage
                lock = 0;
                break;

                case 'C' :
                printf("PRINCIPALE");
                break;

                case 'D' :
                printf("ANNULER");
                break;
                default:
                break;
           }
          if(lock == 0 || lock == 1)
          {
            demande_lock_unlock(lock);
            lock = 2;
          }
		  if(ETAT_VERROUILLAGE == 0) // Déverouiller
		  {
			PORTD = 0x00;
			_delay_ms(25);
		  }
		  else if(ETAT_VERROUILLAGE == 1) // Verrouiller
		  {
		  	PORTD = 0xFF;
			_delay_ms(25);
		  }
		  else if(ETAT_VERROUILLAGE == 2) // Alarme
		  {
			PORTD = ~0x01;_delay_ms(25);_delay_ms(25);
			PORTD = ~0x02;_delay_ms(25);_delay_ms(25);
			PORTD = ~0x04;_delay_ms(25);_delay_ms(25);
			PORTD = ~0x08;_delay_ms(25);_delay_ms(25);
			PORTD = ~0x10;_delay_ms(25);_delay_ms(25);
			PORTD = ~0x20;_delay_ms(25);_delay_ms(25);
			PORTD = ~0x40;_delay_ms(25);_delay_ms(25);
			PORTD = ~0x80;_delay_ms(25);_delay_ms(25);
		  }
		  _delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
		  _delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
    }while(1);
	return 0;
}

