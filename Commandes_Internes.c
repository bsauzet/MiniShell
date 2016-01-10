#define _GNU_SOURCE

#include "Commandes_Internes.h"
#include <time.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char machines[20][1024];
int nb_machines = 0;

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

// Envoie un signal au processus de pid voulu
void cmd_kill(Expression *e) {
	
	int pid = atoi(e->arguments[2]);
	int signal = - atoi(e->arguments[1]); // transforme par exemple le string -9 en l'int 9, numero du signal

	kill(pid, signal);
}

// termine le shell
void cmd_exit() {
	
	kill(getppid(), SIGKILL);
	exit(0);
}

// Affiche l'historique des commandes
void history() {

	char* tab[3] = {"cat", "hist", NULL};
	write_history("hist");
	execvp(tab[0], tab);
}

void remote(Expression *e) {
	
	char* commande = e->arguments[1];
	
	if(strcmp(commande, "add") == 0)
		remote_add(e);
	else if(strcmp(commande, "remove") == 0)
		nb_machines = 0;
//	if(strcmp(commande, "all") == 0)
	
	else if(strcmp(commande, "list") == 0)
		remote_list();
	//else
		//remote_one(e->arguments[1]);
}

// affiche la liste des machines
void remote_list() {
	
	for (int i = 0; i < nb_machines; i++)
		printf("%s\n", machines[i]);
}

// ajoute les machines données en arguments à la liste
void remote_add(Expression* e) {
	
	for (int i = 2; i < LongueurListe(e->arguments); i++)
	{
		strcpy(machines[nb_machines], e->arguments[i]);
		nb_machines ++;
	}
}

