#define _GNU_SOURCE

#include "Commandes_Internes.h"
#include <time.h>

// écrit les arguments sur la sortie standard
void echo(Expression* e) {
	
	printf("%s\n", e->arguments[1]);
}

// donne la date et l'heure actuelle
void date() {
	
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	
	// tableaux de conversion des jours / mois en string
	char* jour[7] = {"dimanche", "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi"};
	char* mois[12] = {"janvier", "fevrier", "mars", "avril", "mai", "juin", "juillet", "aout", "septembre", "octobre", "novembre", "decembre"}; 
	
	printf("%s %d %s %d, %02d:%02d:%02d (UTC +%ld)\n", 
	jour[time.tm_wday], time.tm_mday, mois[time.tm_mon], time.tm_year + 1900, 
	time.tm_hour, time.tm_min, time.tm_sec,
	time.tm_gmtoff/36);
}

// affiche le répertoire courant
void pwd() {
	
	// on pourra rajouter un malloc/realloc pour ne pas avoir de problème de taille
	char dir[1024];
	
	printf("%s\n", getcwd(dir, sizeof(dir)));
}

// change de répertoire courant
void cd(Expression *e) {
	
	chdir(e->arguments[1]);
}

// Récupère le hostname
void hostname() {
	
	char name[1024];
	
	gethostname(name, sizeof(name));
	printf("%s\n", name); 
}
