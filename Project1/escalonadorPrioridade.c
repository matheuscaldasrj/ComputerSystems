#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define NUM_PROGRAMS 7

struct Process {
	char name[20];
	int pid;
	int numberOfExecutions;
	int finished;
	int priority;
	int interruptedByIO;

};
typedef struct Process Process ;


int interpretador(Process processes[]);
int procuraMaiorPrioridade(Process processes[]);
void escalonador(Process processes[]);
int criaProcesso(Process process);
void exibeProntos();
void exibeEspera();

//Criando 7 detalhes de processos globais para handler acessar
Process processes[NUM_PROGRAMS];
int io_flag_in = 0;
int io_flag_out = 0;

void handlerIn(int signal);
void handlerOut(int signal);

int main ()

{
	

	printf("inicio de tudo\n");
	

	//interpreta os comandos e joga na variavel processes que tem informacao de todos os processos
	interpretador(processes);
	
	escalonador(processes);


	printf("\nFim de tudo\n");
	return 0;
}


void escalonador(Process processes[])
{	

	int status;
	int programasFinalizados = 0;

	signal(SIGUSR1 , handlerIn);	

	signal(SIGUSR2 , handlerOut);	
	while(programasFinalizados < NUM_PROGRAMS)
	{
		//continua a escalonar, ainda há programas que não terminaram

		printf("\n\n============================================================================");
		int pos = procuraMaiorPrioridade(processes);

		if(pos != -1)
		{
			//temos que diminuir um nivel de prioridade, o que siginifca adicionar um valor
			processes[pos].priority+=1;

			//achou o processo o proximo a ser executado.
			int pid = processes[pos].pid;
			processes[pos].numberOfExecutions++;
			if(pid == -1)
			{
				//programa ainda nao criado, logo devemos chamar criaProcesso.
				pid = criaProcesso(processes[pos]);
				processes[pos].pid = pid;
			}

			printf("\nExecutando processo: %s - Executado %d vezes", processes[pos].name, processes[pos].numberOfExecutions);

			kill(pid,SIGCONT);
			//espera 2 segundos para verificar se ele terminou,entrou em espera de io
			// ou ainda precisa rodar alguma coisa depois
			
			for(int j=0;j<1000;j++)
			{
				//for apenas pra ele ficar dormindo e acordando e dessa forma conseguir
				//ouvir o handler e poder falar que foi interrompido por IO.

				usleep(2000);
				if(io_flag_in)
				{
					printf("\n %s Foi interrompido por IO, mesmo nao tendo acabado todo\n o tempo que tinha destinado a sua execucao\n", processes[pos].name);
					io_flag_in = 0;
					processes[pos].interruptedByIO = 1;
					//simulacao, vou mandar ele parar de rodar de fato.
					kill(pid,SIGSTOP);
					break;

				}
			}


			int ret = waitpid(pid,&status,WNOHANG);
			//retorno da waitPid com o parametro WNOHANG. Se voltar 0, significa que o processo
			//ainda está rodando
			if(processes[pos].interruptedByIO == 1)
			{
				printf("\nProcesso *****%s***** não foi finalizado pois entrou em IO,\nlogo vai pra fila de espera\n", processes[pos].name);
				
			}

			else
			{
				if(ret == -1)
				{
					printf("\nErro no retorno do waitPid, programa terminou com erro");
					//exit(1);
				}
				else if(ret == 0)
				{
					printf("\nProcesso *****%s***** não foi finalizado. Está na fila mais uma vez.\n", processes[pos].name);
					//processo ainda rodando, vou pausar para verificar novamente quem tem maior prioridade
					kill(pid, SIGSTOP);

				}
				else
				{
					//terminou, vou declarar que ele terminou
					processes[pos].finished = 1;
					printf("\n---Processo *****%s****** terminado---\n", processes[pos].name);
					programasFinalizados++;
				}
			}

			exibeEspera();

			if(io_flag_out)
			{
				processes[pos].interruptedByIO = 0;
				io_flag_out = 0 ;
				printf("\nProcesso *****%s***** acabou IO, vai pra fila de prontos\n", processes[pos].name);
			

			}

			printf("Programas Terminados: %d/%d\n", programasFinalizados, NUM_PROGRAMS);
			printf("============================================================================");


			exibeProntos();
			

		}



	}
	
}



