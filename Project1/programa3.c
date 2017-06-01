#include <stdio.h>
#include <stdlib.h>

int main (){
	
	float result = 2;

	FILE *file = fopen("file7.txt","w");

	if(file == NULL)
	{
		printf("ERRO AO LER ARQUIVO FILE7.TXT");
		exit(1);
	}
	for(int i=0;i<100;i++)
		for( int j=0;j<100;j++)
			for(int k=0;k<1000;k++)
			{
				result+=0.02;
				fprintf(file, "%f ", result);
			}
	
	fclose(file);		

		return 0;
}		 
