/*
MATHEUS CALDAS
1312760
3WB.



1)
Execute 
o programa Corrida de Sapo algumas vezes e 
analise os resultados sobre a ordem de chegada dos 
sapos. Obs: compile com a opção 
–
lpthread
*/



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define NUM_THREADS 5
#define PULO_MAXIMO 100
#define DESCANSO_MAXIMO 1
#define DISTANCIA_PARA_CORRER 100

static int classificacao = 1;
static pthread_mutex_t lock;
static char * resp[200];
static int cont = 0;


void * Correr(void * sapo) {
    int pulos = 0;
    int distanciaJaCorrida = 0;
    while (distanciaJaCorrida <= DISTANCIA_PARA_CORRER) {
      int pulo = rand() % PULO_MAXIMO;
      distanciaJaCorrida += pulo;
      pulos++;
      printf("Sapo %d pulou\n ", (int) sapo);
        int descanso = rand() % DESCANSO_MAXIMO; sleep(descanso);
      }
      printf("Sapo %d  chegou na posicaoo %d com %d pulos\n ", (int) sapo, 
        classificacao, pulos);
      cont++;
      classificacao++;
      pthread_exit(NULL);

}



int main() {
    classificacao = 1;
    pthread_t threads[NUM_THREADS];
    int t;
    printf("Corrida iniciada ... \n ");
        for (t = 0; t < NUM_THREADS; t++) 
		pthread_create( & threads[t], NULL, Correr, (void * ) t);

	//main espera outras threads terminarem
        for (t = 0; t < NUM_THREADS; t++) 
		pthread_join(threads[t], NULL);

    printf("\n Corrida terminada!!\n ");
    
    
pthread_exit(NULL);
 }


/*
O 1o exercicio consistia basicamente em analisar o que estava ocorrendo. A conclusão é que as threads estão ocorrendo em
paraleo e portanto, em algumas execuções temos erros por elas estarem atuando em "áreas críticas" ao mesmo tempo.
Assim por exemplo foi observado que as vezes todos os sapos tinham a mesma "classificação". A solução para este problema será
dada no exercício 2.
*/

/*

2)
Usando mutex, modifique o programa Corrida de Sampo 
para que o problema identificado anteriormente não 
ocorra. 

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define NUM_THREADS 5
#define PULO_MAXIMO 100
#define DESCANSO_MAXIMO 1
#define DISTANCIA_PARA_CORRER 100



//pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

//int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
//int pthread_mutex_lock (pthread_mutex_t *mutex);
//int pthread_mutex_unlock (pthread_mutex_t *mutex);
//int pthread_mutex_destroy (pthread_mutex_t *mutex);





static int classificacao = 1;

//mutex pras threads
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static char * resp[200];
static int cont = 0;


void * Correr(void * sapo) {
    int pulos = 0;
    int distanciaJaCorrida = 0;
    while (distanciaJaCorrida <= DISTANCIA_PARA_CORRER) {
      int pulo = rand() % PULO_MAXIMO;
      distanciaJaCorrida += pulo;
      pulos++;
      printf("Sapo %d pulou\n ", (int) sapo);
        int descanso = rand() % DESCANSO_MAXIMO; sleep(descanso);
      }
      pthread_mutex_lock( &lock );  
         printf("Sapo %d  chegou na posicaoo %d com %d pulos\n ", (int) sapo, classificacao, pulos);
         cont++;
         classificacao++;
      pthread_mutex_unlock( &lock );  
      

pthread_exit(NULL);

}



int main() {

    classificacao = 1;
    pthread_t threads[NUM_THREADS];
    int t;
    printf("Corrida iniciada ... \n ");
        for (t = 0; t < NUM_THREADS; t++) 
    pthread_create( & threads[t], NULL, Correr, (void * ) t);

  //main espera outras threads terminarem
        for (t = 0; t < NUM_THREADS; t++) 
    pthread_join(threads[t], NULL);

    printf("\n Corrida terminada!!\n ");
    
  //destruindo mutex
    pthread_mutex_destroy(&lock);

pthread_exit(NULL);
 }


/*
Nesse exercicio a proposta era solucionar os problemas do exercício 1. Para isso, criamos um mutex como variavel global inicializando-o
Em seguida, sempre que alguém tenta acessar uma área crítica dou um mutex_lock e ao sair da área crítica um mutex_unlock liberando
a área pra próximos acessos. Dessa forma, embora as threads estejam concorrendo em pararelo, ao chegarem em uma "região crítica" 
a que chegar primeiro "prende" e faz com que as próximas que cheguem fiquem esperando a região ser liberada, garantindo assim
a execução correta da corrida dos sapos :)
*/