int criaProcesso(Process process)
{
	int pid = fork();
	if(pid == -1)
	{
		printf("Erro ao executar fork para criar processo %s", process.name);
		return -1;
	}
	else if (pid != 0 )
	{
		//pai
		// da stop no programa pois essa funcao apenas cria, quem manda rodar é o escalonador
		kill(pid,SIGSTOP);		
	}
	else
	{
		execl(process.name,"nada", NULL);
		//criou processo
	}
	return pid;
}

void handlerIn(int signal)
{
	//handler indicando que entrou em io
	io_flag_in = 1;

}

void handlerOut(int signal)
{
	//handler indicando que saiu de IO
	io_flag_out = 1;	

}

void exibeProntos(){
	int count=0;

	printf("\nProcessos Prontos na fila para serem executados: \n      Nome           Execucoes   Prioridade\n");
	for(int i=0;i<NUM_PROGRAMS;i++)
	{
		if(processes[i].interruptedByIO == 0 && processes[i].finished == 0)
		{
			//nao foi interrompido e não terminou
			printf("Processo %s   %d vezes  - Prioridade %d \n", processes[i].name, processes[i].numberOfExecutions, processes[i].priority);
			count++;
		}
	}

	if(count == 0 )
	{
		printf("Todos os programas já foram executados\n");
	}

}

void exibeEspera(){
	int count = 0;
	printf("\nProcessos em espera de IO:\n");
	for(int i=0;i<NUM_PROGRAMS;i++)
	{
		if(processes[i].interruptedByIO == 1 && processes[i].finished == 0)
		{
			//nao foi interrompido e não terminou
			printf("Processo %s \n", processes[i].name);
			count++;
		}
	}
	if(count == 0 )
	{
		printf("Nenhum processo em espera de IO\n");
	}
}



/*
Funcao responsavel por procurar quem tem a maior pririodade na lista do que ainda estao rodando.
*/
int procuraMaiorPrioridade(Process processes[])
{
	int pos = -1; //posicao no vetor processes do processo com maior prioridade no momento.
		

	//menor signfica que estou procurando o menor numero, ou seja, o de maior prioridade
	int menor=100;
	int executions = -1;

	for(int i=0;i< NUM_PROGRAMS; i++)
	{
		//se ele ainda nao terminou e se é o de maior prioridade
		if(!processes[i].finished && processes[i].priority <= menor)
		{
			if(processes[i].priority == menor )
			{
				//eles tem a mesma prioridade, mas temos que ver quem executou menos vezes
				if(processes[i].numberOfExecutions < executions)
				{
					//esse processes[i] deve ir primeiro pois executou menos vezes

					executions = processes[i].numberOfExecutions;
					menor = processes[i].priority;
					pos = i;
				}

			}
			else
			{
				//numero da prioridade é menor, entao com certeza tem mais prioridade

				executions = processes[i].numberOfExecutions;
				menor = processes[i].priority;
				pos = i;
			}
		}
	}

	return pos;
}

int interpretador(Process processes[])
{

	char temp[10];char temp2[10];char temp3[10];
	char programName[20];
	char priority[30];
	int count = 0;

	FILE *file = fopen("configPriority.txt","r");

	if(file == NULL)
	{
		printf("Não foi possível ler o arquivo\n");
		exit(1);
	}



	while(fscanf(file,"%s %s %s", temp, programName, temp2) != -1 || count > NUM_PROGRAMS) 
	{
		//will read something like
		// exec program6 prioridade = 2
		fscanf(file, "%s %s", temp3, priority);

		
		Process currentProcess = processes[count];
		
		//building process
		strcpy(currentProcess.name,programName);
		currentProcess.priority =  atoi(priority);
		currentProcess.finished = 0;
		currentProcess.pid = -1;
		currentProcess.interruptedByIO = 0;

		processes[count] = currentProcess;

		count ++;
	}

	return 0;

}