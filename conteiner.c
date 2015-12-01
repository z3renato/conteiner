#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <conio.h>
#include <time.h>

#define ENTRADA "teste20.txt"
#define TAM_POPULACAO 10

/******************************
*Funções de leitura do arquivo
******************************/

int num_obj(){
	FILE * entrada;
	entrada = fopen(ENTRADA, "r");
	int n;
	fscanf(entrada, "%d", &n);
	fscanf(entrada, "%d", &n);
	fclose(entrada);
	return n;
}

void carregaDados(int *C, int *n, int *p, int *v){
	FILE * entrada;
	entrada = fopen(ENTRADA, "r");
	fscanf(entrada, "%d", C);
	fscanf(entrada, "%d", n);
	int i;
	
	//printf("\n%d | %d\n", *C, *n );

	for(i=0; i< (*n);i++){
		//printf("\n%d | %d\n", *C, *n );
		fscanf(entrada, "%d\t%d",&p[i], &v[i]);
		//printf("peso: %d | valor: %d \n", p[i], v[i]);
	}
	fclose(entrada);
}
//---------------------------------------------------------------------------------------------------------

/******************************
*Funções para gerar os cromossomos
*******************************/
void rnd(int tam, int *v){

	
	int i;
	
	for (i = 0; i < tam; i++){
		v[i] = rand() % 2;
	}
	
}

int* alocaVetor(int tam){
	int *p;
	p = malloc(tam * sizeof(int));
	return p;
}

int * cria_individuo(int tam){
	int  *conteiner;
	conteiner = alocaVetor(tam);
		
	rnd(tam, conteiner);
	return conteiner;
	

}

int ** cria_populacao(int tam_ind, int tam_pop){
	int **pop, i;
	pop = malloc(tam_pop* sizeof(int*));
	
	for(i=0; i< tam_pop; i++){
		pop[i] = cria_individuo(tam_ind);
	}

	return pop;

	
}

//---------------------------------------------------------------------------------------------------


/******************************************
*Funções para saída de informações
******************************************/

void mostra_objetos(int *p, int *v, int n){
	int i;
	
	printf("\n%6s\t%6s\t%6s\n", "Objeto", "Peso", "Valor");
	for (i = 0; i < n; i++){
		printf("%6d\t%6d\t%6d\n", i+1, p[i], v[i]);
	}
}

void mostra_populacao(int **pop, int tam_pop, int tam_ind, int *pesos, int capacidade){
	int i, j;
	printf("\nPopulação criada\n");
	for (i = 0; i < tam_pop; i++)	{
		printf("Individuo %2d : ", i+1);
			for ( j = 0; j < tam_ind; j++){
				printf("%d ", pop[i][j] );

		}
		if(avalia_individuo(pop[i], tam_ind, pesos, capacidade))
			printf("\t Individuo válido");
		else
			printf("\t Individuo inválido");
		
		printf("\n");
	}
}

void mostra_individuo(int *individuo, int tam){
	int i;
	printf("\n");
	for(i = 0; i < tam; i++)
		printf("%d ", individuo[i]);
	

	
}

//-----------------------------------------------------------------------------------------------------

/************************************
*Funções Genéticas
************************************/

void mutacao(int *individuo, int tam_ind){
	int i;
	for(i=0; i< tam_ind; i++){
		individuo[i] = !individuo[i];
	}
}

void *cruzamento(int *ind1, int *ind2, int tam_ind){
	int i, *new_ind;
	new_ind = alocaVetor(tam_ind);
	for(i=0; i<tam_ind;i++){
		if(i%2 == 0)
			new_ind[i] = ind1[i];
		else
			new_ind[i] = ind2[i]; 
	}

	return new_ind;
}

int avalia_individuo(int *individuo, int tam, int *pesos, int capacidade){
	int i, soma = 0;
	for(i=0; i < tam; i++){
		if(individuo[i])
			soma+=pesos[i];
	}
	if(soma <= capacidade)
		return 1;
	return 0;
}

int funcao_objetivo(int *individuo, int tam, int *valores){
	int i, objetivo=0;
	for(i=0; i < tam; i++){
		if(individuo[i])
			objetivo+=valores[i];
	}

	return objetivo;

}



//----------------------------------------------------------------------------------------------------













int main(){
	srand( time(NULL) ); //semente para geração aleatória dos indivíduos
	int C = 0, n = 0; // C = Capacidade do conteiner ** n = Número de itens
	n = num_obj();
	if(!n){
		printf("\nSem objetos para colocar na mochila!\n");
		return 1;
	}

	int p[n], v[n]; //p = pesos dos objetos ** v = valor dos objetos
	carregaDados(&C, &n, p, v);
	printf("Capacidade do conteiner: %d\n", C );
	mostra_objetos(p, v, n);
	int **teste, i, j;
	teste = cria_populacao(n, TAM_POPULACAO);
	mostra_populacao(teste, TAM_POPULACAO, n, p, C);
	//mutacao(teste[0], n);
	





}