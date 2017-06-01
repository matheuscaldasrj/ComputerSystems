/*
Matheus Caldas
1312760
Turma 3WB

*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
/*


1) Faça um programa para somar matrizes de acordo com o seguinte algoritmo: O primeiro

processo irá criar duas matrizes preenchidas e uma terceira vazia na memória compartilhada.

Para cada linha da matriz solução, o seu programa deverá gerar um processo para o seu cálculo.

Ao final deve ser impresssa a matriz resultante.


*/

int main()
{

		int segmento1, segmento2, segmentoFinal, *matrix1, *matrix2, *matrixFinal, status;
		// aloca a memória compartilhada
		segmento1 = shmget (IPC_PRIVATE, sizeof(int)*9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		// associa a memória compartilhada ao processo
		matrix1 = (int*) shmat (segmento1, 0, 0);
		// comparar o retorno com -1;	
		//Filho 1 escreve linha 1
		if(*matrix1 == -1)
		{
			printf("Erro na alocacao");
			return -1;
		}
		
		//linha 1
		*(matrix1)=0;
		*(matrix1+4)=1;
		*(matrix1+8)=4;

		//linha 2
		*(matrix1+12)=1;
		*(matrix1+16)=2;
		*(matrix1+20)=3;
		
		//linha 3
		*(matrix1+24)=10;
		*(matrix1+28)=2;
		*(matrix1+32)=3;
			
		//matrix 2
		
		// aloca a memória compartilhada
		segmento2 = shmget (IPC_PRIVATE, sizeof(int)*9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		// associa a memória compartilhada ao processo
		matrix2 = (int*) shmat (segmento2, 0, 0);
		if(*matrix2 == -1)
		{
			printf("Erro na alocacao");
			return -1;
		}

		//linha 1
		*(matrix2)=1;
		*(matrix2+4)=2;
		*(matrix2+8)=3;

		//linha 2
		*(matrix2+12)=1;
		*(matrix2+16)=2;
		*(matrix2+20)=3;
		
		//linha 3
		*(matrix2+24)=1;
		*(matrix2+28)=2;
		*(matrix2+32)=6;
			
			
		// aloca a memória compartilhada
		segmentoFinal = shmget (IPC_PRIVATE, sizeof(int)*9, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		// associa a memória compartilhada ao processo
		matrixFinal = (int*) shmat (segmentoFinal, 0, 0);
		if(*matrixFinal == -1)
		{
			printf("Erro na alocacao");
			return -1;
		}

		pid_t myPid1;
		myPid1 = fork();

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
					//resultado final
					printf("Resultado final:\n");
					printf("%d %d %d", *(matrixFinal), *(matrixFinal+4), *(matrixFinal+8));
					printf("\n%d %d %d", *(matrixFinal+12), *(matrixFinal+16), *(matrixFinal+20));
					printf("\n%d %d %d\n", *(matrixFinal+24), *(matrixFinal+28), *(matrixFinal+32));
		
				}
				else
				{
				
					//FIlho responsavel por ler linha 3
					//pos 31
					*(matrixFinal+24) = *(matrix1 + 24) + *(matrix2 + 24);
					//pos 32
					*(matrixFinal+28) = *(matrix1 + 28) + *(matrix2 + 28);
					//pos 33
					*(matrixFinal+32) = *(matrix1 + 32) + *(matrix2 + 32);

				}

			}

			else
			{
				//FIlho responsavel por ler linha 2
				//pos 21
				*(matrixFinal+12) = *(matrix1 + 12) + *(matrix2 + 12);
				//pos 22
				*(matrixFinal+16) = *(matrix1 + 16) + *(matrix2 + 16);
				//pos 23
				*(matrixFinal+20) = *(matrix1 + 20) + *(matrix2 + 20);
			}

		}

		else
		{
		
		
			//FIlho responsavel por ler linha 1
			//pos 11
			*matrixFinal = *matrix1 + *matrix2;
			//pos 12
			*(matrixFinal+4) = *(matrix1 + 4) + *(matrix2 + 4);
			//pos 13
			*(matrixFinal+8) = *(matrix1 + 8) + *(matrix2 + 8);

		}
// libera a memória compartilhada do process
shmdt (matrix1);
shmdt (matrix2);
shmdt (matrixFinal);

// libera a memória compartilhada
shmctl (segmento1, IPC_RMID, 0);
shmctl (segmento2, IPC_RMID, 0);
shmctl (segmentoFinal, IPC_RMID, 0);
}


