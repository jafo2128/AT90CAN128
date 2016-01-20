#include "echo.h"
#include <avr/io.h>

// Voir main.c pour la definition des variables globales ci-dessous
typedef enum { false, true } bool;
extern char RECEIVEDBYTE;
extern unsigned char CMD[2];
extern bool FLAG_RECEPTION;
extern bool ETAT_DETECTEUR_FUMEE;
extern bool ETAT_AUTONOMIE_FUMEE;
extern bool ETAT_ALARME_FUMEE;
extern bool ETAT_DETECTEUR_PRESENCE;
extern bool ETAT_AUTONOMIE_PRESENCE;
extern bool ETAT_ALARME_PRESENCE;
extern bool ETAT_VERROUILLAGE;



/**
* \fn void uart_init(int ubrr)
* \brief Permet d'initialiser la liaison série RS232
* \param [INFO] Parite : paire
         [INFO] Donnee : 8bits
         [INFO] Bit de stop : 1bit
         [INFO] Controle de flux : Aucun
         [INFO] Vitesse de transmission : 9600 pour ubrr = 51 et F=8MHz
*/
void uart_init (int ubrr)
{
    DDRD = (1 << PD3);       // On met PD3 en sortie (TXD) et PD2 sortie RxD

    UCSR1B = (1<<TXEN1)|(1<<RXEN1); // Valider la transmission et la réception
	UCSR1B |= (1 << RXCIE1); // Active interruption (USART_RXC)

    UCSR1C = (1<<UCSZ11)|(1<<UCSZ10)|(1<<UPM1); // Parite pair, 8 bits de donnée
    UBRR1H = (unsigned char) (ubrr>>8);
    UBRR1L = (unsigned char) ubrr;
}


/**
* \fn void uart_putchar(char data, FILE *stream
* \brief Cette fonction a ete modifie pour pouvoir utiliser la fonction PRINTF pour afficher des donnees sur l'hyperterminal lors des phases de test
* \param [IN] data, donnee que l'on veut emettre
         [IN] *stream, flux de sortie rs232
*/
void uart_putchar(char data, FILE *stream) // Pour afficher avec printf sur Hyperterminal
{
    do
    {
        // Attente de la fin de l'émission
    }
    while( !(UCSR1A & (1<<UDRE1)) );
    UDR1 = data; //Envoie de la donnée

}

/**
* \fn void uart_emettre(char data)
* \brief Permet d'emettre un char via la liaison RS232 vers le PC/Serveur
* \param [IN] data, donnee que l'on veut emettre
*/
void uart_emettre(char data)
{
    do
    {
        // Attente de la fin de l'émission
    }
    while( !(UCSR1A & (1<<UDRE1)) );
    UDR1 = data; //Envoie de la donnée
    //return 0;

}

/**
* \fn void analyse_Requete()
* \brief Analyse la requete envoyer par le serveur et determine la reponse que le microcontroleur doit envoyer.
* \param [INFO] Plus d'information concernant les codes requetes/reponses dans le rapport de projet
*/
void analyse_Requete()
{
	CMD[0] = CMD[1];
	CMD[1] = RECEIVEDBYTE;
	// PARTIE FUMEE
			if(CMD[0] == '2' && CMD[1] == '0') // ETAT DETECTEUR FUMEE
			{
				if(ETAT_DETECTEUR_FUMEE == 1)
				{
					CMD[0] = '1';
					CMD[1] = '0';
					envoie_Reponse();
					clear_CMD(CMD);
				}
				else
				{
					CMD[0] = '1';
					CMD[1] = '1';
					envoie_Reponse();
					clear_CMD(CMD);
				}
			}
			else if(CMD[0] == '2' && CMD[1] == '1') // ETAT AUTONOMIE FUMEE
			{
				if(ETAT_AUTONOMIE_FUMEE == 1)
				{
					CMD[0] = '1';
					CMD[1] = '2';
					envoie_Reponse();
					clear_CMD(CMD);
				}
				else
				{
					CMD[0] = '1';
					CMD[1] = '3';
					envoie_Reponse();
					clear_CMD(CMD);
				}

			}
			else if(CMD[0] == '2' && CMD[1] == '2') // ETAT ALARME FUMEE
			{
				if(ETAT_ALARME_FUMEE == 1)
				{
					CMD[0] = '1';
					CMD[1] = '4';
					envoie_Reponse();
					clear_CMD(CMD);
				}
				else
				{
					CMD[0] = '1';
					CMD[1] = '5';
					envoie_Reponse();
					clear_CMD(CMD);
				}
			}
	// PARTIE PRESENCE
			else if(CMD[0] == '4' && CMD[1] == '0') // ETAT DETECTEUR PERSONNE
			{
				if(ETAT_DETECTEUR_PRESENCE == 1)
				{
					CMD[0] = '3';
					CMD[1] = '0';
					envoie_Reponse();
					clear_CMD(CMD);
				}
				else
				{
					CMD[0] = '3';
					CMD[1] = '1';
					envoie_Reponse();
					clear_CMD(CMD);
				}

			}

			else if(CMD[0] == '4' && CMD[1] == '1') // ETAT AUTONOMIE PRESENCE
			{
				if(ETAT_AUTONOMIE_PRESENCE == 1)
				{
					CMD[0] = '3';
					CMD[1] = '2';
					envoie_Reponse();
					clear_CMD(CMD);
				}
				else
				{
					CMD[0] = '3';
					CMD[1] = '3';
					envoie_Reponse();
					clear_CMD(CMD);
				}

			}

			else if(CMD[0] == '4' && CMD[1] == '2') // ETAT ALARME PRESENCE
			{
				if(ETAT_ALARME_PRESENCE == 1)
				{
					CMD[0] = '3';
					CMD[1] = '4';
					envoie_Reponse();
					clear_CMD(CMD);
				}
				else
				{
					CMD[0] = '3';
					CMD[1] = '5';
					envoie_Reponse();
					clear_CMD(CMD);
				}

			}

			else if(CMD[0] == '4' && CMD[1] == '3') // ETAT VERROUILLAGE
			{
				if(ETAT_VERROUILLAGE == 1)
				{
					CMD[0] = '3';
					CMD[1] = '6';
					envoie_Reponse();
					clear_CMD(CMD);
				}
				else
				{
					CMD[0] = '3';
					CMD[1] = '7';
					envoie_Reponse();
					clear_CMD(CMD);
				}

			}
	// Si le code de la requete n'existe pas, on ne fait rien, on pourra plus tard au besoin fournir une reponse d'erreur plus precise
			else if(CMD[0] != ' ' && CMD[1] != ' ')
			{
				printf("REQUETE INCONNUE");clear_CMD(CMD);
			}
			FLAG_RECEPTION = 0;

}

/**
* \fn void clear_CMD()
* \brief Permet de reinitialiser le tableau CMD pour les requetes/reponses
*/
void clear_CMD()
{
	CMD[0] = ' ';
	CMD[1] = ' ';
}

/**
* \fn envoie_Reponse()
* \brief Envoie deux donnees au serveur, il s'agit de la reponse du microcontroleur
*/
void envoie_Reponse()
{
	uart_emettre(CMD[0]);
	_delay_ms(25);
	uart_emettre(CMD[1]);
}
