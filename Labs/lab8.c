/*

INF1019 - LABORATÓRIO 8
3WB

MATHEUS CALDAS
1312760

1) Implemente um programa que crie 2 threads:

Uma delas (contCrescente) contará de 1 a N=20 (compausas de T1=1 segundo)

A outra (contDecrescente) de M=30 a 1 (com pausas deT2=2 segundos).

Compile com a opção –l pthread

*/

#include <pthread.h>
#include <stdio.h>

void *contCrescente()
{
	for (int i = 1; i <= 20; i++)
		{
			printf("Valor de N: %d\n", i);
			sleep(1);
		}	
}

void *contDecrescente()
{
	for (int i = 30; i > 0; i--)
		{
			printf("Valor de M: %d\n", i);
			sleep(2);
		}	
}

int main ()
{

	pthread_t threadCres;
	pthread_t threadDecres;

	pthread_create(&threadCres, NULL, contCrescente, NULL);
	pthread_create(&threadCres, NULL, contDecrescente, NULL);

		//a thread invocadora é bloqueada até que cada uma das 2 threads termine.
		//o valor de retorno da thread executando o exit(0) será copiada para o segundo argumento
		//no caso NULL (nao retorna nada)
		pthread_join(threadCres, NULL);
		pthread_join(threadDecres, NULL);


}

/*
O exercício consistia em basicamente criar duas threads, feitas com pthread_create que executavam
cada uma, uma função. contCrescente contava crescente e contDecrescente contava descrescente.

Foi utilizado também a função pthread_join para sinalizar a main pra ela ficar 'bloqueada' até que cada uma
das threads termine.

Foi possível ver que elas estavam ocorrendo simultaneamente, e uma escrevia mais rápida que a outra devido
a diferença de tempo nos sleeps. A saída foi como esperada.
*/


/*
2) Acrescente agora uma variável global inteira que é inicializada com zero, 
incrementada e impressa na tela por cada thread. Verifique que ambas as
threads manipulam a mesma variável.
*/


#include <pthread.h>
#include <stdio.h>

int varGlobal = 0;

void *contCrescente()
{
	for (int i = 1; i <= 20; i++)
		{
			printf("Valor de N: %d\n", i);
			varGlobal +=1;
			printf("=== GLobal : %d ====\n", varGlobal);
			sleep(1);
		}	
}

void *contDecrescente()
{
	for (int i = 30; i > 0; i--)
		{
			printf("Valor de M: %d\n", i);
			varGlobal +=1;
			printf("=== GLobal : %d ====\n", varGlobal);
			sleep(2);
		}	
}

int main ()
{

	pthread_t threadCres;
	pthread_t threadDecres;

	pthread_create(&threadCres, NULL, contCrescente, NULL);
	pthread_create(&threadCres, NULL, contDecrescente, NULL);

		//a thread invocadora é bloqueada até que cada uma das 2 threads termine.
		//o valor de retorno da thread executando o exit(0) será copiada para o segundo argumento
		//no caso NULL (nao retorna nada)
		pthread_join(threadCres, NULL);
		pthread_join(threadDecres, NULL);


}

/*
O exercício era basicamente o mesmo, a diferença é que agora temos a variável global varGlobal
que é compartilhada por ambos as threads (diferente do que ocorria com os processos por exemplo).

Foi possível observar na saída que a variável global ia incrementando a cada execução de de cada thread.
(evidenciando que ela está sendo compartilhada por ambas as threads)
*/


/*
3) Usando threads, escreva um programa C que implemente o problema do produtor/consumidor. 
O produtor e o consumidor devem ser threads de execução dentro do mesmo processo
compartilhando os recursos do processo. O produtor deve produzir dados
(números inteiros pseudo-aleatórios) a cada 1 segundo colocando-os em uma fila 
(buffer, implementaçãocircular). O consumidor deve retirar dados da fila a cada 2
segundos. O tamanho máximo da fila deve ser de 8 elementos (MAXFILA) e tanto o 
produtor como o consumidor devem produzir/consumir 64 elementos (números inteiros de 1 a 64,
por exemplo) evitando condições de corrida. Variáveis compartilhadas entre threads 
são simplesmente variáveis globais.
*/


#include <pthread.h>
#include <stdio.h>


#define NUM_MAXIMO 8
#define TOTAL 64

//vetor com os valores
int vetor[8];

//proximo elemento a ser retirado.
int ini = 0;

//numero de elementos
int numElementos = 0;

void insere(int elemento);
int removeElem();
void exibe();

void insere(int elemento)
{
	int fim;
	if(numElementos ==8)
		printf("Fila cheia -- Elemento %d não inserido\n", elemento);
	else
	{
		//nao esta cheia, vamos inserir elemento
		fim = (ini + numElementos) % NUM_MAXIMO;
		vetor[fim] = elemento;
		numElementos++;

		printf("Produzido %d\n", elemento);
	}

}

int removeElem()
{	
	int valor;

	if(numElementos == 0)
		printf("Fila vazia\n");
	else
	{
		valor = vetor[ini];
		ini = (ini + 1) % NUM_MAXIMO;
		numElementos--;
		printf("Consumido %d\n", valor);
		return valor;
	}

	return -1;
}


void *produz()
{
	for (int i = 0; i < TOTAL; i++)
		{
			sleep(1);
			insere(i);
		}	
}

void *consome()
{
	for (int i = 0; i < TOTAL; i++)
		{
			sleep(2);
			int valor = removeElem();
		}	
}

int main ()
{

	pthread_t threadCres;
	pthread_t threadDecres;

	pthread_create(&threadCres, NULL, produz, NULL);
	pthread_create(&threadCres, NULL, consome, NULL);

		//a thread invocadora é bloqueada até que cada uma das 2 threads termine.
		//o valor de retorno da thread executando o exit(0) será copiada para o segundo argumento
		//no caso NULL (nao retorna nada)
		pthread_join(threadCres, NULL);
		pthread_join(threadDecres, NULL);


}

/*
Neste exercício removeElem e insere são apenas funções que implementam uma fila circular
feita atráves de um vetor com um limite de 8 valores.

As threas em si executam as funções 'produz' e 'consome'. Essas funções que chamam as operações
a serem feitas na fila.

Foi necessário também tratar os casos quando o consumidor tentava inserir algum valor na fila
mas ela estava cheia. (No caso o elemento era ignorado).

A saída foi como esperado. Como a produção era maior que o consumo não havia o problema de se tentar
consumir e a fila aparecer vazia.

Foi possível observar o funcionamento das threads, como elas são criadas e executadas, assim como
implementar uma fila circular. E também a questão das variáveis globais que nada mais são que variáveis
compartilhadas por todas nossas threads.


*/