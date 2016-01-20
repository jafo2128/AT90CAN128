#include <avr/io.h>
#include "config.h"	//définit la fréquence du quartz et celle du bus CAN
#include "can_lib.h"	//librairie CAN de Atmel
#include <avr/power.h>
#include <util/delay.h>
/* Mise en place du controle d'autonomie sur le MCP25050 2013-2014
Ce programme permet de reconfigurer un MCP en envoyant les trames de reconfiguration */

void init_emission(U8 *buffer);
void init_reception(U8 *buffer);

st_cmd_t msg1;      // description du message 1

// configuration a envoyer vers le MCP25050 
U8 buffer1[3]= {0x2C /*adresse du registre STCON*/, 0b11111111 /*masque */, 0x70 /*valeur*/};
U8 buffer2[3]= {0x1F /*adresse du registre GPDDR*/, 0x0F /*masque */, 0b00001111 /*valeur*/};
U8 buffer3[3]= {0x0E /*adresse du registre ADCON0*/, 0b11110000 /*masque */, 0b01110000 /*valeur*/};
U8 buffer4[3]= {0x0F /*adresse du registre ADCON1*/, 0b11111111 /*masque */,  0b10001101/*valeur*/};
U8 buffer5[3]= {0x1C /*adresse du registre IOINTEN*/, 0b00000011 /*masque */,  0b00000010/*valeur*/};
U8 buffer6[3]= {0x1D /*adresse du registre IOINTPO*/, 0b00000011 /*masque */,  0b00000010/*valeur*/};
U8 buffer7[3]= {0x04 /*adresse du registre OPTREG1*/, 0b11111111 /*masque */,  0b11110011/*valeur*/};
U8 buffer8[3]= {0x30 /*adresse du registre ADCMPNH*/, 0b11111111 /*masque */,  0x7D/*valeur*/};
U8 buffer9[3]= {0x31 /*adresse du registre ADCMPNL*/, 0b11000001 /*masque */,  0x00/*valeur*/};


int result;

int main (void) {
	
	/* Disable clock division */
	
	clock_prescale_set(clock_div_1);
	
	//initialisation de la liaison CAN
	can_init(0x00);

	DDRA=0xFF;
	PORTA =0x00;
	_delay_ms(10);_delay_ms(10);_delay_ms(10);
	PORTA =0xFF;

// on configure le MCP25050 avec buffer1
	while(1){

	init_emission(buffer1);
	while(can_cmd(&msg1) != CAN_CMD_ACCEPTED); // attend que la commande soit acceptée
	while(can_get_status(&msg1) == CAN_STATUS_NOT_COMPLETED);// attend que la commande soit effectuée
	_delay_ms(1);



//configuration du MCP25050
// on configure les entrees/sorties du MCP25050


	init_emission(buffer2);
	while(can_cmd(&msg1) != CAN_CMD_ACCEPTED); // attend que la commande soit acceptée
	while(can_get_status(&msg1) == CAN_STATUS_NOT_COMPLETED);// attend que la commande soit effectuée
	_delay_ms(1);
	


// on configure ADCON0
	init_emission(buffer3);
	while(can_cmd(&msg1) != CAN_CMD_ACCEPTED); // attend que la commande soit acceptée
	while(can_get_status(&msg1) == CAN_STATUS_NOT_COMPLETED);// attend que la commande soit effectuée
	_delay_ms(10);


// on configure ADCON1

	init_emission(buffer4);
	while(can_cmd(&msg1) != CAN_CMD_ACCEPTED); // attend que la commande soit acceptée
	while(can_get_status(&msg1) == CAN_STATUS_NOT_COMPLETED);// attend que la commande soit effectuée
	_delay_ms(10);


	
// on configure IOINTEN

	init_emission(buffer5);
	while(can_cmd(&msg1) != CAN_CMD_ACCEPTED); // attend que la commande soit acceptée
	while(can_get_status(&msg1) == CAN_STATUS_NOT_COMPLETED);// attend que la commande soit effectuée
	_delay_ms(10);
	

	
// on configure la comparaison H

	init_emission(buffer6);
	while(can_cmd(&msg1) != CAN_CMD_ACCEPTED); // attend que la commande soit acceptée
	while(can_get_status(&msg1) == CAN_STATUS_NOT_COMPLETED);// attend que la commande soit effectuée
	_delay_ms(10);
	

// on configure la comparaison L

	init_emission(buffer7);
	while(can_cmd(&msg1) != CAN_CMD_ACCEPTED); // attend que la commande soit acceptée
	while(can_get_status(&msg1) == CAN_STATUS_NOT_COMPLETED);// attend que la commande soit effectuée

	_delay_ms(10);

	// on configure la polarite

	init_emission(buffer8);
	while(can_cmd(&msg1) != CAN_CMD_ACCEPTED); // attend que la commande soit acceptée
	while(can_get_status(&msg1) == CAN_STATUS_NOT_COMPLETED);// attend que la commande soit effectuée
	_delay_ms(10);
	_delay_ms(10);_delay_ms(10);_delay_ms(10);

	

		init_emission(buffer9);
	while(can_cmd(&msg1) != CAN_CMD_ACCEPTED); // attend que la commande soit acceptée
	while(can_get_status(&msg1) == CAN_STATUS_NOT_COMPLETED);// attend que la commande soit effectuée
	_delay_ms(10);
_delay_ms(10);_delay_ms(10);_delay_ms(10);

}
		init_reception(buffer1);

		while(can_cmd(&msg1) != CAN_CMD_ACCEPTED); // attend que la commande soit accepté
		while(can_get_status(&msg1) == CAN_STATUS_NOT_COMPLETED);// attend que la commande soit effectuée
		if((buffer1[0]&0x01)==0x01 && (buffer1[1]&0x01)==0x01) PORTA=~PORTA;//affichage des données reçues sur les leds
		
		while(1){
		result= buffer1[1];
		result= result <<8;
	//	result= buffer1[1];
		PORTA = ~result ; 
	}

return 0;
}

void init_emission(U8 *buffer) {
	//msg1.handle = ;					//managed by library
	msg1.cmd = CMD_TX_DATA;				//active l'emission
	msg1.id.std = 0x680 ;				//identifiant du 2eme MCP25050
	msg1.dlc = 3;						//longueur du message
	msg1.pt_data = buffer;				//pointeur sur les données à envoyer
	msg1.status = MOB_NOT_COMPLETED;	//statut par défaut de la commande 
	//msg1.ctrl =
}


void init_reception(U8 *buffer) {
	//msg1.handle = ;					//managed by library
	msg1.cmd = CMD_RX_DATA;				//active la reception
	msg1.id.std = 0x7FF;				//canal sur lequel la trame va etre reçue
	msg1.dlc = 2;						//longueur de la trame de données en octet
	msg1.pt_data = buffer;				//adresse où stocker les données
	msg1.status = MOB_NOT_COMPLETED;	//statut par défaut de la commande 
	//msg1.ctrl = 

}
/*void init_emission1(U8 *buffer) {
	//msg1.handle = ;					//managed by library
	msg1.cmd = CMD_TX_DATA;				//active l'emission
	msg1.id.std = 0x681 ;				//identifiant du 2eme MCP25050
	msg1.dlc = 3;						//longueur du message
	msg1.pt_data = buffer;				//pointeur sur les données à envoyer
	msg1.status = MOB_NOT_COMPLETED;	//statut par défaut de la commande 
	//msg1.ctrl =
}
*/
