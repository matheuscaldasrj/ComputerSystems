/*
Rio, 27 de outubro de 2016
Matheus Caldas
1312760
Turma 3WB

1)
Execute e explique o funcionamento do código apresentado nos slides.
*/


#include <stdio.h>
#include <sys/stat.h>
#include  <signal.h>
#include  <fcntl.h> 
#include  <unistd.h> 



#define OPENMODE (O_RDONLY)
#define FIFO "minhaFifo"

int main (void)
{
    int fpFIFO;
    char ch;
    if
    (access(FIFO, F_OK) == -1)
    {
        if
        (mkfifo (FIFO, S_IRUSR | S_IWUSR) != 0)
        {
            fprintf (stderr,
                     "Erro ao criar FIFO %s\n"
                     , FIFO);
            return -1;
        }
    }
    puts ("Abrindo FIFO");
    if
    ((fpFIFO = open (FIFO, OPENMODE)) < 0)
    {
        fprintf (stderr,
                 "Erro ao abrir a FIFO %s\n"
                 , FIFO);
        return -2;
    }
    puts ("Começando a ler...");
    while
    (read (fpFIFO, &ch,sizeof (ch)) > 0)
        putchar (ch);
    puts ("Fim da leitura");
    close (fpFIFO);
    return 0;
}



#include <stdio.h>
#include <sys/stat.h>


#include  <signal.h>
#include  <fcntl.h> 
#include  <unistd.h> 
#include <string.h>

#define OPENMODE (O_WRONLY | O_NONBLOCK)
#define FIFO "minhaFifo"
int main (void)
{
    int fpFIFO;
    char mensagem[] ="Melancia sem caroço";
    if
    (access(FIFO, F_OK) == -1)
    {
        if (mkfifo (FIFO, S_IRUSR | S_IWUSR) != 0)
        {
            fprintf (stderr,"Erro ao criar FIFO %s\n", FIFO);
            return -1;
        }
    }
    puts ("Abrindo FIFO");
    if ((fpFIFO = open (FIFO, OPENMODE)) < 0)
    {
        fprintf (stderr,"Erro ao abrir a FIFO %s\n", FIFO);
        return -2;
    }
    puts ("Começando a escrever...");
    write(fpFIFO, mensagem, strlen(mensagem));
    puts (
        "Fim da escrita"
    );
    close (fpFIFO);
    return 0;
}



/*

Primeiro executamos o leitor, ele cria a FIFO caso ela ainda não exista com mkfifo e na hora de dar open fica no aguardo.

Obs:

****
Sem o non-nlock no open mode, o nosso programa leitor fica esperando na linha  fpFIFO = open (FIFO, OPENMODE) alguém escrever alguma coisa.
Ao mesmo tempo, por outro terminal eu mando o arquivo de escrita escrever uma mensagem. Nesse momento, o de leitura, como agora já tem alguém lá, consegue ler
a escrita e se encerra corretamente.
****

Depois executamos a escrita que abre a FIFO já existente e escreve nela. Como já explicado no obs, nesse momento a leitura que estava aguardamento, le e imprime na tela.

Ficou claro que as flags tem papel decisivo e é importante entender eles bem para conseguir executar corretamente os programas.


*/



/*
2)

Abra duas seções no terminal:

-Na primeira, execute um programa que fica em loop lendo de uma FIFO para depois escrever na saída padrão (tela).
-Na segunda, execute um programa que fica lendo da entrada padrão (teclado) e depois escreve na mesma fifo.
*/



#include <stdio.h>
#include <sys/stat.h>


#include  <signal.h>
#include  <fcntl.h> 
#include  <unistd.h> 
#include <string.h>


#define OPENMODE (O_RDONLY)
#define FIFO "minhaFifo"

int main (void)
{
    int fpFIFO;
    char msgLida;
    char flag[] = "sair";
    if
    (access(FIFO, F_OK) == -1)
    {
        if
        (mkfifo (FIFO, S_IRUSR | S_IWUSR) != 0)
        {
            fprintf (stderr,
                     "Erro ao criar FIFO %s\n"
                     , FIFO);
            return -1;
        }
    }
    puts ("Abrindo FIFO..");
    if
    ((fpFIFO = open (FIFO, OPENMODE)) < 0)
    {
        fprintf (stderr,
                 "Erro ao abrir a FIFO %s\n"
                 , FIFO);
        return -2;
    }
    puts ("Começando a ler...");

    while(1)
{
    while (read (fpFIFO, &msgLida ,sizeof (msgLida)) > 0)
        putchar(msgLida);
}
    puts ("Fim da leitura");
    close (fpFIFO);
    return 0;
}



#include <stdio.h>
#include <sys/stat.h>


#include  <signal.h>
#include  <fcntl.h> 
#include  <unistd.h> 
#include <string.h>

#define OPENMODE (O_WRONLY | O_NONBLOCK)
#define FIFO "minhaFifo"
int main (void)
{
    int fpFIFO;
    char msg;
    if
    (access(FIFO, F_OK) == -1)
    {
        if (mkfifo (FIFO, S_IRUSR | S_IWUSR) != 0)
        {
            fprintf (stderr,"Erro ao criar FIFO %s\n", FIFO);
            return -1;
        }
    }
    puts ("Abrindo FIFO");
    if ((fpFIFO = open (FIFO, OPENMODE)) < 0)
    {
        fprintf (stderr,"Erro ao abrir a FIFO %s\n", FIFO);
        return -2;
    }
    puts ("Digite alguma coisa...");
    while(1)
	{
		scanf("%c", &msg);
		 write(fpFIFO, &msg, 1);
		
	}
   

    close (fpFIFO);
    return 0;
}


