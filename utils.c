
#include <time.h>

int sorteio(){
	srand( (unsigned)time(NULL) );
	return 	rand() % 100;
}

