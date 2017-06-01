/*
INF1019 - SISTEMAS DE COMPUTACAO

Matheus Caldas
1312760
1) Usando processos, escreva um programa C que 
implemente o problema do produtor/consumidor. 
Existem 2 consumidores.  O produtor e os 
consumidores devem compartilhar os recursos do 
processo (filas e demais dados). O produtor deve 
produzir dados (números inteiros) a cada 1 
segundo colocando-os em uma fila (buffer). Os 
consumidores devem retirar dados da fila a cada 2 
segundos. O tamanho máximo da fila deve ser de 
8 elementos (MAXFILA) e tanto o produtor como 
os dois consumidores devem produzir/consumir 
64 elementos
. 

*/


#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGPERM 0600    // msg queue permission
#define MAX_PROD 64

int msgqid, rc;
int done;
int tamFila = 0;

typedef struct msg_buf {
  long mtype;
  int valor;
} msg;


void *produz()
{
    for(int i=0;i<MAX_PROD;i++)
    {

      msg msg1;
      msg1.mtype = 1;
      msg1.valor = i;
      while(tamFila == 8)
      {
        printf("***FILA CHEIA -> ESPERANDO CONSUMO PARA PRODUZIR NOVAMENTE****\n");
        sleep(2);
      }
      printf("Mensagem %d posta na fila pelo produtor - tamanhoFila: %d\n", msg1.valor, tamFila);
      rc = msgsnd(msgqid, &msg1, sizeof(msg1.valor), 0);
      tamFila++;

      sleep(1);
    }
}

void *consome1()
{
  for(int i=0;i<MAX_PROD/2;i++)
    {
        sleep(2);
        msg msgReceived;
        //le a msg da fila, guardando seu valor em msgReceived
        rc = msgrcv(msgqid, &msgReceived, sizeof(msgReceived.valor), 0, 0); 
        printf("Mensagem %d consumida pelo consumidor 1\n", msgReceived.valor);
        tamFila--;
    }

}

void *consome2()
{
  for(int i=0;i<MAX_PROD/2;i++)
    {
        sleep(2);
        msg msgReceived;
        //le a msg da fila, guardando seu valor em msgReceived
        rc = msgrcv(msgqid, &msgReceived, sizeof(msgReceived.valor), 0, 0); 
        printf("Mensagem %d consumida pelo consumidor 2\n", msgReceived.valor);
        tamFila--;
    }

}


int main()
{
  // cria a fila de msgs
  msgqid = msgget(IPC_PRIVATE, MSGPERM|IPC_CREAT|IPC_EXCL);

  pthread_t threadProduz;
  pthread_t threadConsome1;
  pthread_t threadConsome2;

  //cria as thread
  pthread_create(&threadProduz, NULL, produz, NULL);
  pthread_create(&threadConsome1, NULL, consome1, NULL);
  pthread_create(&threadConsome2, NULL, consome2, NULL);

  
  //a thread invocadora é bloqueada até que cada uma das 2 threads termine.
  //o valor de retorno da thread executando o exit(0) será copiada para o segundo argumento
  //no caso NULL (nao retorna nada)
  pthread_join(threadProduz, NULL);
  pthread_join(threadConsome1, NULL);
  pthread_join(threadConsome2, NULL);

  // remove a fila de id msgqid
  rc=msgctl(msgqid,IPC_RMID,NULL);
  printf("Fila de msgs de id %d foi removida\n",msgqid);

  return 0;

}


/*
Para resolucao deste exercício foi utilizada uma biblioteca (#include <sys/msg.h>) de troca de mensagens.
Após pesquisar, descobri as fncoes
msgget que cria uma fila de msg
msgctrl que remove uma fila de msg
msgrcv que recebe uma msg
msgsnd que manda uma msg

O que fiz foi no início criar 3 threads. Uma para produção e duas para consumo. Além disso criei uma fila de msgs.

No produtor, a cada 1seg ele criava uma msg do tipo = 1 (aqui não importa o tipo pois o destino não importa, o consumidor
pegará a próxima msg da fila) e valor da msg como um número inteiro botando essa estrutura na fila. Para o produtor fiz isso 64 vezes
(valor de MAX_PROD)

Ele também verifica se a fila está cheia. Fiz isso utilizando uma variável global, não sei se há maneira mais eficaz
pela própria biblioteca de msg.  (No caso não ficava cheio porque a produção estava junto com o consumo. Ao final de 2seg
2 msgs haviam sido criadas e 2 consumidas.)

No consumidor(ambas as funcoes sao iguais, só mudam o print para indicar consumir correto) eu apenas lia da fila de msg a próxima msg
a ser lida.


Ao final a fila de msg foi removida com o msgctl.


Nota: Não entendi porque ao utilizar uma função consome única passando como parâmetro quem era o consumidor
recebia erro segmentação. Para solucionar o problema, embora com repetição de código, a opção foi criar duas funções
em separado :(

*/


