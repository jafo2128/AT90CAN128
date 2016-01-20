#include <stdio.h>




// La description de ces fonctions est faites dans buzzer.c
void start_timer0(unsigned short int ocr0a);
void stop_timer0(void);
void start_timer1(unsigned short int ocr1a);
void stop_timer1(void);
void wait_OCR1A_timer1(void);
void note(char temps, int note);
void init_Buzzer(void);
void activer_Buzzer(char mode);
