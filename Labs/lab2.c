
/*
Matheus Caldas
1312760
3WB
*/

//Ex 1

/*
Enunciado: 

Elaborar um programa que leia os elementos de duas matrizes 3 X 3 de um arquivo

texto contendo: número da linha, número da coluna, valor,

e que realize a soma das matrizes lidas da seguinte forma:

-A soma dos elementos deve ser feita por, no mínimo, 3 processos

-A comunicação entre os processos deve ser feita via arquivo

-Os arquivos são compartilhados entre pai e filho

*/  

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
		pid_t myPid1;
		myPid1 = fork();

		int row1[3];
		int row2[3];
		int row3[3];


		FILE *file1;
		FILE *file2;
		FILE *resultFile;
		
		
	  	file1 = fopen ("matrix1.txt","r");
		file2 = fopen ("matrix2.txt","r");
		resultFile = fopen ("resultFile.txt","a");
		int row, column, value;
		
		if(myPid1 != 0)
		{
			//PAI
			wait(0);
			pid_t myPid2;
			myPid2 = fork();


			if(myPid2 !=0)
			{
				wait(0);
				pid_t myPid3;
				myPid3 = fork();
				
				if(myPid3 != 0)
				{
					wait(0);

					printf("resultado final->");
					int value1,value2,value3;
					int i=1;
					resultFile = fopen("resultFile.txt", "r");
					printf("\n Resultado da Matriz \n");
					while (fscanf(resultFile, "%d %d %d", &value1,&value2,&value3)!=EOF)
					{
						printf("%d %d %d\n",value1, value2,value3);
					}		
					//esperando 3o irmao
				}
				else
				{
					file1 = fopen ("matrix1.txt","r");
					file2 = fopen ("matrix2.txt","r");
					while (fscanf(file1, "%d %d %d", &row,&column,&value)!=EOF)
					{
						if(row == 3)
						{	
							//printf("%d %d %d\n", row, column,value);
							row3[column-1] = value;
						}
					}

					while (fscanf(file2, "%d %d %d", &row,&column,&value)!=EOF)
					{
						if(row == 3)
						{	
							//printf("%d %d %d\n", row, column,value);
							row3[column-1] += value;
						}
					}

				//printf linha 3 no arquivo 3
				//printf("linha 3-> %d %d %d\n", row3[0],row3[1],row3[2]);
				fprintf(resultFile, "%d %d %d\n", row3[0],row3[1],row3[2]);
				}

			}

			else
			{
				//Filho 2 escreve linha 2
			  	 	while (fscanf(file1, "%d %d %d", &row,&column,&value)!=EOF)
					{
						if(row == 2)
						{	
							//printf("%d %d %d\n", row, column,value);
							row2[column-1] = value;
						}
					}
					while (fscanf(file2, "%d %d %d", &row,&column,&value)!=EOF)
					{
						if(row == 2)
						{	
							//printf("%d %d %d\n", row, column,value);
							row2[column-1] += value;
						}
					}
			
				//printf("linha 2-> %d %d %d\n", row2[0],row2[1],row2[2]);
				fprintf(resultFile, "%d %d %d\n", row2[0],row2[1],row2[2]);
				//printf linha 2 no arquivo 3

			}

		}

		else
		{
		
				//Filho 1 escreve linha 1

			  	 	while (fscanf(file1, "%d %d %d", &row,&column,&value)!=EOF)
					{
						if(row == 1)
						{	
							//printf("%d %d %d\n", row, column,value);
							row1[column-1] = value;
						}
					}
					while (fscanf(file2, "%d %d %d", &row,&column,&value)!=EOF)
					{
						if(row == 1)
						{	
							//printf("%d %d %d\n", row, column,value);
							row1[column-1] += value;
						}
					}
				
				//printf("linha 1-> %d %d %d\n", row1[0],row1[1],row1[2]);
				fprintf(resultFile, "%d %d %d\n", row1[0],row1[1],row1[2]);
				//printf linha 1 no arquivo 3
	
		}

		

	

}


/*
Resultado da execucao foi:

resultado final->
 Resultado da Matriz 
4 5 4
2 5 7
4 2 1


O primeiro exercício consistia em ler dados de arquivo para realizar a soma de matrizes. O objetivo era dividir o programa em pequenas tarefas, cada um feita por um "filho". No meu código fiz que cada filho era responsável por somar uma linha da matriz. Ao final o pai apenas imprimia a soma que já havia sido feita. Vale ressaltar que como solicitado a forma possível com que pais e os outros filhos "continuassem" o trabalho um do outro era por meio do arquivo. Afinal eles não compartilham a mesma memória. Com um arquivo, todos podiam acessar e realizar a tarefa.

A cada fork um novo filho era criado e foi possível entender que os filhos criados eram realmente novos programas, não guardando as mesmas variáveis por exemplo, afinal estavam em posições de memória diferente. Cada filho imprimia em um arquivo a sua parte do trabalho. Assim por exemplo o 1o filho somava e guardava a soma da linha 1. E assim por diante.

Ao final o pai apenas lia desse arquivo a soma e imprimia.


Matrizes utilizadas: 

1 1 0
1 2 2
1 3 2
2 1 1
2 2 1
2 3 4
3 1 2
3 2 1
3 3 0

1 1 4
1 2 3
1 3 2
2 1 1
2 2 4
2 3 3
3 1 2
3 2 1
3 3 1



*/ 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


//Ex 2

