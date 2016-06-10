#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX 5

//Semaforos
sem_t vazio;
sem_t cheio;
sem_t mutex;

int buffer[5];

void *Produtor(void *arg){
	int i, fibo, a = 0, b = 1;

	for(i = 0; i < 25; i++){
		fibo = a + b;
		printf("[%d] Fibo: [%d]\n", i + 1, fibo);
		a = b;
		b = fibo;
	}
}

void *Consumidor(void *arg){
	printf("lel\n");
}

int main(int argc, char *argv[]){
	pthread_t produtor, consumidor;
	sem_init(&vazio, 0, MAX);
	sem_init(&cheio, 0, 0);
	sem_init(&mutex, 0, 1);

	int rc_p, rc_c;
	rc_p = pthread_create(&produtor, NULL, Produtor, NULL);
	rc_c = pthread_create(&consumidor, NULL, Consumidor, NULL);
	if (rc_p){                
    	printf("ERRO! Codigo de retorno de pthread_create() eh %d\n", rc_p);                            
    	exit(-1);                          
    }
    if (rc_c){                
    	printf("ERRO! Codigo de retorno de pthread_create() eh %d\n", rc_c);                            
    	exit(-1);                          
    }

	pthread_join(produtor, NULL);
	pthread_join(consumidor, NULL);
	return 0;
}