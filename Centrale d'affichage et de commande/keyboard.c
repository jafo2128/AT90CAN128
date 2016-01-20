#include <avr/io.h>
#include <util/delay.h>
#include "keyboard.h"

extern int ETAT_VERROUILLAGE;
/**
* \fn void init_Clavier(void)
* \brief Initialisation des ports necessaire a l'utilisation du clavier
*/
void init_Clavier(void)
{
    DDRC = 0x0F;             // Ligne en entree et colonne en sortie
}

/**
* \fn void demande_lock_unlock(char lock)
* \brief Permet de gerer le verrouillage ou deverrouillage la maison
* \param [IN] lock permet d'indiquer si l'on souhaite verrouiller ou deverrouiller la maison
        - 0 : Deverrouiller la maison
        - 1 : Verrouiller la maison
        [OUT] La fonction renvoie une valeur:
        - 0 : L'utilisateur n'a pas reussi a entrer le code de securite NB_ECHEC de suite
        - 1 : L'utilisateur a reussi a verrouiller la maison
        - 2 : L'utilisateur a reussi a deverrouiller la maison
*/
int demande_lock_unlock(char lock)
{
    //====================//
    //      VARIABLES     //
    //====================//
    int i = 0;
    int j = 0;
    int k = 0;
    char c = 0;                 // contient le caractere taper par l'utilisateur
    int essai = 0;              // indique le nombre d'essai que l'utilisateur a effectue
    char code_taper[5];         // contient le code taper par l'utilisateur
    char code_reel[5] = CODE;   // Pour modifier le code de securite, il faut aller dans keyboard.h
    //====================//
    LCD_effacer(2);
    LCD_effacer(1);
    //====================//
    while(k!=4 && essai < NB_ECHEC)
    {
        if(j==0)
        {
            cursor_xy(0,1);
            if(lock==1)         // Demande de verrouillage
            {
                printf("VERROUILLAGE");
            }
            else                // Demande de deverrouillage
            {
                printf("DEVERROUILLAGE");
            }
            cursor_xy(0,2);
            printf("ESSAI %d/%d:",essai+1,NB_ECHEC);    // Afficher le nombre d'essai effectue
        }

        c = obtenir_code();

        if (c =='1'||c =='2'||c =='3'||c =='A'||c =='4'||c =='5'||c =='6'||c =='B'||c =='7'||c =='8'||c =='9'||c =='C'||c =='*'||c =='0'||c =='#'||c =='D')
        {
            code_taper[j] = obtenir_code();
            _delay_ms(10);
            printf("%c",code_taper[j]);
            j++;
        }

        if(j==4)    // Si l'utilisateur a entre 4 caracteres
        {
            j=0;
            k=0;
            // Comparaison du code
            for(i=0; i<4; i++)
            {
                if(code_taper[i] == code_reel[i])
                {
                    _delay_ms(10); // Necessaire sinon ça va trop vite et il ya des erreurs
                    k++;
                }
            }

            i=0;
            LCD_effacer(2);
            LCD_effacer(1);
            if(k!=4) // Si le code entrer est erronee, on incremente le nombre d'essai effectue
            {
                essai++;
            }
            LCD_effacer(2);
            LCD_effacer(1);
        }
    }// Fin while

    if(essai==NB_ECHEC) // Si l'utilisateur a entre trop de fois le mauvais code
    {
		ETAT_VERROUILLAGE = 2;
		LCD_effacer(1);
		LCD_effacer(2);
        printf("Alarme activee");
		_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
		_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
		_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
		_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
		_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
		_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
		LCD_effacer(1);
		LCD_effacer(2);
	
        return 0;
    }
    if(k==4) // Si le code est bon
    {
        if(lock==1)
        {	
			LCD_effacer(1);
			LCD_effacer(2);
            printf("verrouille");
			ETAT_VERROUILLAGE = 1;
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);

			LCD_effacer(1);
			LCD_effacer(2);
            return 1;
        }
        else
        {
			ETAT_VERROUILLAGE = 0;
			LCD_effacer(1);
			LCD_effacer(2);
            printf("deverouille");
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			_delay_ms(25);_delay_ms(25);_delay_ms(25);_delay_ms(25);
			LCD_effacer(1);
			LCD_effacer(2);
            return 2;
        }
    }
}// Fin fonction

/**
* \fn char obtenir_code(void)
* \brief Cette fonction renvoie le caractere taper sur le digicode
* \param [OUT] caractere contient le caractere taper par l'utilisateur
*/
char obtenir_code()
{
    char caractere;
    //====================//
    //      Colonne 1     //
    //====================//
    PORTC = 0b00001110; // C1
    _delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
	_delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
    if(PINC == 0b11101110)
    {
        return caractere = '1';  // L1
    }
    if(PINC == 0b11011110)
    {
        return caractere = '4'; // L2
    }
    if(PINC == 0b10111110)
    {
        return caractere = '7';   // L3
    }
    if(PINC == 0b01111110)
    {
        return caractere = '*'; // L4
    }
    //====================//
    //      Colonne 2     //
    //====================//
     PORTC = 0b00001101; // C2
    _delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
	_delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
    if((PINC & 0b00010000)==0)
    {
        return caractere = '2';  // L1
    }
    if((PINC & 0b00100000)==0)
    {
        return caractere = '5'; // L2
    }
    if((PINC & 0b01000000)==0)
    {
        return caractere = '8';   // L3
    }
    if((PINC & 0b10000000)==0)
    {
        return caractere = '0'; // L4
    }
    //====================//
    //      Colonne 3     //
    //====================//
    PORTC = 0b00001011; // C3
    _delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
    if((PINC & 0b00010000)==0)
    {
        return caractere = '3';  // L1
    }
    if((PINC & 0b00100000)==0)
    {
        return caractere = '6'; // L2
    }
    if((PINC & 0b01000000)==0)
    {
        return caractere = '9';   // L3
    }
    if((PINC & 0b10000000)==0)
    {
        return caractere = '#'; // L4
    }
    //====================//
    //      Colonne 4     //
    //====================//
    PORTC = 0b00000111;
    _delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
	_delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
    if((PINC & 0b00010000)==0)
    {
        return caractere = 'A';  // L1
    }
    if((PINC & 0b00100000)==0)
    {
        return caractere = 'B'; // L2
    }
    if((PINC & 0b01000000)==0)
    {
        return caractere = 'C';   // L3
    }
    if((PINC & 0b10000000)==0)
    {
        return caractere = 'D'; // L4
    }
}//FIN Fonction

/**
* \fn char menu(void)
* \brief Cette fonction renvoie le caractere taper sur la 4eme colonne du clavier
* \param [OUT] caractere contient le caractere taper par l'utilisateur
*/
char menu()
{

    char caractere;
    //====================//
    //      Colonne 4     //
    //====================//
    PORTC = 0b00000111;
    _delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
	_delay_ms(25);
    _delay_ms(25);
    _delay_ms(25);
	_delay_ms(25);
    if((PINC & 0b00010000)==0)
    {
        return caractere = 'A';  // L1
    }
    if((PINC & 0b00100000)==0)
    {
        return caractere = 'B'; // L2
    }
    if((PINC & 0b01000000)==0)
    {
        return caractere = 'C';   // L3
    }
    if((PINC & 0b10000000)==0)
    {
        return caractere = 'D'; // L4
    }
}// Fin fonction
