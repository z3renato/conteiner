#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <conio.h>
#include <time.h>

#define ENTRADA "teste10000.txt"
#define TAM_POPULACAO 500
#define N_ITERACOES 100000

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
		//v[i] = 1;
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

void mostra_populacao(int **pop, int tam_pop, int tam_ind, int *pesos, int capacidade, int *objetivos){
	int i, j;
	printf("\nPopulação criada\n");
	for (i = 0; i < tam_pop; i++)	{
		printf("Individuo %5d : ", i+1);
			for ( j = 0; j < tam_ind; j++){
				printf("%d ", pop[i][j] );

		}
		if(avalia_individuo(pop[i], tam_ind, pesos, capacidade))
			printf("\t Individuo válido\t");
		else
			printf("\t Individuo inválido\t");
		printf("\tFunção objetivo: %d", objetivos[i]);
		printf("\n");
	}
}

void mostra_individuo(int *individuo, int tam){
	int i;
	printf("\n");
	for(i = 0; i < tam; i++)
		printf("%d ", individuo[i]);
	

	
}

void mostra_objetivos(int *objetivos, int tam_pop){
	int i;
	printf("\nFunções objetivo\n");
	for(i = 0; i<tam_pop; i++){
		printf("%d\n",objetivos[i] );

	}

}
int peso_individuo(int *individuo, int tam, int *pesos){
	int i, soma = 0;
	for(i=0; i < tam; i++){
		if(individuo[i])
			soma+=pesos[i];
	}

	return soma;


}

//-----------------------------------------------------------------------------------------------------

/************************************
*Funções Genéticas
************************************/

void mutacao(int *individuo, int tam_ind){

	int i, a;
	int r = rand() % tam_ind;
	for(i=0; i< r; i++){
		a = rand() % tam_ind;
		individuo[a] = !individuo[a];
	}
}

void cruzamento(int *ind1, int *ind2, int tam_ind, int *new_ind){
	int i, r;
	new_ind = alocaVetor(tam_ind);
	for(i=0; i<tam_ind;i++){
		r = rand() % 2;
		if(!r){
			new_ind[i] = ind1[i];
		}else
			new_ind[i] = ind2[i];
	}

	
}