/*


Optei por criar as 3 matrizes no espaço compartilhado para entender melhor como funcionava
e poder repetir o processo de criação mais de uma vez..

Fui dando fork e cada filho ficou responsável pro uma linha da matriz.

Cada filho somava a linha e já guardava o valor de cada posição da linha na outra matriz que estava no espaço de memória compartilhada
Dessa maneira, todos podiam encher essa matrix no lugar compartilhado.

Foi interessante aprender os conceitos básicos de memória compartilhada e como pode ser um recurso poderoso para dividir processos
e ainda assim eles poderem trabalhar juntos sem precisar de um arquivo externo.

Resultado final:
1 3 7
2 4 6
11 4 9


*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
/*


2) Faça um programa que leia uma mensagem e a armazene na memória compartilhada com id

= 8752. Faça um outro programa que utilize o mesmo id (8752) e exiba a mensagem para o

usuário.

*/
int main()
{

		int segmento, status;
		char *mensagem;

		// aloca a memória compartilhada
		segmento = shmget (8752, 30, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		// associa a memória compartilhada ao processo
		mensagem = (char*) shmat (segmento, 0, 0);
		// comparar o retorno com -1;	
		if(*mensagem == -1)
		{
			printf("Erro na alocacao de memoria");
			return -1;
		}

		printf("Digite uma mensagem: ");
		scanf("%s", mensagem);
		printf("\nMensagem Lida\nFim do programa 1\n");

		shmdt (mensagem);
		return 0;


}


/*
Esse exercicio foi interessante para perceber e saber como utilizar o código do
espaço de memoria compartilhada. Assim, no primeiro programa criei um espaço com o código 8752.

Mesmo após um tempo, ao executar outro programa que procurava por esse codigo foi possível encontrar
o valor das variaveis.

No 1o programa uma frase era lida e guardada numa memoria compartilhada. O segundo apenas acessava com 
o código e apenas imprimia.

Vale notar que no primeiro programa se eu passasse IPC_CREAT | IPC_EXCL como argumento no 3o parametro, 
levava um segmentation fault. Lendo a documentação parece ser porque 
" If the segment already exists, the call fails." Então após o primeiro teste, o segmento já exisita e dava erro.

Como resultado final, ficou que o primeiro programa um espaço de 30 bytes na memoria.
Nele mesmo eu mato a relação do processo com a posição de memória mas o segmento ainda está lá.

No segundo programa eu leio desse segmento e depois libero tanto o processo quanto o espaço de memória.
Dessa forma o programa executou sem segmentation fault dando como resultado:

Programa 1:
Digite uma mensagem: Testando

Mensagem Lida
Fim do programa 1

Programa 2:
Mensagem lida do programa 1: Testando



*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main()
{

		int segmento, status;
		char *mensagem;

		// aloca a memória compartilhada, passando o mesmo Id que já tinhamos passado.
		segmento = shmget (8752,30,0);
		// associa a memória compartilhada ao processo( no caso a memoria ja foi modificada);
		mensagem = (char*) shmat (segmento, 0, 0);
		// comparar o retorno com -1;
	
		printf("\n\nMensagem lida do programa 1: %s\n", mensagem);
		// libera a memória compartilhada do process
		shmdt (mensagem);
		shmctl(segmento,IPC_RMID,0);

		return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

/*

3) Faça um programa paralelo para localizar uma chave em um vetor de 100 posições. Crie o

vetor na memória compartilhada com dados numéricos inteiros, desordenados e com

repetições. Divida o espaço de busca e gere processos para procurar a chave na sua área de

busca, informando as posições onde a chave foi localizada. Ao final o programa deve listar as

posições onde a chave foi encontrada.

*/
int main()
{
		
		int segmento,*vetor;
		// aloca a memória compartilhada
		segmento = shmget (IPC_PRIVATE, sizeof(int)*100, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		// associa a memória compartilhada ao processo
		vetor = (int*) shmat (segmento, 0, 0);
		// comparar o retorno com -1;	
		if(*vetor == -1)
		{
			printf("Erro ao alocar memoria compartilhada");
			return -1;
		}

		/*
		segmentoFinal e resultadoFinal são variaveis onde usaremos apenas pra guardar onde todas as posicoes foram encontradas.
		*/
		int segmentoFinal,*resultadoFinal;
		segmentoFinal = shmget (IPC_PRIVATE, sizeof(int)*100, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
		// associa a memória compartilhada ao processo
		resultadoFinal = (int*) shmat (segmentoFinal, 0, 0);
		if(*resultadoFinal == -1)
		{
			printf("Erro ao alocar memória compartilhada");
		}

		int *resultFinalTemp = resultadoFinal;
	
		pid_t myPid;

		int randomNumber;

		int key = 77;

		//gerando numero aleatorio.
		for(int j=0;j<100;j++)
		{
			//numero aleatorio entre 0 e 100;
			randomNumber = rand() % 100;

			//forcando para ter chave em varios lugares do vetor aleatorio.
			if(j==1 || j==18 || j==23 || j==49 || j==89 || j==92)
			{
				vetor[j] = key;
			}
			else
			{
				vetor[j] = randomNumber;
			}
		}

		*vetor = 39;
		for(int i=0;i<10;i++)
		{
			myPid = fork();
			wait(0);
			if(myPid == 0)
			{	
				//contador do numero de chaves. Cada processo tem "direito" a achar no máximo 10 chaves.
				//Assim o vetor Resultado final tem 100 posicoes. Cada filho pode usar até 10.
				int countKey = 0;
				for(int count=i*10;count<(10*i)+10;count++)
				{
					if(vetor[count] == key)
					{
						resultadoFinal[i + countKey] = count;
						countKey++;
					}
				}
				//liberando memoria compartilhada do processo
				shmdt(vetor);
				shmdt(resultadoFinal);
				exit(0);	
						
			}

				
		
		}


		for(int k=0;k<100;k++)
		{
			if(resultadoFinal[k] != 0)
				printf("Chave encontrada na posicao %d\n", resultadoFinal[k]);
		}


		printf("Liberando espacos de memoria....\n");
		//liberando memoria compartilhada.
		shmctl(segmento,IPC_RMID,0);
		shmctl(segmentoFinal,IPC_RMID,0);



}



/*

O resultado obtido foi, como esperado:

Chave encontrada na posicao 1
Chave encontrada na posicao 18
Chave encontrada na posicao 23
Chave encontrada na posicao 49
Chave encontrada na posicao 89
Chave encontrada na posicao 92
Liberando espacos de memoria....




Nesse exercício foi gerado um vetor aleatorio com 100 posições. Escolhi como chave o valor 77 (valor esse a ser procurado)
Criei de início no pai um espaço para memória compartilhada e associei ao processo.
Fiz um for com um fork, fazendo assim a criação de 10 filhos e cada um já podendo acessar essa memória compartilhada.

Então dividi a tarefa de cada filho..O primeiro iria procurar apenas entre a posição 0 e 9 do vetor. O segundo apenas entre 10 e 19, o terceiro
entre 20 e 29 e assim por diante. Sendo portanto cada filho responsável por um pedaço do vetor.

Aprendi conceitos importantes como o fato de ter que dar um shmdt para liberar memoria de cada processo criado.
E fixei os conceitos do que é feito antes do fork permanece com os filhos.
Foi interessante notar que é possível dividir os processos entre diversos filhos, assim problemas maiores 
e mais pesados podem ser feitos em partes.

Uma tarefa bastante interessante foi armazenar as posicoes que eu ia achando num vetor compartilhado por todos. Assim, cada filho tinha direito a achar
até 10 chaves no seu setor. E ia armazenando nesse vetor grande que todos podiam usar.

Ao final foi impresso pelo pai esse vetor grande que dizia todas as posicoes onde a chave foi encontrada.

*/

