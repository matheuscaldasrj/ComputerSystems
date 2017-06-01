/*

Matheus Caldas
1312760
3WB

Faça um programa que crie 2 processos filhos e alterne a execução dos filhos. Após

10 trocas de contexto, o processo pai mata os filhos.

OBS: Os filhos são formados por loops eternos.


*/


#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>


#include <sys/types.h>
#include <sys/wait.h>


int main (void)
{	
	printf("Pid: %d\n", getpid());
	pid_t filho1 = fork();
	
	
	if(filho1 != 0)
	{
		//pai

		pid_t filho2 = fork();
		
		if(filho2 != 0)
		{
			//pai

			for(int i=0;i<10;i++)
			{
				sleep(1);

				kill(filho1, SIGSTOP);

				kill(filho2, SIGCONT);

				kill(filho2, SIGSTOP);

				kill(filho1, SIGCONT);

			}
			
			kill(filho1,SIGKILL);
			kill(filho2,SIGKILL );
			
		}
		else
		{//filho 2
			while(1)
			{
				printf("Filho 2 executando\n");
				sleep(1);
				
			}

		}		
					

	}
	
	else
	{
			//filho 1
			while(1)
			{
				printf("Filho 1 executando\n");
				sleep(1);
				
			}
	}



	return 0;
}


/*
Foram criados dois filhos em loop eterno para ficarem executando..


No pai havia um for para efetuar 10 trocas entre as execuções do pai e do filho. Isso foi feito enviando sinais
atraves da função kill para os filhos em questão, alternando entre os sinais de continuar e de parar.

Encontrei problemas com sincronização, por vezes ele executava vários filhos em sequencia (ao menos era o que estava
aparecendo no print)


Ao final do loop de 10 trocas era enviado o sinal para os filhos serem encerrados.

*/



/*
2) Faça um programa para monitorar e informar o preço de chamadas telefônicas. O

programa deverá ser executado em background.

O início e o término de uma chamada são informados através dos sinais SIGUSR1

e SIGUSR2, respectivamente.

O custo da ligação é de 2 centavos por segundo, para ligações de até 1 minuto e de

1 centavo por segundo, a partir do 2o. minuto, ou seja, uma ligação de 1m30s custa

R$1,50.

*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>


void iniciaLigacao(int sinal);
void terminaLigacao(int sinal);
time_t hora_inicio;
time_t hora_final;

int main (void)
{

signal(SIGUSR1 , iniciaLigacao);	
signal(SIGUSR2	 , terminaLigacao);	

printf("Pid: %d\n", getpid());
	while(1)
{
	
	//declarando sinais
	
}
	return 0;
}

void iniciaLigacao(int sinal)
{

  	time(&hora_inicio);
	printf("Chamada iniciada\n");
}

void terminaLigacao(int sinal)
{
	double duracao;
	float custo = 0;
	time(&hora_final);
	duracao = difftime(hora_final,hora_inicio);
		

	if(duracao < 60)
	{
		custo = duracao*2;
	}
	else
	{
		//primeiro minuto
		custo = 120;
		//a partir do segundo minuto
		custo += (duracao-60);
	}
	
	printf("Chamada terminada. Duracao de %.f segundos\n",duracao );
	printf("O custo foi de R$ %.2f reais\n",custo/100);

	
	exit(0);
}

/*
A função main apenas fica em loop. Isso porque no começo declaramos os handler para dois sinais. Um de inicio de chamada e
outro de término.

Quando era enviada o sinal por outro terminal de inicio, o handler de inicio armazenava a hora.
Quando era enviado o sinal por outro terminal de fim, o handler de saida verificada a diferença de tempo e realiza o cálculo do valor.


Foi interessante aprender os conceitos de delegar handler e de relembrar como se usa a função kill por outro terminal enquanto
o programa em questão está rodando.

Aparentemente a execução está correta.
*/


/*

Faça um programa para controlar os sinais de trânsito de uma via por computador.

O programa deve controlar 4 sinais, o primeiro fica aberto por 30 segundos, em

atenção por 5 segundos e fechado por 30 segundos. O segundo incrementa o tempo

em 10 segundos para aberto, 5 para em atenção e 10 para fechado em relação ao

primeiro sinal. E assim sucessivamente para o terceiro sinal em relação ao segundo

e para o quarto em relação ao terceiro.

Seu programa deve criar processos filho para controlar os sinais / gerar comandos

para as mudanças de sinal, que devem ser indicadas na saída.

1)
Aberto = 30
Atencao = 5
Fechado = 30

2)
Aberto = 40
Atencao = 10 
Fechado = 40

3)
Aberto = 50 
Atencao = 15 
Fechado = 50 

4)
Aberto = 60 
Atencao = 20
Fechado = 60 

*/



#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>


#include <sys/types.h>
#include <sys/wait.h>

void fecha(int sinal);
void abre(int sinal);
void alerta(int sinal);

int cont = 0;
int sinal1 [3] = {0,0,0};
int sinal2  [3] = {0,0,0};
int sinal3 [3] = {0,0,0};
int sinal4 [3] = {0,0,0};

