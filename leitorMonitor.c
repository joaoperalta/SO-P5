#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_TAREFA 3 /* NUMERO MAXIMO DE TAREFAS */
#define NOME_FICH 12 /* NUMERO DE CARATERES DE UM FICHEIRO */

typedef struct buffer
{
	char files[MAX_TAREFA][NOME_FICH+1];
	sem_t prod, cons;
	
}*buffer_t;

void* leTerminal(void*);
void* multiLeitor(void*);

int main(){
	buffer_t buf = (buffer_t)malloc(sizeof(struct buffer));
	sem_init(&buf->prod,0,MAX_TAREFA);
	sem_init(&buf->cons,0,0);
	int tid_input, tid_leitura; 
	pthread_t input; 
	pthread_t leitura;
	if((tid_input = pthread_create(&input,NULL,leTerminal,(void*)buf))!=0){
		perror("ERRO: TAREFA DE INPUT NAO CONCLUIDA");
		exit(-1);
	}
	sleep(20);
	if((tid_leitura= pthread_create(&leitura,NULL,multiLeitor,(void*)buf))!=0){
		perror("ERRO: TAREFA DE LEITURA NAO CONCLUIDA");
		exit(-1);
	}
	while(1);
	return 0;
}

void* leTerminal(void* arg){
	buffer_t buf = (buffer_t)arg;
	buf->files;
	long int i = 0;
	while(1){
		sem_wait(&buf->prod);
		printf("Input %ld\n", i);
		if(fgets(buf->files[i%MAX_TAREFA],NOME_FICH+1,stdin)==NULL){
			perror("ERRO DE LEITURA");
			pthread_exit(NULL);
		}
		i++;
		sem_post(&buf->cons);
	}
}

void* multiLeitor(void* arg){
	buffer_t buf = (buffer_t)arg;
	long int i = 0;
	char* nomeFich = (char*) malloc(sizeof(char)*(NOME_FICH+1));
	while(1){
		sem_wait(&buf->cons);
		if(strncmp(buf->files[i%MAX_TAREFA],"SO2014",4)==0){
			strcpy(nomeFich, buf->files[i%MAX_TAREFA]);
			printf("%s\n", nomeFich);
		}
		i++;
		sem_post(&buf->prod);
	}
}