/*
2)
9
Exercícios – Troca de Mensagens
2) Tal como no exercício anterior, implemente a tro
ca 
de mensagens sé que neste exercício as mensagens 
devem ser direcionadas para um dos consumidores. 
Assim, serão geradas 64 mensagens, sendo 32 para 
um dos consumidores e 32 para o outro.

*/

#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


#define MSGPERM 0600    // msg queue permission
#define MAX_PROD 64

int msgqid, rc;
int tamFila = 0;

typedef struct msg_buf {
  long mtype;
  int valor;
} msg;


void *produz()
{
    for(int i=0;i<MAX_PROD;i++)
    {

      msg msg1;
      msg1.mtype = i%2 + 1;
      msg1.valor = i;
      while(tamFila == 8)
      {
        printf("***FILA CHEIA -> ESPERANDO CONSUMO PARA PRODUZIR NOVAMENTE****\n");
        sleep(2);
      }
      printf("Mensagem %d posta na fila pelo produtor para o consumidor %d tamanhoFila: %d\n", msg1.valor, i%2 + 1, tamFila);
      rc = msgsnd(msgqid, &msg1, sizeof(msg1.valor), 0);
      tamFila++;

      sleep(1);
    }
}

void *consome1()
{
  for(int i=0;i<MAX_PROD/2;i++)
    {
        long type = 1;
        sleep(2);
        msg msgReceived;

        //le a msg da fila, guardando seu valor em msgReceived
        rc = msgrcv(msgqid, &msgReceived, sizeof(msgReceived.valor), 1, 0); 
        printf("Mensagem %d consumida pelo consumidor 1\n", msgReceived.valor);
        tamFila--;
    }

}

void *consome2()
{
  for(int i=0;i<MAX_PROD/2;i++)
    {
        long type = 2;
        sleep(2);
        msg msgReceived;
        //le a msg da fila, guardando seu valor em msgReceived
        rc = msgrcv(msgqid, &msgReceived, sizeof(msgReceived.valor), 2, 0); 
        printf("Mensagem %d consumida pelo consumidor 2\n", msgReceived.valor);
        tamFila--;
    }

}


int main()
{
  // cria a fila de msgs
  msgqid = msgget(IPC_PRIVATE, MSGPERM|IPC_CREAT|IPC_EXCL);

  pthread_t threadProduz;
  pthread_t threadConsome1;
  pthread_t threadConsome2;

  //cria as thread
  pthread_create(&threadProduz, NULL, produz, NULL);
  pthread_create(&threadConsome1, NULL, consome1, NULL);
  pthread_create(&threadConsome2, NULL, consome2, NULL);

  
  //a thread invocadora é bloqueada até que cada uma das 2 threads termine.
  //o valor de retorno da thread executando o exit(0) será copiada para o segundo argumento
  //no caso NULL (nao retorna nada)
  pthread_join(threadProduz, NULL);
  pthread_join(threadConsome1, NULL);
  pthread_join(threadConsome2, NULL);

  // remove a fila de id msgqid
  rc=msgctl(msgqid,IPC_RMID,NULL);
  printf("Fila de msgs de id %d foi removida\n",msgqid);

  return 0;

}

/*
Nessa questao a explicação é exatamente igual ao do item anterior.

A grande diferença é que agora as msgs tem um direcionamento específico (devem ser consumidas pelo consumidor 1 OU 2 e não qualquer um)
Para isso, o mType foi de vital importância.
Agora o produtor a cada produção criava uma msg para cada consumidor (mType = 1 ou mType =2 ). Dessa forma ao usar msgrcv no penúltimo
argumento eu identificava qual era o mType, ou seja, indicava qual era o destino.

Assim por exemplo. Se o consumidor 1 tentava receber na fila, ele recebe a próxima msg ENDEREÇADA a ele. Mesmo que tenha msgs pro
consumir 2 na fila o consumidor 1 não consegue pegar pois os mTypes são diferentes.

*/