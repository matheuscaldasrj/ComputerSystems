/* 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver <port>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>


#define BUFSIZE 1024


 #include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

char result[BUFSIZE];
/*
Struct to know where exactly a string begin and end
in an array of chars.
*/
struct Position
{
  int initial;
  int final;
};

int createDirectory(char *path, char *dirName)
{
  strcat(path,dirName);

  printf("\n\n----------------LETS CREATE DIRECTORY----------------");

  if(mkdir(path,0777) != 0)
  {

    strcpy(result,"\nErro:\n");
    strcat(result,strerror(errno));
    strcat(result," ao criar dir de path");
    strcat(result,path);
  

    printf("Erro:\n%s\n ao criar dir de path %s\n",strerror(errno), path);
    return -1;
  }
  else
  {
    strcpy(result,"\nDiretorio ");
    strcat(result,path);
    strcat(result,"criado com sucesso!");
    
    printf("\nDiretorio \"%s\" criado com sucesso!\n", path);
    return 0;
  }
}

int removeDirectory(char *path, char *dirName)
{
  strcat(path,dirName);
  
  printf("\n\n----------------LETS REMOVE A DIRECTORY----------------");  

  if(rmdir(path) != 0)
  {

    strcpy(result,"\nErro:\n");
    strcat(result,strerror(errno));
    strcat(result," ao tentar deletar dir de path");
    strcat(result,path);
  

    printf("Erro:\n%s ao tentar deletar dir de path %s\n",strerror(errno), path);
    return 0;
  }
  else
  {

    strcpy(result,"\nDiretorio");
    strcat(result,path);
    strcat(result,"removido com sucesso!\n");
    
    printf("\nDiretorio %s removido com sucesso!\n", path);
    return 0;
  }
}

void getAllFileNames(char *allFileNames, struct Position positions[40], char *path, int *count)
{
  
  int currentPosition = 0;
  int i = 0;
  int beginWith = 0;
  DIR *dp;
  struct dirent *ep;     

  printf("\n\n------------LETS GET ALL FILE NAMES ----------------\n");
    allFileNames[0] = '\0';

  dp = opendir (path);
  if (dp != NULL)
  { 
      while (ep = readdir (dp))
      {
       //concatenate strng
        strcat(allFileNames, ep->d_name);
        positions[i].initial = currentPosition;
        currentPosition += strlen(ep->d_name);
        positions[i].final = currentPosition;
        strcat(allFileNames,"\n");
        currentPosition+=1;
        i++;

      }
    *count = i;
  (void) closedir (dp);

  }
    else
  {

    strcpy(result,"\nErro ao tentar exibir arquivos no diretorio de path: ");
    strcat(result,path);
    printf("Erro ao tentar exibir arquivos no diretorio %s\n", path);
  }

  
}

int readFile(char *path, char *payload, int nrBytes, int offset)
{
  char payloadTemp[100];

  printf("\n\n----------------LETS READ----------------");
  int file=0;
    if((file=open(path,O_RDONLY)) < -1)
  {
    printf("Erro na abertura do arquivo");
                return -1;
  }

  //setando offset  
  lseek(file,offset,SEEK_SET);
    
  //lendo do offset nrbytes e guardando em payload
  read(file,payload,nrBytes);

    return 0;    

}



int checkIfUserIsAble(int userId,char *path, int pOwnersCanWrite, int pOthersCanWrite)
{

  FILE *fp;
  char filePath[100];
  
  int owner;
  int ownerCanWrite;
  int othersCanWrite;

  int wasFound = 0;
  int amIOwner = 0;
    printf("\nCheguei aqui\n");


    printf("path: %s\nuserId%d", path,userId);


  fp = fopen("permissions.txt", "ab+");
  //path ownerCanWrite othersCanWrite;
  if(fp != NULL)
  {
    printf("Diferente de null");
  }
  else
  {
    printf("Igual a null\n");
  }
  while(fscanf(fp, "%s %d %d %d", filePath, &owner, &ownerCanWrite, &othersCanWrite ) == 4)
  {
    printf("path: %s\nOwnerCanWrite: %d\notherCanWriter%d", filePath, ownerCanWrite, othersCanWrite);

    if(!strcmp(path,filePath))
    {
      printf("Achou o arquivo nos permissions\n");
      wasFound = 1; 
      if(owner == userId)
      {
        amIOwner = 1;
        if(ownerCanWrite == 1)
        {
          printf("Autorizado a escrever\n");
          return 1;
        }
      }
      else
      {
        //iam not owner
        if(othersCanWrite == 1)
        {
          printf("Autorizado a escrever\n");
          return 1;
        }
      }
      
      printf("Achou o arquivo");
      printf("Usuario nao Autorizado\n");
       return 0;
    }
    
  }

  fclose(fp);

  FILE * fp2;

   fp2 = fopen ("permissions.txt", "ab");
   fprintf(fp2, "%s %d %d %d\n", path, userId, pOwnersCanWrite, pOthersCanWrite);
  
   fclose(fp2);
  

   //file nao achado em permissions
  printf("Errro: (?) %s\n",strerror(errno));

  //usuario que ta criando arquivo
  return 1;
}

