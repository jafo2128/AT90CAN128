/** \file lcd.c
   \brief les fonctions relatives au driver de LCD  */

#include <avr/io.h>
#include <util/delay.h>
#include "Lcd.h"

#define E      0   // LIgne E du bus (enable) sur portC.0
#define RW     1   // LIgne RW du bus (Read/Write) sur portC.1
#define RS     2



// REMPLACER TOUS LES C PAR DES B DANS LES PORTS MAINTENANT !!!


/** \addtogroup lcd*/
/*@{*/
/**
* \fn void LCD_effacer(char ligne)
* \brief Cette fonction permet d'effacer une ligne en particulier et de revenir au debut de cette ligne
* \param [IN] ligne est le numero de la ligne a effacer, 1 ou 2
*/
void LCD_effacer(char ligne)
{
    if(ligne == 1 || ligne == 2)
    {
        cursor_xy(0,ligne);
        printf("                ");
        cursor_xy(0,ligne);
    }
    _delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25); // N?cessaire avant de faire un affichage ou autre
}


/**  \brief Envoie une commande au LCD
     \param [in] func = code de la commande a envoyer, voir file Lcd.h

     Envoie une commande au lcd via

    - PORTB = BUS de controle (RW,RS,E)
    - PORTA = Bus de données

    Exemple \code LCD_function_set(LCD_CLEAR); \endcode

    \note PORTC et PORTA doivent etre configurés comme sortie au préalable
     : aucun delai d'attente supplémentaire n'est     nécessaire , le delay
     ( 160 ou 4us etant inclus dans cette fonction )
*/

void LCD_function_set(char func)
{
    PORTB = PORTB & ~(1<<RS);
    PORTB = PORTB &~(1<<RW);
    PORTB = PORTB |(1<<E);
    PORTA = func;
    _NOP();
    PORTB = PORTB &~(1<<E);
    _NOP();

}

/** \brief Envoie un caractere au LCD
     \param [in] caractere le caractere a envoyer

         l'envoie se fait via

    - PORTB = BUS de controle (RW,RS,E)
    - PORTA = Bus de données

    Note : ne pas oublier d'initialiser les ports dans le bon sens et d'initialisater le LCD
    Exemple \code LCD_putchar('a'); \endcode

    */

void LCD_envoyer_car(char caractere)    // cette fonction envoie un caractere vers le LCD
{
    PORTB = PORTB |(1<<RS);
    PORTB = PORTB &~(1<<RW);
    PORTB = PORTB |(1<<E);
    PORTA = caractere;
    _NOP();
    PORTB = PORTB &~(1<<E);
    _NOP();
    _delay_us(40);
}

int LCD_putschar(char caractere,FILE *stream)
{
    PORTB = PORTB |(1<<RS);
    PORTB = PORTB &~(1<<RW);
    PORTB = PORTB |(1<<E);
    PORTA = caractere;
    _NOP();
    PORTB = PORTB &~(1<<E);
    _NOP();
    _delay_us(40);


}
/** \brief Initialise le LCD

         Initialise les ports en E/S et les Modes de fonctionnement du LCD
   Exemple \code LCD_init(); \endcode

    */
void LCD_init(void)
{
    DDRA = 0xFF;
    DDRB = 0xFF;
    _delay_ms(25);_delay_ms(15); // Attendre que l'alimentation ait lieu
    LCD_function_set(0x38); // 00110000
    _delay_us(49);
    LCD_function_set(0x0F); // 00001100
    _delay_us(49);
    LCD_function_set(0x01);
    _delay_ms(2);
    LCD_function_set(0x06);
    _delay_ms(2);

    // Indiquer que le LCD est fonctionnel
    cursor_xy(0,0);
    printf("LCD INITIALISE");
    attente(10);
    LCD_effacer(2);     // Effacer la ligne 1
}

/** \brief Placer le curseur sur un emplacement précis du LCD
     \param [in] x est le numéro de la colonne, qui prend une valeur de 0 à 15 ou 0 est le caractere le plus a gauche de l'ecran
            [in] y est le numéro de la ligne, qui prend la valeur 1( première ligne )ou 2( deuxieme ligne)
         l'envoie se fait via
*/
void cursor_xy(char x, char y)
{
    PORTA=0;
    PORTB = PORTB & ~(1<<RS);
    PORTB = PORTB &~(1<<RW);
    PORTB = PORTB |(1<<E);
    PORTA=PORTA|(1<<7);
    if (y==1)
    {
        PORTA=PORTA |x;
    }
    else
    {
        PORTA=PORTA|(1<<6);
        PORTA=PORTA &~(1<<5);
        PORTA=PORTA &~(1<<4);
        PORTA=PORTA |x;
    }
    _NOP();
    PORTB = PORTB &~(1<<E);
}


/**
* \fn void defiler(unsigned char car, unsigned char j, unsigned char ligne)
* \brief Cette fonction permet de faire defiler un caractere sur une ligne en particulier a partir d'une certaine case
* \param [IN] car est le caractere que l'on souhaite faire defiler
         [IN] j est le numéro de colonne jusqu'au quel on souhaite faire defiler le caractere ??????????????????????
         [IN] ligne est la ligne sur laquelle on souhaite faire defiler le caractere
*/
void defiler(unsigned char car, unsigned char j, unsigned char ligne)
{
    int k = 0;
    for(k=16; k>=j; k--)
    {
        cursor_xy(k,ligne);
        _delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
        printf("%c",car);
        printf(" ");
        _delay_ms(10);
    }
}

