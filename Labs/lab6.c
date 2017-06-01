1) Execute o programa dado (exemplo de uso de semáforos) e verifique / explique sua execução.


R:
Ao começar a execução passando como parametro por exemplo  & (-1), o semáforo ainda não existe e portanto entra no while printando .....

Um segundo programa ao começar a execução recebendo 1 por exemplo cria o semaforo e inicializa o seu valor. A partir desse momento o 1o programa sai do do seu while pois o semáforo foi criado.
Os dois programas "correm" para ver quem chega no for primeiro.

Ambos entram no for, e o primeiro a chegar na operação P da um "wait" para todos que estiverem operando com aquele semáforo.

Nesse momento, o 1o a chegar (suponhamos que seja o 1o programa) executa 0o e depois executa a operação V, liberando o semáforo.
O que chegou em 2o quando tentou executar a operação P percebeu que já tinha alguém com posse do semáforo e ele fica esperando até o semáforo ser liberado.
Quando o 1o executou a operação V liberou o semáforo, e dessa forma o 2o pode acessar. Então executou Xx e ai liberou o semáforo novamente. O 1o estava esperando e só após o 2o liberar o semáforo ele consegue liberar novamente a operação P para imprimir Oo e assim por diante.

2) Produtor-Consumidor

Escreva um programa formado por dois 
processos concorrentes, leitor e impressor, que 
compartilham uma área de trabalho (memória) 
executando um loop infinito. Para sincronizar as 
suas ações, eles fazem uso de semáforos. 

O processo leitor fica lendo caracteres da entrada 
padrão e colocando em um buffer de 16 
posições. Quando o buffer está cheio o processo 
impressor deve imprimi-lo na saída padrão.


#include<sys/shm.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


#include <sys/sem.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h>

  union
semun {
  int
  val;
  struct
  semid_ds * buf;
  ushort *
    array;
};
// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);


int main ( int argc, char*argv[])
{
	int segmento, id, pid, status;
	char *vetorChar;
	int semId;
	
	//semaforo sendo criado
	semId = semget(8753, 1, 0666 | IPC_CREAT);
	//inicializa valor do semaforo
	setSemValue(semId);

	// aloca a memória compartilhada
	segmento = shmget (8754, 5*sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	// associa a memória compartilhada ao processo
	vetorChar = (char*) shmat (segmento, 0, 0);

	// comparar o retorno com -1
	if ((id = fork()) < 0)
	{
		puts ("Erro na criação do novo processo");
		exit(-2);
	}
	else
	if(id == 0)
	{
		//filho
		while(1)
		{
			semaforoP(semId);
			printf("Digite 5 letras\n");
			for(int i=0;i<5;i++)
			{
				scanf(" %c", &vetorChar[i]);
			}
			semaforoV(semId);
			
			
		}
	}
	else
	{	
		sleep(2);

		while(1)
		{
			semaforoP(semId);
			printf("Armazenado no buffer %s\n", vetorChar);
			semaforoV(semId);
		}

		printf("Processo pai = \n");
	}

	// libera a memória compartilhada do processo
	shmdt (vetorChar);
	// libera a memória compartilhada
	shmctl (segmento, IPC_RMID, 0);
	
	return 0;
}


//funcoes auxiliares pro semaforo
int
setSemValue(int semId) {
  union
  semun semUnion;
  semUnion.val = 1;
  return
  semctl(semId, 0, SETVAL, semUnion);
}
void delSemValue(int semId) {
  union
  semun semUnion;
  semctl(semId, 0, IPC_RMID, semUnion);
}
int semaforoP(int semId) {
  struct
  sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = -1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, & semB, 1);
  return
  0;
}
int semaforoV(int semId) {
  struct
  sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = 1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, & semB, 1);
  return
  0;
}



R:

O código começa com o semáforo sendo criado e em seguida alocamos a memoria compartilhada.
Criamos um filho e quando chegamos na execução do pai, damos apenas um sleep para o filho poder ser executado primeiro.

Nesse momento, o filho que chega primeiro e usa semaforoP. Assim quando o pai tenta acessar semaforoP fica em processo de espera.