/*
This function decides what to do.
it will can the write or read functions
*/

void writeFirstStep(char *whatToDo, char *action)
{
  char *payload;
  char *path;
  char *offset;
  char *nrbytes;
  int res;
  char *userId;
  char *ownersCanWrite;
  char *othersCanWrite;

  int isUserAble;
  userId = strtok(NULL,",");
  payload =  strtok(NULL,",");
  path = strtok(NULL,",");
  offset = strtok(NULL,",");
  nrbytes = strtok(NULL,",");
  ownersCanWrite = strtok(NULL,",");
  othersCanWrite = strtok(NULL,",");

  printf("UserID: %d ", atoi(userId));
  
  isUserAble = checkIfUserIsAble(atoi(userId),path, atoi(ownersCanWrite),atoi(othersCanWrite));

  if(isUserAble == 0)
  {

      printf("Usuario nao autorizado a escrever no arquivo\n");
     strcpy(result,"Usuario nao autorizado a escrever no arquivo ");
     strcat(result, path);
     return ;
  }
  res = writeFile(path,payload, atoi(nrbytes), atoi(offset));
  if(res == 0) 
  {
       
     strcpy(result,"\nFoi escrito ");
    strcat(result,payload);
    strcat(result," no path ");
    strcat(result,path);

    printf("Foi escrito '%s' no arquivo %s\n", payload, path);
  }
  else if(res == -1)
  {

    
    strcpy(result,"\nErro ao tentar escrever ");
    strcat(result,payload);
    strcat(result," no path ");
    strcat(result,path);

    printf("Erro ao tentar escrever %s no arquivo %s", payload, path);
    
  } 
  else 
  {

    strcpy(result,"\n Arquivo de path ");
    strcat(result,path);
    strcat(result," foi removido do sistema");

    printf("Arquivo de path %s foi removido do sistema", path);
  }

  //Para escrever e ler
  //1-
  //write path(file) 10 20
  //2-
  //read  path(file) 10 20

}

void readFirstStep(char *whatToDo, char *action)
{
  char payload[100];
  char *path;
  char *offset;
  char *nrbytes;
  int res;

  path = strtok(NULL,",");
  offset = strtok(NULL,",");
  nrbytes = strtok(NULL,",");
  

  //caso read
  res = readFile(path, payload, atoi(nrbytes), atoi(offset));
  if(res != -1) 
  {
    strcpy(result,"Foi lido ");
    strcat(result,payload);
    strcat(result," no path ");
    strcat(result,path);
    
    printf("\nFoi lido '%s' do arquivo %s\n", payload, path);
  }
  else
  {
    strcpy(result,"\nErro ao tentar ler' ");
    strcat(result,payload);
    strcat(result," no path ");
    strcat(result,path);


    printf("Erro ao tentar ler %s do arquivo %s", payload, path);
  }
  //Para escrever e ler
  //1-
  //write path(file) 10 20
  //2-
  //read  path(file) 10 20

}



void createOrDeleteDir(char *whatToDo, char *action)
{
  char *path;
  char *dirName;
  path = strtok(NULL," ,");
  dirName = strtok(NULL," ,");
  //fugindo do 
  strtok(NULL,"");

  if(!strcmp(action, "createDir"))
  {
    //printf("Caso createDir\n");
    createDirectory(path,dirName);

  }
  else
  {
    //printf("Caso delete dir\n");
    removeDirectory(path,dirName);

  }

  //5-Cria dir
  //createDir path directoryName

  //6-Remove Dir
  //deleteDir path directoryName

}

int getInfoFile(char *path, char *result)
{


  char owner[10];
  char ownerCanWrite;
  int othersCanWrite;
  char filePath[100];
  char sizeChar[2048];
  FILE *fp;

  fp = fopen("permissions.txt", "ab+");
  //path ownerCanWrite othersCanWrite;
  if(fp != NULL)
  {
    printf("Diferente de null");
  }
  else
  {
    printf("Igual a null\n");
  }
  while(fscanf(fp, "%s %s %d %d", filePath, owner, &ownerCanWrite, &othersCanWrite ) == 4)
  {
    printf("path: %s\nOwnerCanWrite: %d\notherCanWriter%d", filePath, ownerCanWrite, othersCanWrite);

    if(!strcmp(path,filePath))
    {
      printf("Usuario owner é:------ %s", owner);
      struct stat st;
      stat(path,&st);
      int size = st.st_size;

      sprintf(sizeChar, "%d", size);
  
      strcpy(result,"Usuario owner é: ");
      strcat(result,owner);
      strcat(result," e tamanho do arquivo é: ");
      strcat(result,sizeChar);
      strcat(result, " bytes\n");
    
      return atoi(owner);

    }
 
}   
printf("Erro: Nao foi achado esse arquivo no permissions");
 
 return -1;
}