/*
Enunciado: 

2) Elaborar um programa que procure as seguintes palavras em um arquivo texto

contendo o poema “Irene no céu” de Manoel Bandeira:

“Irene”, “ceu”, “humor”, “Pedro”, “boa”.

Cada trabalhador buscará uma palavra e indicará em um arquivo a sua palavra e o

número de ocorrências. Ao final o programa deverá listar as palavras e o numero de

ocorrências de cada uma.

*/  
int main()
{
		pid_t myPid1;
		myPid1 = fork();

		FILE *poema;
		FILE *resultFile2;
		
		
	  	poema = fopen ("poema.txt","r");
		resultFile2 = fopen ("resultFile2.txt","a");
		
		if(myPid1 != 0)
		{
			//PAI
			wait(0);
			pid_t myPid2;
			myPid2 = fork();


			if(myPid2 !=0)
			{
				wait(0);
				pid_t myPid3;
				myPid3 = fork();
				
				if(myPid3 != 0)
				{
					wait(0);
					pid_t myPid4;
					myPid4 = fork();

					
					if(myPid4 !=0)
					{	wait(0);
						
						pid_t myPid5;
						myPid5 = fork();

						if(myPid5 !=0)

						{
							wait(0);
							printf("Cheguei ao fim, vamos ler do arquivo criado..\n");
							resultFile2 = fopen ("resultFile2.txt","r");
							char word[200];
							int count;
							while (fscanf(resultFile2, "%s %d", word, &count)!=EOF)
							{
					
								printf("Palavra: %s - %d\n", word, count);
							}
						}
						else
						{
							//filho 5
							char word[200];
							int boaCount = 0;
				
							poema = fopen ("poema.txt","r");
							while (fscanf(poema, "%s", word)!=EOF)
							{
					
								if(strcmp(word,"boa")==0)
									boaCount++;
							}
							//printf("\n boa %d vezes\n", boaCount);
							fprintf(resultFile2, "%s %d \n", "boa", boaCount);
						}
						
					}	
					else
					{
						//filho 4
						char word[200];
						int ceuCount = 0;
				
						poema = fopen ("poema.txt","r");
						while (fscanf(poema, "%s", word)!=EOF)
						{
					
							if(strcmp(word,"ceu")==0)
								ceuCount++;
						}
						//printf("\nceu %d vezes\n", ceuCount);
						fprintf(resultFile2, "%s %d \n", "ceu", ceuCount);
				
					}

					
				}
				else	
				{
					//filho 3
					char word[200];
					int humorCount = 0;
					
					poema = fopen ("poema.txt","r");
					while (fscanf(poema, "%s", word)!=EOF)
					{
						
						if(strcmp(word,"humor")==0)
							humorCount++;
					}
					//printf("\nHumor %d vezes\n", humorCount);
					fprintf(resultFile2, "%s %d \n", "humor", humorCount);
				}

			}

			else
			{
				//filho 2
				char word[200];
				poema = fopen ("poema.txt","r");
				int pedroCount=0;
				while (fscanf(poema, "%s", word)!=EOF)
					{
						if(strcmp(word,"Pedro")==0)
							pedroCount++;
					}
				//printf("\nPedro %d vezes\n", pedroCount);
				fprintf(resultFile2, "%s %d \n", "Pedro", pedroCount);
			}

		}

		else
		{
			//filho 1
			char word[200];
			poema = fopen ("poema.txt","r");
			int ireneCount =0;
			while (fscanf(poema, "%s", word)!=EOF)
			{
				
				if(strcmp(word,"Irene")==0)
					ireneCount++;
			}
			//printf("\nIrene %d vezes\n", ireneCount);
			fprintf(resultFile2, "%s %d \n", "Irene", ireneCount);

		}

}

/*
Resultado da execucao foi:

Cheguei ao fim, vamos ler do arquivo criado..
Palavra: Irene - 5
Palavra: Pedro - 1
Palavra: humor - 1
Palavra: ceu - 1
Palavra: boa - 1


No segundo trabalho a ideia era basicamente a mesma. Mas agora teríamos 5 filhos. Cada um lia o arquivo e identificava a ocorrência de uma certa palavra.

Assim por exemplo o 1o filho lia o arquivo procurando quantas vezes a palavra Irene aparecia.

Ao final escrevia em outro arquivo o nome da palavra procurada (Irene) e o número de ocorrências. Essa escrita em outro arquivo foi a forma encontrada para os filhos e os pais falarem entre si, afinal como já mencionado ao realizar o fork temos novos programa que não compartilham a mesma memória.

Ao final de tudo o pai lia o arquivo final que constava as palavras e o número de ocorrências e apenas imprimia.

Em ambos os exercicios era utilizada o comando fork, em seguida, pela verificação do PID sabia se estava com o controle do pai ou do filho. No caso do pai, dava o comando wait(0) para que ele esperasse o seu filho ser executado. E assim sucessivamente para todos os filhos. Ao final, quando o pai retomava o controle do processo, executa o print.

Uma dificuldade encontrada encontrada neste segundo exercicio foi na acentuação, que foi retirada para evitar problemas de comparação com as palavras. Ao ponto final e : foi adicionado um espaco para as coparacoes de palavras.


Poema utilizado:


Irene no Céu

Irene preta
Irene boa
Irene sempre de bom humor .

Imagino Irene entrando no ceu :
- Licença, meu branco!
E São Pedro bonachão:
- Entra, Irene. Você não precisa pedir licença.


*/






