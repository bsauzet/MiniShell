#include "Shell.h"
#include "Evaluation.h"
#include "Commandes_Internes.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

void
verifier(int cond, char *s)
{
    if (!cond)
    {
        perror(s);
        exit(1);
    }
}

void command_pipe(Expression *e) {
    // création du pipe
    int tube[2];
    pipe(tube);
    // 2 processus pour le pipe
    pid_t pid;
    pid=fork();

    if (pid != 0) {// processus père
        close(tube[1]);
        dup2(tube[0], STDIN_FILENO);
        evaluer_expr(e->droite);
    }
    else { // processus fils
        close(tube[0]);
        dup2(tube[1], STDOUT_FILENO);
        evaluer_expr(e->gauche);
    }
}

void redirection_out(Expression *e, int append) {
	// on sauve stdout pour le restaurer
	int stdout = dup(1);
	// descripteur de fichier
	int fd;
	
	// si append est faux, on utilise O_TRUNC
	if(!append)
		fd = open(e->arguments[0], O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else {
		fd = open(e->arguments[0], O_CREAT | O_WRONLY, 0666);
		lseek(fd, 0, SEEK_END);
	}
	
	// On redirige la sortie standard vers l'entrée d'un fichier
	dup2(fd, STDOUT_FILENO);
	
	// execution
	evaluer_expr(e->gauche);
	
	close(fd);
	dup2(stdout, 1);
	close(stdout);
}

void redirection_in(Expression *e) {
	// on sauve stdin pour le restaurer
	int stdin = dup(0);
	// descripteur de fichier
	int fd = open(e->arguments[0], O_RDONLY);
	
	// On redirige la sortie du fichier vers l'entrée standard
	dup2(fd, STDIN_FILENO);
	
	// execution
	evaluer_expr(e->gauche);
	
	close(fd);
	dup2(stdin, 0);
	close(stdin);
}

void subshell(Expression *e) {

		// Création d'un noeud qui appelle un MiniShell, ustiliser pstree -s pour voir le niveau de profondeur
		char* args[2] = {"./Shell", NULL};
		Expression* shell = ConstruireNoeud(SIMPLE, NULL, NULL, args);    
		// execution du pipe
		Expression* expr = ConstruireNoeud(PIPE,e->gauche,shell,NULL);
		evaluer_expr(expr->gauche);
}

bool is_commande_interne(Expression *e) {
	
	char* command = e->arguments[0];
	
	if(strcmp(command, "echo") == 0)
		echo(e);
	else if(strcmp(command, "date") == 0)
		date();
	//else if(strcmp(command, "cd") == 0)
	
	else if(strcmp(command, "pwd") == 0)
		pwd();
	/*else if(strcmp(command, "history") == 0)
	
	else if(strcmp(command, "hostname") == 0)
	
	else if(strcmp(command, "kill") == 0)
	
	else if(strcmp(command, "exit") == 0)*/
	
	else 
		return false;
	return true;
}

int
evaluer_expr(Expression *e)
{
    pid_t pid;
    int status = 0;
    pid=fork();

    // Si la commande n'est pas lancée en arrière plan, on attend la fin du fils pour continuer l'utilisation du shell
    if(pid!=0 && e->type != BG)
        wait(&status);
    // En fonction du type de commande, le fils va exécuter la commande
    else
        switch(e->type) {
        case SIMPLE :
            if(! is_commande_interne(e))
				execvp(e->arguments[0], e->arguments);
            break;
        case BG :
            evaluer_expr(e->gauche);
            break;
        case SEQUENCE :
            evaluer_expr(e->gauche);
            evaluer_expr(e->droite);
            break;
        case PIPE :
            command_pipe(e);
            break;
		case SEQUENCE_ET :
			if(evaluer_expr(e->gauche) == 0)  // si la premiere expression échoue, on n'effectue pas la seconde
				evaluer_expr(e->droite);
			break;
		case SEQUENCE_OU :
			if(evaluer_expr(e->gauche) != 0)  // si la premiere expression échoue, on effectue la seconde
				evaluer_expr(e->droite);
			break;
		case REDIRECTION_O :
			redirection_out(e, 0);
			break;
		case REDIRECTION_A :
			redirection_out(e, 1);
			break;
		case REDIRECTION_I :
			redirection_in(e);
			break;
		case SOUS_SHELL :
			subshell(e);
			break;
        default :
            printf("default\n");
        }
    // On vérifie régulièrement si un fils s'est terminé pour se débarasser des zombies
    waitpid(-1,&status,WNOHANG);

    return status;
}