void infoOrInfoFile(char *whatToDo, char *action)
{
  char *path;
  int i;
  path = strtok(NULL,",");
  
  struct Position positions[40];
  char allFileNames[1024];
  
  strcpy(result,"All files: \n");

  if(!strcmp(action, "info"))
  {
    
    getAllFileNames(allFileNames, positions, path, &i);
    //now, lets print all file names to check if is working the positions
    for(int j=0;j<i;j++)
    {
        
      int size = positions[j].final - positions[j].initial + 1;
      for(int k=positions[j].initial;k < positions[j].final;k++)
      {

        printf("%c", allFileNames[k]);
        
      }
          printf("\n");
    
    }
    strcpy(result,"All file names: \n");
    strcat(result,allFileNames);

  }
  else
  {
    char ownerId[20];
    int info = getInfoFile(path,result);

    if(info == -1)
    {
      strcpy(result,"Arquivo nao encontrado...sem informacoes disponiveis\n");
      
    }
    else
    {
      printf("Owner id encontrado\n");
      //strcat(result, info);
    }

    printf("Caso de info do File");
  }

  //compara action e chama a respectiva

  //Para informacao
  //3-Do dir
  //info path(dir) (comando ls)
  //4-Do arquivo
  //infoFile ./arquivo (permissao e tamanho)
}


/*
This functions receives whatToDo as an array of chars and calls
the necessary functions to the what it was supposed to be done

*/
int interpreter(char *whatToDo)
{
  char *action;
  int count =0;
  int max;
  int userId;

  //excluindo \n
  char *newLine = strchr(whatToDo,'\n');
  if(newLine)
    *newLine = 0;

  //cleans result;
  memset(result,0,BUFSIZE); 
  strcpy(result,"...\n");

  action = strtok(whatToDo,",");
  if(!strcmp(action, "write") )
  {
    //printf("Caso escrita \n\n");
    writeFirstStep(whatToDo,action);
  }
  else
  if(!strcmp(action, "read"))
  {
    //printf("Caso leitura\n\n");
    readFirstStep(whatToDo,action);
  }
  else
  if(!strcmp(action, "createDir") || !strcmp(action, "deleteDir"))
  {
    //printf("Caso createDir ou DeleteDir, espero ler mais 1 parametro\n");
    createOrDeleteDir(whatToDo, action);
  }
  else
  if(!strcmp(action, "info") || !strcmp(action, "infoFile"))
  {
    //printf("Caso info ou infoFile, espero ler mais 1 parametro\n");
    infoOrInfoFile(whatToDo,action);
  }
  else
  {
    strcpy(result,"****COMANDO NAO E VALIDO******");
    printf("****Comando Inválido****\n");
    return -1;
  }

  //Para escrever e ler
  //1-
  //write blabla path(file) 10 20
  //2-
  //read  blabla path(file) 10 20

  //Para informacao
  //3-Do dir
  //info path(dir) (comando ls)
  //4-Do arquivo
  //infoFile ./arquivo (permissao e tamanho)


  //5-Cria dir
  //createDir pathTODirectory

  //6-Remove Dir
  //deleteDir pathTODirectory

  printf("Final do interpreter\n");
}
int writeFile(char *path, char *payload, int nrBytes, int offset)
{ 
  //TODO
  //check
  //1-If nrBytes = 0, delete File.
  //2-If path doesnt exist yet, create and write normally.

  //opening file
  printf("\n\n--------------------LETS WRITE------------------------\n");

  if(nrBytes == 0)
  {
    printf("NrBytes = 0, arquivo sera excluido\n");
    if(!remove(path))
    {
      printf("arquivo excluido com sucesso\n");
      return -2;
    }
    
    return -1;
  }

  int file=0;
    if((file=open(path,O_RDWR | O_CREAT, 0757)) < -1)
  {
    printf("Erro na abertura do arquivo");
                return -1;
  }

  //setando offset
    lseek(file,offset,SEEK_SET);
    
    //lendo de offset, nrbytes do que está em payload
    write(file,payload,nrBytes);

    return 0;
}


 //end of MyCode

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg); 
  exit(1);
}
int parse (char *buff, int *cmd, char *name) {
    char *cmdstr;
    
    cmdstr = strtok(buff," ");
        name = strtok(NULL,"\0");
    cmd = atoi(cmdstr);
}
int main(int argc, char **argv) {
  int sockfd; /* socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buf */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */
  char name[BUFSIZE];   // name of the file received from client
  int cmd;              // cmd received from client
  char whatToDo[100];    
  /* 
   * check command line arguments 
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /* 
   * socket: create the parent socket 
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
       (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
     sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
  while (1) {

    /*
     * recvfrom: receive a UDP datagram from a client
     */
    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
     (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0)
      error("ERROR in recvfrom");
      
    parse(buf, &cmd, name);
    /* 
     * gethostbyaddr: determine who sent the datagram
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
        sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server received datagram from %s (%s)\n", 
     hostp->h_name, hostaddrp);
    printf("server received %d/%d bytes: %s\n", strlen(buf), n, buf);
    
    //buf2 is just to aux
    interpreter(buf);
    
    /* 
     * sendto: echo the input back to the client 
     */
     printf("FINAL DO SERVIDOR\n");
    n = sendto(sockfd, result, strlen(result), 0, 
         (struct sockaddr *) &clientaddr, clientlen);
    if (n < 0) 
      error("ERROR in sendto");
  }
}
