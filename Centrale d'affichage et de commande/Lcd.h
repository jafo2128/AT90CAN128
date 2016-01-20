/** \file
    \brief Entetes relatives aux commandes LCD et fonctions du clavier et lcd */

#include <stdio.h>    // pour FILE


/*  signaux contrôle  du bus Port xx */
#define E      0   // LIgne E du bus (enable) sur portC.0
#define RW     1   // LIgne RW du bus (Read/Write) sur portC.1
#define RS     2   // LIgne RS du bus (RS) sur portC.2


/** \defgroup lcd */
/** \addtogroup lcd Fonctions pour la commande du LCD */
/** \todo Definir les autres commandes du LCD .....
/*@{*/
#define LCD_CLEAR    0x01    /*< \brief Efface le LCD */

void LCD_function_set(char func);
void LCD_init(void);
void LCD_envoyer_car(char caractere);
int LCD_putschar(char caractere,FILE *stream);
void defiler(unsigned char car, unsigned char j, unsigned char ligne);

static FILE std_out_lcd = FDEV_SETUP_STREAM(LCD_putschar, NULL, _FDEV_SETUP_WRITE);

void cursor_xy(char x, char y);


/*@}*/

/** _NOP() Permet d'utiliser la macro _NOP(); (avec le poitn vrugle apres) : comme si il s'agissait
    d'une fonction : insert en fait une inscruction NOP (opcode 0x0000) dans le code assembleur
utile pour des nanodelay (d'un cycle CPU en fait)  par exemple entre un PORTB=0xF0 et  x=PINB
 */
#define _NOP()	asm volatile("nop"::)