/*
A ideia de funcionamento é bastante parecida com o exercício 1, porém nesse exercício não escrevemos simplesmente
uma mensagem "fixa" na FIFO, mas pedimos ao usuário que escreva algo. Dessa forma, utilizei um while(1) para ficar sempre
lendo atráves da funcao scanf. Após a leitura ele já escrevia na FIFO.

Enquanto isso, o programa que lia e exibia também ficava em um while(1) para cada nova palavra no FIFO ele conseguir ler
e escrever na saída do terminal.

Para testar a execução da maneira correta, primeiro executamos o de leitura, para assim como exemplo anterior (devido ao uso
correto das flags) ele ficar esperando até algo ser escrito. Nesse momento executamos o escritor que fica esperando uma entrada
do teclado. Ao digitar algum texto e apertamos ENTER, o texto é escrito na FIFO e o leitor consegue ver que tem algo na FIFO 
e imprime no terminal, possibilitando, portando uma comunicação entre as FIFOS.

*/


/*
Exercício extra

Escreva um programa que primeiro cria uma FIFO e em seguida cria dois processos filhos que escrevem uma String FIFO.
O pai dá um waitpid em seguida lê as strings desse FIFO e exibe na tela.


*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include  <signal.h>
#include  <fcntl.h> 
#include  <unistd.h> 
#include <string.h>
#include <sys/wait.h>

#define FIFO "minhaFifozinha"
#define OPENMODE_READ (O_RDONLY)
#define OPENMODE_WRITE (O_WRONLY | O_NONBLOCK)

int main(){

	pid_t filho1;
	
if(access(FIFO, F_OK) == -1)
	    {//testa se a fifo nao existe ainda

		if (mkfifo (FIFO, S_IRUSR | S_IWUSR) != 0)
		{
		    //erro ao criar fifo
		    fprintf (stderr,"Erro ao criar FIFO %s\n", FIFO);
		    return -1;
		}
	    }
	printf("FIFO CRIADA...\n");


	//criando filho
	 filho1 = fork();
	
	

	if(filho1 != 0)
	{
		printf("Entrou no pai pela 1a vez\n");
			
		//pai, vamos criar o outro filho

		pid_t filho2;
		filho2 = fork();
		
		if(filho2 != 0)
		{
			//abre pra leitura 
			int fpFIFO;
			sleep(1);
			  if
		    	((fpFIFO = open (FIFO, OPENMODE_READ)) < 0)
		    	{
				fprintf (stderr,
					 "Erro ao abrir a FIFO %s\n"
				 	, FIFO);
				return -2;
		   	 }

			wait(0);
			char valorLido;			
			//read			
			//pai de novo, vamos ler o que foi escrito no FIFO
			 puts ("Começando a ler no pai: \n");
			//enquanto tiver alguma coisa vai lendo.
			while(read (fpFIFO, &valorLido,sizeof (valorLido)) > 0)
				putchar (valorLido);
			puts ("\nFim da leitura do pai"); close (fpFIFO);

		}
		else
		{
			//segundo filho
			printf("Entrou no segundo filho\n");
			int fpFIFO;
			puts ("Abrindo FIFO no 2o filho");
			sleep(2);
			if ((fpFIFO = open (FIFO, OPENMODE_WRITE)) < 0)
			   {
				fprintf (stderr,"Erro ao abrir a FIFO %s no segundo filho \n", FIFO);
				return -2;
			    }
			char msg[] = "OI, eu sou o SEGUNDO filho";
			puts ("Começando a escrever no 2o filho...");
			write(fpFIFO, msg, strlen(msg));
	    		puts ("Fim da escrita 2o filho");
				
			}

	}
	
	else
	{	
		//filho 1
		  int fpFIFO;
		  puts ("Abrindo FIFO no 1o filho");
		  sleep(2);
		  if ((fpFIFO = open (FIFO, OPENMODE_WRITE)) < 0)
		    {
			fprintf (stderr,"Erro ao abrir a FIFO %s no primeiro filho \n", FIFO);
			return -2;
		    }
		char msg[] = "OI, eu sou o primeiro filho";
		puts ("Começando a escrever 1o filho...");
		write(fpFIFO, msg, strlen(msg));
    		puts ("Fim da escrita 1o filho");
		
		
	}


	return 0;
}

/*
Como pedido no enunciado foram criados dois filhos através de dois forks.

O pai inicialmente cria a FIFO, depois ele abre a FIFO e da wait(0) para esperar que os filhos escrevam algo. Dessa maneira quando ele tentar ler
algo já terá sido escrito.

Os filhos são bem parecidos, eles abrem a FIFO e escrevem uma mensagem indicando quem são eles.

Quando o controle volta ao pai, ele vai lendo tudo que tá no FIFO que no caso são as duas mensagens dos dois filhos.

Obs: Ao executar inúmeras vezes, algumas (poucas) tentativas davam um erro ao tentar abrir a FIFO. Acredito que o problema tenha sido
o tempo de execução na relação pai/filho onde um filho tentava abrir antes do pai criar etc. 
Ao adicionar sleeps curtos de 1-2segundos esse problema foi resolvido =)


A saida foi:

FIFO CRIADA...
Entrou no pai pela 1a vez
Abrindo FIFO no 1o filho
Entrou no segundo filho
Abrindo FIFO no 2o filho
Começando a escrever 1o filho...
Começando a escrever no 2o filho...
Fim da escrita 2o filho
Fim da escrita 1o filho
Começando a ler no pai: 

OI, eu sou o SEGUNDO filhoOI, eu sou o primeiro filho
Fim da leitura do pai


*/