O filho fica esperando pelo scanf das entradas. Após 5 entradas (No enunciado pede 18, mas para testes fiz apenas para 5, basta trocar 5 por 18 e aumentar o tamanho do vetor) o filho chama SemaforoV, permitindo ao pai acessar seu trecho de código e ele pode então printar o que estava armazenado. 

Agora, o pai usando SemaforoV libera o filho que estava esperando pelo SemaforoP. O processo se repete.


3) Faça programas para alterar um valor de 
uma variável na memória compartilhada.
Um programa soma 1 à variável e o outro 
soma 5 à variável. Utilize semáforos para 
alterar a variável (região crítica)

Programa 1

#include<sys/shm.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


#include <sys/sem.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h>

  union
semun {
  int
  val;
  struct
  semid_ds * buf;
  ushort *
    array;
};
// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);


int main ( int argc, char*argv[])
{
	int segmento, id, pid, status;
	int *valor;
	int semId;
	
	//semaforo sendo criado
	semId = semget(8780, 1,  0666 | IPC_CREAT);
	//inicializa valor do semaforo\n
	setSemValue(semId);

	// aloca a memória compartilhada
	segmento = shmget (8781, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	// associa a memória compartilhada ao processo
	valor = (int*) shmat (segmento, 0, 0);
  *valor = 0;
	 

	while(1)
  {  
   semaforoP(semId);
   sleep(2);
   *valor+=1;
   printf("Valor: %d\n", *valor);
   semaforoV(semId);
  }


	// libera a memória compartilhada do processo
	shmdt (valor);
	// libera a memória compartilhada
	shmctl (segmento, IPC_RMID, 0);
	
	return 0;
}


//funcoes auxiliares pro semaforo
int
setSemValue(int semId) {
  union
  semun semUnion;
  semUnion.val = 1;
  return
  semctl(semId, 0, SETVAL, semUnion);
}
void delSemValue(int semId) {
  union
  semun semUnion;
  semctl(semId, 0, IPC_RMID, semUnion);
}
int semaforoP(int semId) {
  struct
  sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = -1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, & semB, 1);
  return
  0;
}
int semaforoV(int semId) {
  struct
  sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = 1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, & semB, 1);
  return
  0;
}




Programa 2


#include<sys/shm.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


#include <sys/sem.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h>

  union
semun {
  int
  val;
  struct
  semid_ds * buf;
  ushort *
    array;
};
// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);


int main ( int argc, char*argv[])
{
	int segmento, id, pid, status;
	int *valor;
	int semId;
	
	//semaforo sendo criado
	semId = semget(8780, 1, 0666);
	//inicializa valor do semaforo\n
	//setSemValue(semId);

	// aloca a memória compartilhada
	segmento = shmget (8781, sizeof(int), 0666);

	// associa a memória compartilhada ao processo
	valor = (int*) shmat (segmento, 0, 0);

	while(1)
  {  
	 semaforoP(semId);
	 sleep(2);
	 *valor+=5;
	 printf("Valor: %d\n", *valor);
	 semaforoV(semId);
  }
		

	// libera a memória compartilhada do processo
	shmdt (valor);
	// libera a memória compartilhada
	shmctl (segmento, IPC_RMID, 0);
	
	return 0;
}


//funcoes auxiliares pro semaforo
int
setSemValue(int semId) {
  union
  semun semUnion;
  semUnion.val = 1;
  return
  semctl(semId, 0, SETVAL, semUnion);
}
void delSemValue(int semId) {
  union
  semun semUnion;
  semctl(semId, 0, IPC_RMID, semUnion);
}
int semaforoP(int semId) {
  struct
  sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = -1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, & semB, 1);
  return
  0;
}
int semaforoV(int semId) {
  struct
  sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = 1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, & semB, 1);
  return
  0;
}


R: São dois programas que executam simultamente. Como eles acessam o mesmo id de endereço de memória compartilhada eles conseguem se comunicar. Os semáforos fazem com o que seja somado 1 e depois 5, 1 e depois 5, isso é garantido pois temos também um COMPARTILHAMENTO DE SEMÁFOROS feitos pelos ids. Dessa forma, os programas conseguem atraves de semafaroP fazer com o que quem tente acessar essa parte de código entre em modo espero e semaforoV que faz com que o trecho de código seja liberado. Assim, garantimos uma execução alternada.