/*

3) Usando threads, escreva um programa C que implemente o 
problema do produtor/consumidor. O produtor deve produzir 
dados (números inteiros pseudo
-
aleatórios) a cada 1 segundo 
colocando
-
os em uma fila (buffer, implementação circular). O 
consumidor deve retirar dados da fila a cada 2 segundos. O 
tamanho máximo da fila deve ser de 8 elementos (MAXFILA) e 
tanto o produtor como o consumidor devem produzir/consumir 
64 elementos (números inteiros de 1 a 64, por exemplo) 
evitando condições de corrida. Variáveis compartilhadas entre 
threads são simplesmente variáveis globais
.

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


//mutex para lockar producao/consumo
static pthread_mutex_t lock;

//mutex para lockar bufferMaximo.
static pthread_mutex_t lockBuffer;


//iniciando mutex de lock de MaxBuffer
static pthread_cond_t can_produce =  PTHREAD_COND_INITIALIZER;
  
//iniciando mutex de lock de MinBuffer
static pthread_cond_t can_consume =  PTHREAD_COND_INITIALIZER;

void insere(int elemento)
{
  int fim;
  if(numElementos ==8)
{
    printf("Fila cheia -- Elemento %d não inserido\n", elemento);
    pthread_cond_wait(&can_produce, &lockBuffer);
    
}

    //entrando em região critica (insere)
    pthread_mutex_lock( &lock ); 
    //nao esta cheia, vamos inserir elemento
    fim = (ini + numElementos) % NUM_MAXIMO;
    vetor[fim] = elemento;
    numElementos++;
    
    //ja produziu posso liberar pro consumo
    pthread_cond_signal (&can_consume);

    printf("Produzido %d\n", elemento);

    //saindo de região crítica
    pthread_mutex_unlock( &lock );
}

int removeElem()
{ 
  int valor;

  if(numElementos == 0)
{
    printf("Fila vazia\n");
    //se ta vazia espero pra remover
    pthread_cond_wait(&can_consume, &lockBuffer);
}
  
    valor = vetor[ini];
    ini = (ini + 1) % NUM_MAXIMO;
    numElementos--;
    printf("Consumido %d\n", valor);
    pthread_cond_signal (&can_produce);
    return valor;
    
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
      //locando regiao critica (remove)
      pthread_mutex_lock( &lock ); 
      int valor = removeElem();
      //unlockando região critica (remove)
      pthread_mutex_unlock( &lock ); 
    } 
}

int main ()
{

   //iniciando mutexs
    pthread_mutex_init(&lock,NULL);
      pthread_mutex_init(&lockBuffer,NULL);


  //criando as threads.
  pthread_t threadCres1;
  pthread_t threadDecres1;

  pthread_create(&threadCres1, NULL, produz, NULL);
  pthread_create(&threadDecres1, NULL, consome, NULL);


  //a thread invocadora é bloqueada até que cada uma das 2 threads termine.
  //o valor de retorno da thread executando o exit(0) será copiada para o segundo argumento
  //no caso NULL (nao retorna nada)
  pthread_join(threadCres1, NULL);
  pthread_join(threadDecres1, NULL);


   //destruindo mutex
   pthread_mutex_destroy(&lock);



}

/*
Nesse exercício foi usado basicamente o mesmo código do lab anterior. A diferença é que agora vamos tratar as áreas críticas com mutex.
Assim, iniciamos o mutex no começo da main. Criamos as threads e falamos pra main esperar até o fim de suas execuções.

O Mutex lock é responsável pelo controle das regiões críticas, no caso ao inserir e ao retirar.
O Mutex lockBuffer é usado junto com as variáveis de condição para impedirmos a produção quando a pilha estiver cheia
e impedirmos o consumo quando a pilha tiver vazia.  

can_produce e can_consume são variaveis de condição usadas para dizer se o processo está liberado para produção e/ou liberado
para consumo. Dessa forma, garantimos que se a fila tiver cheia não há produção e se a fila estiver vazia não há consumo.

Importante observar também que:
pthread_cond_signal (&can_consume) por exemplo faz com que seja mandado um sinal avisando que já foi produzido algo, portanto pode consumir.
Assim onde tinha ficado parado em pthread_cond_wait(&can_consume, &lockBuffer); pois estava vazio e devia esperar uma produção, agora,
após a produção já pode inserir.


Ao fim da main, destruímos os mutex utilizados.

*/