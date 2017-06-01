#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 
int main (){
	
	float result = 2;
	//simula entrada de io	
	kill(getppid(),SIGUSR1);
	//simula saida de IO	
	kill(getppid(),SIGUSR2);
	
for(int i=0;i<100;i++)
 for( int j=0;j<1000;j++)
	result += 0.001;	

	
		return 0;
}		 