int avalia_individuo(int *individuo, int tam, int *pesos, int capacidade){
	int i, soma = 0;
	for(i=0; i < tam; i++){
		if(individuo[i])
			soma+=pesos[i];
	}
//	printf("\t %6d\t", soma);
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

void carrega_objetivos(int **populacao, int tam_pop, int tam_ind, int *valores, int *objetivos){
	int i;
	for(i = 0; i < tam_pop; i++){
		objetivos[i] = funcao_objetivo(populacao[i], tam_ind, valores);
	}	
}

void penaliza_objetivo(int *objetivo){
	*objetivo -= -(*objetivo)/2;
}

void trata_invalidos(int **pop, int tam_pop, int tam_ind, int *pesos, int capacidade, int *objetivos){
	int i;
	for(i = 0; i < tam_pop; i++){
		if(!avalia_individuo(pop[i], tam_ind, pesos, capacidade))	
			penaliza_objetivo(&objetivos[i]);
	}
}

void repara_individuo(int *individuo, int tam, int *pesos, int capacidade){
	int i, soma=0;
	for(i = 0; i < tam; i++){
		if(individuo[i])
			soma+=pesos[i];

		if(soma > capacidade){
			i--;
			break;
		}
	}
	for(; i< tam; i++){
		individuo[i] = 0;
	}
}
int torneio (int *ind1, int *ind2, int tam, int *objetivos, int i1, int i2){
	int vencedor;
	int objetivo1 = objetivos[i1];
	int objetivo2 = objetivos[i2];
	if(objetivo1 > objetivo2)
		return i2;	

	return i1;
}

void seleciona_jogadores(int tam_pop, int *j1, int *j2){
	
	*j1 = rand()%tam_pop;
	*j2 = rand()%tam_pop;
	if( *j1 == *j2 )
		*j2 = rand()%tam_pop;

}
//----------------------------------------------------------------------------------------------------

int melhor_individuo(int *objetivos, int tam_pop ){
	int i, maior = 0;
	for(i=0; i < tam_pop; i++)
		if(objetivos[i] > objetivos[maior])
			maior = i;
	
	return maior;
}

int run(){
	srand(time(NULL)); //semente para geração aleatória dos números
	int capacidade = 0, num_objetos = 0; // Capacidade = Capacidade do conteiner ** num_objetos = Número de itens disponíveis para colocar no conteiner
	num_objetos = num_obj();
	if(!num_objetos){
		printf("\nSem objetos para colocar na mochila!\n"); // Caso o número de objetos seja 0
		return 1;
	}
	/***********************************************
	*Declaração inicial e carregamento dos dados
	***********************************************/
	int pesos[num_objetos], valores[num_objetos]; //pesos = pesos dos objetos ** valores = valor dos objetos
	int objetivos[TAM_POPULACAO]; // vetor com a função objetivo de cada indivíduo da população
	carregaDados(&capacidade, &num_objetos, pesos, valores);
	//printf("Capacidade do conteiner: %d\n", capacidade );
	//mostra_objetos(pesos, valores, num_objetos);
	int **populacao; // para armazenar a matriz que representa a população

	//------------------------------------------------------

	/*****************************************************************************
	*Inicialização da população e objetos que serão usados no algorítmo genético
	*****************************************************************************/

	// i

	populacao = cria_populacao(num_objetos, TAM_POPULACAO); //Criando uma população de tamanho pré-definido (TAM_POPULACAO)
	carrega_objetivos(populacao, TAM_POPULACAO, num_objetos, valores, objetivos);

	//mostra_populacao(populacao, TAM_POPULACAO, num_objetos, pesos, capacidade, objetivos); // Função para exibir a população gerada
	//mostra_objetivos(objetivos, TAM_POPULACAO);
	
	//---------------------------------------------------------------------------
	int cont = 0, maior_objetivo = 0, maior = 0;
	int j1, j2, r, perdedor;
	//
	while(cont < N_ITERACOES){

		trata_invalidos(populacao, TAM_POPULACAO, num_objetos, pesos, capacidade, objetivos);
		seleciona_jogadores(TAM_POPULACAO, &j1, &j2);
		perdedor = torneio(populacao[j1], populacao[j2], num_objetos, objetivos, j1, j2);
		int operacao_genetica;
		operacao_genetica = rand() % 2;
		if(!operacao_genetica){
			r = rand() % TAM_POPULACAO;
			if(perdedor == j1){
				cruzamento(populacao[j2], populacao[r], num_objetos, populacao[perdedor]);
			}else
				cruzamento(populacao[j1], populacao[r], num_objetos, populacao[perdedor]);
		}else{
			mutacao(populacao[perdedor], num_objetos);
		}
		
		carrega_objetivos(populacao, TAM_POPULACAO, num_objetos, valores, objetivos);
		maior_objetivo = melhor_individuo(objetivos, TAM_POPULACAO);
		if(objetivos[maior_objetivo] > objetivos[maior]){
			maior = maior_objetivo;
		}//nt *individuo, int tam, int *pesos, int capacidad
		if(!avalia_individuo(populacao[maior_objetivo], num_objetos, pesos, capacidade))
			repara_individuo(populacao[maior_objetivo], num_objetos, pesos, capacidade);
		
		if(maior_objetivo == maior)
			cont++;


	}
	int melhor = melhor_individuo(objetivos, TAM_POPULACAO);
	//mostra_populacao(populacao, TAM_POPULACAO, num_objetos, pesos, capacidade, objetivos); // Função para exibir a população gerada

	printf("\nMelhor individuo: %d  ", melhor+1);
	//mostra_individuo(populacao[melhor], num_objetos);

	printf("\nValor da carga: %9d Peso da carga: %6d\n", objetivos[melhor], peso_individuo(populacao[melhor], num_objetos, pesos));
	return 0;

}











int main(){
	if(!run())
		printf("\nSucesso!\n");
	else
		printf("\nDeu ruim!\n");




}