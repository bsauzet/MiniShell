#include "Commandes_Internes.h"

int echo(Expression* e) {
	
	printf("%s\n", e->arguments[1]);
	return 0;
}
