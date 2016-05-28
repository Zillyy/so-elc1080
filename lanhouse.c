#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#define NUM_PC 3
#define LIVRE 0
#define EM_USO 1

//Criacao dos semaforos
sem_t mutex_pcs[3];
sem_t mutex_lan;

//Área compartilhada
typedef struct{
	int pcs[3];
	int fila_de_espera;
} lanhouse;
lanhouse lan; 

typedef struct{
	int cliente_id;
} param_cliente;

//verifica se há pcs disponiveis e retorna o indice do pc desligado ha mais tempo
int verifica_pcs(){
	int i;
	for(i = 0; i < NUM_PC; i++){
		if(lan.pcs[i] == LIVRE){
			return i;
		}
	}
	return -1;
}

void *Lanhouse(void *arg){
	param_cliente *cliente = arg;

	printf("Cliente [%d] chegando\n", cliente->cliente_id);
	if(lan.fila_de_espera >= 15){
		printf("Cliente [%d] saindo: Sala de espera lotada\n", cliente->cliente_id);
		return NULL;
	} else{
		//sem_wait(&mutex_lan);
		

		int i = verifica_pcs();
		//sem_post(&mutex_lan);
		if(i == -1){
			lan.fila_de_espera++;
		}

		lan.fila_de_espera--;
		lan.pcs[i] == EM_USO;
		
		sem_wait(&mutex_pcs[i]);
		printf("PC [%d] sendo usado pelo cliente [%d]\n", i + 1, cliente->cliente_id);
		usleep(10000 * (8 + (rand()%2)));
		printf("PC [%d] esta livre pelo cliente [%d]\n", i + 1, cliente->cliente_id);
		sem_post(&mutex_pcs[i]);
		lan.pcs[i] == LIVRE;

		printf("Cliente [%d] saindo\n", cliente->cliente_id);
		
		return NULL;
	}
	
}

int main(int argc, char *argv[]){
	//Verificacoes de parametros 
	if(argc != 2){
		printf("Uso: %s (numero de usuarios)\n", argv[0]);
		exit(-1);
	}
	if(atoi(argv[1]) < 1){
		printf("O numero de usuarios deve ser maior ou igual a 1!\n");
		exit(-1);
	}

	srand((unsigned)time(NULL));

	int rc, i;
	int num_usuarios = atoi(argv[1]);
	pthread_t usuarios[num_usuarios];
	param_cliente *cliente = (param_cliente *) malloc(sizeof(param_cliente) * num_usuarios);

	//Preenche parametros de cada cliente/thread
	for (i = 0; i < num_usuarios; i++){
		cliente[i].cliente_id = i + 1;
	}

	lan.fila_de_espera = 0;

	//Inicializacao dos mutexes dos pcs e da lan e das flags dos pcs
	for (i = 0; i < NUM_PC; i++){
		sem_init(&mutex_pcs[i], 0, 1);
		lan.pcs[i] = LIVRE;
	}
	sem_init(&mutex_lan, 0, 1);

	//Criacao das threads dos usuarios
	for(i = 0; i < num_usuarios; i++){	
		usleep(10000 * (1 + (rand()%4)));
		rc = pthread_create(&usuarios[i], NULL, Lanhouse, (void *)&cliente[i]);                            
   		if (rc){                
       		printf("ERRO! Codigo de retorno de pthread_create() eh %d\n", rc);                            
        	exit(-1);                          
    	}
	}

	//Join das threads dos usuarios
	for(i = 0; i < num_usuarios; i++){
		pthread_join(usuarios[i], NULL);   
	}
	printf("Lanhouse fechando...\n");

	free(cliente);
	return 0;
}