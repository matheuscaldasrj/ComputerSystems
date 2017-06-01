/*

Matheus Caldas
1312760
3WB

1) Faça um programa para criar dois processos
que se comunicam via pipe. O Pai lê do pipe
enquanto o filho escreve no pipe. Exiba o resultado
do que foi escrito e lido.

*/



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main(){

	int fd[2];
	pid_t pid;


	//criando pipe
	if(pipe(fd) == -1)
	{
		printf("Erro ao criar pipe");
		exit(-1);
	}

	 pid = fork();

	if(pid != 0)
	{
		wait(0);
		//pai le, logo fechamos o 1
		char result[20];
		read(fd[0], result,sizeof(result));
		close(fd[1]);
		printf("String lida no pai: %s\n", result);

	}
	
	else
	{
		
		char texto[20] = "Teste de pipe\n";
		printf("String escrita no filho: %s\n", texto);
		write(fd[1], texto, sizeof(texto));
		close(fd[0]);
		//filho escreve, logo fechamos o 0
	}


	return 0;
}

/*
O programa começa no filho e escreve no pipe um texto. Ele dá close no fd[0] porque nao usamos para leitura

No pai, ele apenas le o que tá no "arquivo" e da close no fd[1] porque não é usada para escrita
*/



/*
Questão 2:

2) Faça um programa para redirecionar a entrada
e a saída, lendo os dados de um arquivo e
gerando a saída em outro.


*/



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	//abrindo apenas pra leitura
	int in  = open("ex2.txt",O_RDONLY, 0666);
	
	if(in==-1)
	{
		printf("Erro ao abrir o arquivo\n");
	}
	
	//apaga registro 0 que é o padrão de leitura
	close(0);
		
	//agora sem entrada padrao, vou setar quem é
	dup(in);	

	//apaga registro 1 padrao de escrita do out
	int out = open("ex2result.txt", O_WRONLY|O_CREAT,0666);
	close(1);
	
	//agora sem saida padrao, vou setar esse arquivo como saida padrao
	dup(out);
	char result[200];
	scanf(" %[^\n] ", result);
	printf("Result: %s\n", result);


	return 0;
}

/*
O programa simplesmente abre um arquivo;
Apagamos o registro padrão de leitura, e setamos quem será o novo (no caso o nosso arquivo ex2)


Depois, apagamos o registro padrão de escrita.
Ao final, sem uma saída padrão, podemos dar dup na nova saida, que será o nosso novo arquivo.

Assim ao efetuar um scanf estamos lendo do arquivo ex2, pois o padrão de leitura virou este arquivo.
E ao efetuar um printf estamos escrevendo no arquivo ex2result, pois o padrão de escrita virou este arquivo.

*/

/*
Questão 3 

3) Faça um programa para criar um pipe e
executar dois processos que são utilitários do Unix
que se comuniquem através do pipe criado (a shell
faz isso)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void)
{
        int     fd[2];
        pid_t   pid;

        pipe(fd);
	
		pid = fork();

        if(pid == 0)
        {
         
         //filho, apaga padrao do pipe
		close(1);
		dup(fd[1]);
		close(fd[0]);		
        execl("/bin/ps","ps","-all",  0);
                
        }
        else
        {
		waitpid(-1,0,0);
		
        close(0);
		dup(fd[0]);
		close(fd[1]);
		execl("/bin/grep", "grep", "firefox", 0);
        }

        return(0);
}

/*
Primeiro criamos o nosso pipe.
Executamos o fork, gerando um filho.

O pai começa a executar e espera seu filho com o waitpid.

Assim o filho apaga escrita padrao do pipe. Agora sem padrão conseguimos fazer dup botando nosso novo padrao.
Damos close para apagar valor de entrada e executamos por exemplo o ps.

Agora quando chegamos no pai, 
Apaga-se a leitura padrão e podemos definir a nova leitura padrao como sendo a do pipe criado.
Com o close matamos a escrita.

Assim, o filho ao executar ps, redirecionamos a saida do pipe pro grep e conseguimos neste exemplo filtrar e observar a execução do firefox.

*/

