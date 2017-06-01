#include <stdio.h>
#include <stdlib.h>

int main (){
	
	float result = 2;

	FILE *file = fopen("file1.txt","w");

	if(file == NULL)
	{
		printf("ERRO AO LER ARQUIVO FILE7.TXT");
		exit(1);
	}
	for(int i=0;i<100;i++)
		for( int j=0;j<1000;j++)
			for(int k=0;k<500;k++)
				fprintf(file, "%f ", result);

	
	fclose(file);		

		return 0;
}		 
