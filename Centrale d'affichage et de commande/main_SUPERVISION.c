
// Programme echo de la liaison RS232 du AT90CAN128


#include <avr/io.h>
#include <stdio.h>
#define F_CPU 8000000UL // Fréquence de l'horloge
#include <util/delay.h>

#include "echo.h"
#include "buzzer.h"
#include "avr/interrupt.h"


static FILE out_rs232 = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);

typedef enum { false, true } bool;


bool FLAG_RECEPTION = 0;            // '1' si une donnee est recue sur la liaison RS232, '0' sinon
char RECEIVEDBYTE;                  // Donee reçu sur la liaison RS232
unsigned char CMD[2] = "  ";        // Tableau utilise pour les requetes/reponses entre le uC et le Serveur
bool ETAT_DETECTEUR_FUMEE = 0;      // '1' Si presence de fumee, '0' sinon
bool ETAT_AUTONOMIE_FUMEE = 0;      // '1' Si autonomie suffisante, '0' sinon
bool ETAT_ALARME_FUMEE = 0;         // '1' Si alarme activee, '0' sinon
bool ETAT_DETECTEUR_PRESENCE = 0;   // '1' Si un individu est detecte, '0' sinon
bool ETAT_AUTONOMIE_PRESENCE = 0;   // '1' Si autonomie suffisante, '0' sinon
bool ETAT_ALARME_PRESENCE = 0;      // '1' Si alarme activee, '0' sinon
bool ETAT_VERROUILLAGE = 0;         // '1' Si le clavier est verrouille, '0' sinon


/**
* \fn ISR(USART1_RX_vect)
* \brief Interruption permettant de savoir si une donnee est presente sur la liaison RS232 et de copier cette donnee dans RECEIVEDBYTE
*/
ISR(USART1_RX_vect)
{
	FLAG_RECEPTION = 1;
	RECEIVEDBYTE = UDR1;
  	//UDR1 = RECEIVEDBYTE; //Decommenter seulement si on souhaite visualise ce qu'on ecrit sur l'hyperterminal
}


int main()
{
	CLKPR = 0x00;   // Permet d'avoir une frequence de 8MHz
	uart_init(25);  // A 8MHz, BAUD = 51 permet d'avoir une vitesse de transmission de 9600

	DDRA = 0xFF; //Port A en sortie pour les LED
	bool reception = 0;

	fprintf(&out_rs232,"UART operationnelle \n"); // Printf du flux RS232
	stdout = &out_rs232;
	printf("UART OPERATIONNELLE");
	sei(); //Interruption active



    while(1)
	{
		reception = FLAG_RECEPTION;
		_delay_ms(25);
		_delay_ms(25);

		if(reception==1)
		{
			analyse_Requete();	// Le serveur demande des informations au uC
		}
        // Défilement des LED pour signifier que l'on n'est pas dans l'analuse_Requete
		PORTA = 0x01;_delay_ms(25);
		PORTA = 0x02;_delay_ms(25);
		PORTA = 0x04;_delay_ms(25);
		PORTA = 0x08;_delay_ms(25);
		PORTA = 0x10;_delay_ms(25);
		PORTA = 0x20;_delay_ms(25);
		PORTA = 0x40;_delay_ms(25);
		PORTA = 0x80;_delay_ms(25);

	};
    return 0;

}