int main (void)
{	

	signal(SIGUSR1 , abre);	
	signal(SIGUSR2	 , fecha);
	signal(SIGPROF, alerta);

	printf("Pid: %d\n", getpid());
	pid_t filho1 = fork();
	
	

	if(filho1 != 0)
	{
		//pai

		pid_t filho2 = fork();
		
		if(filho2 != 0)
		{
			pid_t filho3 = fork();
		
			if(filho3 !=0)
			{
				//pai
				pid_t filho4 = fork();
				
				if(filho4 != 0)
				{
					//pai

						//abrindo sinais inicialmente!

						printf("Sinal 1\n");
						kill(filho1,SIGUSR1);
						
						printf("Sinal 2\n");
						kill(filho2,SIGUSR1);
						
						printf("Sinal 3\n");
						kill(filho3,SIGUSR1);
						
						printf("Sinal 4\n");
						kill(filho4,SIGUSR1);		
						
						sinal1[0]=1;
						sinal2[0]=1;
						sinal3[0]=1;
						sinal4[0]=1;
						
						while(1)
						{
							sleep(1);
							cont++;
							printf("Tempo %d s\n", cont);

							if(cont%65 == 0 && sinal1[2])
							{
								//1 abre
								printf("Sinal 1\n");
								kill(filho1, SIGUSR1);
								sinal1[2] = 0;
								sinal1[0] = 1;

							}
							
							if (cont%30 == 0 && sinal1[0]){
								//1 atencao
								printf("Sinal 1\n");
								kill(filho1, SIGPROF);

								sinal1[0] = 0;
								sinal1[1]=1;
							}
							if(cont%35 == 0 && sinal1[1])
							{
								//1 fecha								
								printf("Sinal 1\n");
								kill(filho1, SIGUSR2);

								sinal1[1] = 0;
								sinal1[2] = 1;
							}
							//SINAL 2							
							if(cont%90 == 0 && sinal2[2])
							{
								//2 abre
								printf("Sinal 2\n");
								kill(filho2, SIGUSR1);
								sinal2[2] = 0;
								sinal2[0]= 1;
							}
							if(cont%40 == 0 && sinal2[0])
							{
								//2 atencao
								printf("Sinal 2\n");
								kill(filho2, SIGPROF);

								sinal2[0] = 0;
								sinal2[1] = 1;

							}
							if(cont%50 == 0 && sinal2[1])
							{
								//2 fecha
								printf("Sinal 2\n");
								kill(filho2, SIGUSR2);

								sinal2[1] = 0;
								sinal2[2] = 1;

							}

							//SINAIS 3
							if(cont%115 == 0 && sinal3[2])
							{
								//3 abre
								printf("Sinal 3\n");
								kill(filho3, SIGUSR1);

								sinal3[2] = 0;
								sinal3[0] = 1;
							}
							if(cont%50 == 0 && sinal3[0])
							{
								//3 alerta
								printf("Sinal 3\n");
								kill(filho3, SIGPROF);

								sinal3[0] = 0;
								sinal3[1] = 1;
						
							}
							if(cont%65 == 0 && sinal3[1]) {
								//3 fecha
								printf("Sinal 3\n");
								kill(filho3, SIGUSR2);

								sinal3[1] = 0;
								sinal3[2] = 1;
							}

							//SINAIS 4
							if(cont%140 == 0 && sinal4[2])
							{
								//4 abre
								printf("Sinal 4\n");
								kill(filho4, SIGUSR1);

								sinal4[2] = 0;
								sinal4[0] = 1;
							}
							if(cont%60 == 0 && sinal4[0])
							{
								//4 alerta
								printf("Sinal 4\n");
								kill(filho4, SIGPROF);

								sinal4[0] = 0;
								sinal4[1] = 1;
							}
							if(cont%80 == 0 && sinal4[1]) {
								//4 fecha
								printf("Sinal 4\n");
								kill(filho4	, SIGUSR2);

								sinal4[1] = 0;
								sinal4[2] = 1;
							}		
						}
				}

				else
				{
					//filho 4
	
					while (1){}
				}
			}

			else
			{
				//filho 3
				while (1){}
				
			}
			
		}
		else
		{//filho 2
			while (1){}

		}		
					

	}
	
	else
	{
			
		while (1){}
		//filho 1
			
	}



	return 0;
}


void fecha(int sinal)
{
	printf("Fechou\n");

}

void abre(int sinal)
{
	printf("Abriu\n");

}


void alerta(int sinal)
{
	printf("Alerta\n");

}
	

/*
Para realizar o exercício criei 4 filhos, um para cada sinal e botei eles para ficar em loop, para continuarem a executar.

No começo do pai deleguei os handler e dessa forma todos os filhos teriam eles.

Assim, no pai executei a cada um segundo uma verificação para ver se os sinais deveriam ser trocados. Caso fosse necessaário ser trocados
era emitido um sinal que exibia a ação do sinal (Abrir/Fechar/Alertar).

Fiz também uma verificação para só exibir que o sinal fechou se o estado anterior fosse em alerta por exemplo. Porque simplesmente
analisando os números, poderiam ter múltiplos comuns e ele poderia fechar sem ter aberto.

Um problema encontrado foi a sincronização. Não entendi muito bem porque mesmo dando um print antes do kill, as vezes o comando do sinal
era exibido antes do print.

O exercicio foi bom para entender melhor os handler e fixar os conceitos de fork/pai/filho e o que herda de quem.

*/

