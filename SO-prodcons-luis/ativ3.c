#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <semaphore.h>
	#define MAX_BUFFER 5 //Qtd de posicoes do buffer
	#define MAX_PRIMOS 25 //Qtd de numeros produzidos

	//Semaforos
	sem_t vazio;
	sem_t cheio;
	sem_t mutex;

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
		int i = 0, fibo = 1, a = 0, b = 1, j = 0, value;

		printf("Produtor iniciando...\n");
		
		while(i < MAX_PRIMOS){
			
			sem_getvalue(&vazio, &value);
			while(value != 5) sem_getvalue(&vazio, &value);
			sem_wait(&mutex);
			while(j < MAX_BUFFER){
				sem_wait(&vazio);
			
				pos_prod = (pos_prod + 1) % MAX_BUFFER; //Proxima posicao de insercao
				buffer[pos_prod] = fibo; //Insere fibo na posicao pos_prod do buffer
				if(i > 0){
					a = b;
					b = fibo;
					fibo = a + b;
				}
				i++; j++;
				printf("Produzindo item [%d] na posicao: [%d]\n", buffer[pos_prod], pos_prod);

				sem_post(&cheio);
			}
			j = 0;
			sem_post(&mutex);
		}
		printf("Produtor saindo...\n");
	}

	void *Consumidor(void *arg){
		printf("[%d] Consumidor iniciando...\n", pthread_self());
		while(max_cons > 0){
			max_cons--;    //Decrementa variavel global
			sem_wait(&cheio);
			sem_wait(&mutex);
			pos_cons = (pos_cons + 1) % MAX_BUFFER; //Proxima posicao de consumo
			printf("[%d] Consumindo item [%d] na posicao [%d]\n", pthread_self(), buffer[pos_cons], pos_cons);
			if(VerificaPrimo(buffer[pos_cons])){     //Verifica se eh primo
				printf("[%d] eh primo!\n", buffer[pos_cons]);
			}
			sem_post(&mutex);
			sem_post(&vazio);
		}
		
		printf("[%d] Consumidor saindo...\n", pthread_self());
	}

	int main(int argc, char *argv[]){
		//Verificacoes de parametros 
		if(argc != 2){
			printf("Uso: %s (numero de consumidores)\n", argv[0]);
			exit(-1);
		}
		if(atoi(argv[1]) < 1){
			printf("O numero de consumidores deve ser maior ou igual a 1!\n");
			exit(-1);
		}

		int num_cons = atoi(argv[1]);

		//Threads
		pthread_t produtor, consumidor[num_cons];
		
		//Init dos semaforos
		sem_init(&vazio, 0, MAX_BUFFER);
		sem_init(&cheio, 0, 0);
		sem_init(&mutex, 0, 1);

		//Criacao e verificacao de erros das threads
		int rc_p, rc_c, i;
		rc_p = pthread_create(&produtor, NULL, Produtor, NULL);
		if (rc_p){                
	    	printf("ERRO! Codigo de retorno de pthread_create() eh %d\n", rc_p);                            
	    	exit(-1);                          
	    }

	    for(i = 0; i < num_cons; i++){
	    	rc_c = pthread_create(&consumidor[i], NULL, Consumidor, NULL);
		    if (rc_c){                
		    	printf("ERRO! Codigo de retorno de pthread_create() eh %d\n", rc_c);                            
		    	exit(-1);                          
		    }
	    }

	    //Join de threads
		pthread_join(produtor, NULL);
		for(i = 0; i < num_cons; i++){
			pthread_join(consumidor[i], NULL);	
		}
		
		return 0;
	}
