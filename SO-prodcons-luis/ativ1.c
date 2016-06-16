#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX_BUFFER 5 //Qtd de posicoes do buffer
#define MAX_PRIMOS 25 //Qtd de numeros produzidos

//Semaforos
sem_t vazio;
sem_t cheio;

//Buffer
int buffer[MAX_BUFFER];

//Posicoes iniciais dos produtores/consumidores
int pos_prod = MAX_BUFFER - 1;
int pos_cons = MAX_BUFFER - 1;

//Variavel usada para parar o consumidor apos 25 numeros
int max_cons = 25;

//Verifica se o numero eh primo
int VerificaPrimo(int alvo){
	if(alvo <= 1 || (alvo != 2 && alvo % 2 == 0)){ //Nenhum inteiro menor que 1 ou par maior que 2 eh primo
		return 0;
	} else{
		int aux = 3;
		while(aux <= alvo / 2){
			if(alvo % aux == 0){
				return 0;
			} else{
				aux += 2;
			}
		}
		return 1;
	}
}

void *Produtor(void *arg){
	int i = 0, fibo = 1, a = 0, b = 1;

	printf("Produtor iniciando...\n");
	while(i < MAX_PRIMOS){
		sem_wait(&vazio);
		pos_prod = (pos_prod + 1) % MAX_BUFFER; //Proxima posicao de insercao
		buffer[pos_prod] = fibo; //Insere fibo na posicao pos_prod do buffer
		if(i > 0){
			a = b;
			b = fibo;
			fibo = a + b;
		}
		i++;
		printf("Produzindo item [%d] na posicao: [%d]\n", buffer[pos_prod], pos_prod);
		sem_post(&cheio);
	}
	printf("Produtor saindo...\n");
}

void *Consumidor(void *arg){
	printf("Consumidor iniciando...\n");
	while(max_cons > 0){
		max_cons--; //Decrementa variavel global
		sem_wait(&cheio);
		pos_cons = (pos_cons + 1) % MAX_BUFFER; //Proxima posicao de consumo
		printf("Consumindo item [%d] na posicao [%d]\n", buffer[pos_cons], pos_cons);
		if(VerificaPrimo(buffer[pos_cons])){     //Verifica se eh primo
			printf("[%d] eh primo!\n", buffer[pos_cons]);
		}
		sem_post(&vazio);
	}
	printf("Consumidor saindo...\n");
}

int main(int argc, char *argv[]){
	//Threads
	pthread_t produtor, consumidor;
	
	//Init dos semaforos
	sem_init(&vazio, 0, MAX_BUFFER);
	sem_init(&cheio, 0, 0);

	//Criacao das threads
	int rc_p, rc_c;
	rc_p = pthread_create(&produtor, NULL, Produtor, NULL);
	rc_c = pthread_create(&consumidor, NULL, Consumidor, NULL);

	//Verificacao de erros na criacao das threads
	if (rc_p){                
    	printf("ERRO! Codigo de retorno de pthread_create() eh %d\n", rc_p);                            
    	exit(-1);                          
    }
    if (rc_c){                
    	printf("ERRO! Codigo de retorno de pthread_create() eh %d\n", rc_c);                            
    	exit(-1);                          
    }

    //Join de threads
	pthread_join(produtor, NULL);
	pthread_join(consumidor, NULL);
	return 0;
}