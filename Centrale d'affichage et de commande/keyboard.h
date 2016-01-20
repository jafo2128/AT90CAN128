#include <stdio.h> 
#define NB_ECHEC 3 // Nombre d'essai pour entrer le code
#define CODE "87BC"  // Code pour deverrouillé le systeme

char obtenir_code(void);
int demande_lock_unlock(char lock);
void init_Clavier(void);