Desafio:
4) Faça programas que utilizam a memória 
compartilhada para trocar mensagens. Utilize 
semáforos para sincronizar a aplicação

Programa 1 
#include<sys/shm.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


#include <sys/sem.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h>

  union
semun {
  int
  val;
  struct
  semid_ds * buf;
  ushort *
    array;
};
// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);


int main ( int argc, char*argv[])
{
  int segmento, id, pid, status;
  char *msg;
  int semId;
  
  //semaforo sendo criado
  semId = semget(8774, 1,  0666 | IPC_CREAT);
  //inicializa valor do semaforo\n
  setSemValue(semId);

  // aloca a memória compartilhada
  segmento = shmget (8775, 40*sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

  // associa a memória compartilhada ao processo
  msg = (char*) shmat (segmento, 0, 0);

 char *msgtemp = msg;
  
  while(1)
  {

   semaforoP(semId);
   printf(" %s\n", msg);
   //printf("Digite um mensagem: \n");
   scanf(" %s", msg);
   msg = msgtemp;
   semaforoV(semId);
 }

  // libera a memória compartilhada do processo
  shmdt (msg);
  // libera a memória compartilhada
  shmctl (segmento, IPC_RMID, 0);
  
  return 0;
}


//funcoes auxiliares pro semaforo
int
setSemValue(int semId) {
  union
  semun semUnion;
  semUnion.val = 1;
  return
  semctl(semId, 0, SETVAL, semUnion);
}
void delSemValue(int semId) {
  union
  semun semUnion;
  semctl(semId, 0, IPC_RMID, semUnion);
}
int semaforoP(int semId) {
  struct
  sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = -1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, & semB, 1);
  return
  0;
}
int semaforoV(int semId) {
  struct
  sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = 1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, & semB, 1);
  return
  0;
}



Program 2 

#include<sys/shm.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


#include <sys/sem.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h>

  union
semun {
  int
  val;
  struct
  semid_ds * buf;
  ushort *
    array;
};
// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);


int main ( int argc, char*argv[])
{
  int segmento, id, pid, status;
  char *msg;
  int semId;
  
  //semaforo sendo criado
  semId = semget(8774, 1, 0666);
  //inicializa valor do semaforo\n

  // aloca a memória compartilhada
  segmento = shmget (8775, 40*sizeof(char), 0666);

  // associa a memória compartilhada ao processo
  msg = (char*) shmat (segmento, 0, 0);


   char *msgtemp = msg;
  
 while(1)
 {

   semaforoP(semId);
   printf(" %s\n", msg);
   //printf("Digite um mensagem: \n");
   scanf(" %s", msg);
   msg = msgtemp;
   semaforoV(semId);
 }

    

  // libera a memória compartilhada do processo
  shmdt (msg);
  // libera a memória compartilhada
  shmctl (segmento, IPC_RMID, 0);
  
  return 0;
}


//funcoes auxiliares pro semaforo
int
setSemValue(int semId) {
  union
  semun semUnion;
  semUnion.val = 1;
  return
  semctl(semId, 0, SETVAL, semUnion);
}
void delSemValue(int semId) {
  union
  semun semUnion;
  semctl(semId, 0, IPC_RMID, semUnion);
}
int semaforoP(int semId) {
  struct
  sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = -1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, & semB, 1);
  return
  0;
}
int semaforoV(int semId) {
  struct
  sembuf semB;
  semB.sem_num = 0;
  semB.sem_op = 1;
  semB.sem_flg = SEM_UNDO;
  semop(semId, & semB, 1);
  return
  0;
}

R:
O desafio foi basicamente uma junção da questão 2 e 3. Usei memória compartilhada para podermos compartilhar as mensagens, assim como um semáforo compartilhado, tudo feito através de Ids. Assim, eu garantia que um programa só podia mandar uma nova mensagem após o outro programa escrevesse algo. Criei também uma variável "msgTemp" para poder apagar o que estava na memória compartilhada pois não seria mais útil e precisava de mais espaço para novas mensagens.
